#include "animatedwidgets/popups/popupmenufiltermodel.h"
#include "animatedwidgets/popups/popupmenurowtype.h"
#include "animatedwidgets/popups/popupmenuitem.h"
#include "animatedwidgets/utils/textsearchutils.h"

namespace animatedwidgets {

PopupMenuFilterModel::PopupMenuFilterModel(QObject* parent)
    : QSortFilterProxyModel(parent)
{
    setFilterKeyColumn(0);
}

QString PopupMenuFilterModel::searchText() const {
    return m_searchText;
}

void PopupMenuFilterModel::setSearchText(const QString& text) {
    const QString query = text.trimmed();
    if (m_searchText == query)
        return;
    m_searchText = query;
    invalidateFilter();
    notifyHighlightChanged();
}

bool PopupMenuFilterModel::highlightEnabled() const {
    return m_highlightEnabled;
}

void PopupMenuFilterModel::setHighlightEnabled(bool enabled) {
    if (m_highlightEnabled == enabled)
        return;
    m_highlightEnabled = enabled;
    notifyHighlightChanged();
}

QColor PopupMenuFilterModel::highlightColor() const {
    return m_highlightColor;
}

void PopupMenuFilterModel::setHighlightColor(const QColor& color) {
    if (!color.isValid() || m_highlightColor == color)
        return;
    m_highlightColor = color;
    notifyHighlightChanged();
}

bool PopupMenuFilterModel::highlightBold() const {
    return m_highlightBold;
}

void PopupMenuFilterModel::setHighlightBold(bool bold) {
    if (m_highlightBold == bold)
        return;
    m_highlightBold = bold;
    notifyHighlightChanged();
}

QVariant PopupMenuFilterModel::data(const QModelIndex& index, int role) const {
    if (role != PopupMenuItem::HighlightRole)
        return QSortFilterProxyModel::data(index, role);
    if (!index.isValid() || index.model() != this || !m_highlightEnabled || m_searchText.isEmpty()
        || QSortFilterProxyModel::data(index, PopupMenuItem::TypeRole).toInt() != static_cast<int>(PopupMenuRowType::Option)
        || QSortFilterProxyModel::data(index, PopupMenuItem::SeparatorRole).toBool())
        return QVariant();

    bool matched = false;
    const QString html = TextSearchUtils::searchAndHighlight(
        QSortFilterProxyModel::data(index, Qt::DisplayRole).toString(), m_searchText,
        m_highlightColor, m_highlightBold, &matched);
    return matched ? QVariant(html) : QVariant();
}

void PopupMenuFilterModel::notifyHighlightChanged() {
    if (rowCount() > 0 && columnCount() > 0)
        emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1), {PopupMenuItem::HighlightRole});
}

bool PopupMenuFilterModel::matchesOption(int row, const QModelIndex& parent) const {
    const QModelIndex index = sourceModel()->index(row, qMax(0, filterKeyColumn()), parent);
    if (index.data(PopupMenuItem::SeparatorRole).toBool()
        || index.data(PopupMenuItem::TypeRole).toInt() != static_cast<int>(PopupMenuRowType::Option))
        return false;

    const int firstColumn = qMax(0, filterKeyColumn());
    const int lastColumn = filterKeyColumn() < 0 ? sourceModel()->columnCount(parent) : firstColumn + 1;
    for (int column = firstColumn; column < lastColumn; ++column) {
        if (TextSearchUtils::matches(sourceModel()->index(row, column, parent).data(filterRole()).toString(), m_searchText))
            return true;
    }
    return false;
}

void PopupMenuFilterModel::setSourceModel(QAbstractItemModel* model) {
    if (model == this || model == sourceModel())
        return;

    for (const auto& connection : m_sourceConnections)
        disconnect(connection);
    m_sourceConnections.clear();
    QSortFilterProxyModel::setSourceModel(model);
    if (!model)
        return;

    // Group heading visibility depends on other rows, not only on the changed row.
    m_sourceConnections.append(connect(model, &QAbstractItemModel::dataChanged, this,
        [this](const QModelIndex&, const QModelIndex&, const QVector<int>& roles) {
            if (roles.isEmpty() || roles.contains(filterRole()) || roles.contains(PopupMenuItem::TypeRole)
                || roles.contains(PopupMenuItem::SeparatorRole) || roles.contains(PopupMenuItem::GroupIndexRole))
                invalidateFilter();
        }));
    m_sourceConnections.append(connect(model, &QAbstractItemModel::rowsInserted, this, [this] { invalidateFilter(); }));
    m_sourceConnections.append(connect(model, &QAbstractItemModel::rowsRemoved, this, [this] { invalidateFilter(); }));
    m_sourceConnections.append(connect(model, &QAbstractItemModel::rowsMoved, this, [this] { invalidateFilter(); }));
    m_sourceConnections.append(connect(model, &QAbstractItemModel::layoutChanged, this, [this] { invalidateFilter(); }));
}

bool PopupMenuFilterModel::filterAcceptsRow(int row, const QModelIndex& parent) const {
    if (!sourceModel())
        return false;
    if (m_searchText.isEmpty())
        return true;

    const QModelIndex index = sourceModel()->index(row, qMax(0, filterKeyColumn()), parent);
    if (index.data(PopupMenuItem::SeparatorRole).toBool())
        return false;
    if (index.data(PopupMenuItem::TypeRole).toInt() != static_cast<int>(PopupMenuRowType::GroupTitle))
        return matchesOption(row, parent);

    // A heading stays visible only when its following group contains a matching option.
    const QVariant groupIndex = index.data(PopupMenuItem::GroupIndexRole);
    for (int next = row + 1; next < sourceModel()->rowCount(parent); ++next) {
        const QModelIndex candidate = sourceModel()->index(next, qMax(0, filterKeyColumn()), parent);
        if (candidate.data(PopupMenuItem::TypeRole).toInt() == static_cast<int>(PopupMenuRowType::GroupTitle)
            || (groupIndex.isValid() && candidate.data(PopupMenuItem::GroupIndexRole) != groupIndex)
            || (!groupIndex.isValid() && candidate.data(PopupMenuItem::SeparatorRole).toBool()))
            break;
        if (matchesOption(next, parent))
            return true;
    }
    return false;
}

} // namespace animatedwidgets
