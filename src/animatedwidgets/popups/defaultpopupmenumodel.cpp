#include "animatedwidgets/popups/defaultpopupmenumodel.h"

#include <algorithm>

namespace animatedwidgets {

DefaultPopupMenuModel::DefaultPopupMenuModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

int DefaultPopupMenuModel::rowCount(const QModelIndex& parent) const {
    return parent.isValid() ? 0 : static_cast<int>(m_strings.size());
}

QVariant DefaultPopupMenuModel::data(const QModelIndex& index, int role) const {
    if (!ownsIndex(index))
        return QVariant();

    if (role == Qt::DisplayRole)
        return m_strings.at(index.row());
    if (role == Qt::CheckStateRole)
        return m_selectedIndexes.contains(index.row()) ? Qt::Checked : Qt::Unchecked;
    return QVariant();
}

Qt::ItemFlags DefaultPopupMenuModel::flags(const QModelIndex& index) const {
    if (!ownsIndex(index))
        return Qt::NoItemFlags;

    if (m_disabledRows.contains(index.row()))
        return Qt::ItemIsSelectable;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool DefaultPopupMenuModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!ownsIndex(index) || role != Qt::CheckStateRole)
        return false;

    bool valid = false;
    const int state = value.toInt(&valid);
    if (!valid || (state != Qt::Checked && state != Qt::Unchecked))
        return false;

    QList<int> rows = m_selectedIndexes;
    if (state == Qt::Unchecked)
        rows.removeAll(index.row());
    else if (m_selectionMode == SingleSelection)
        rows = {index.row()};
    else if (!rows.contains(index.row()))
        rows.append(index.row());
    applySelection(rows);
    return true;
}

QStringList DefaultPopupMenuModel::strings() const {
    return m_strings;
}

void DefaultPopupMenuModel::setStrings(const QStringList& strings) {
    if (m_strings == strings)
        return;

    const bool hadSelection = !m_selectedIndexes.isEmpty();
    beginResetModel();
    m_strings = strings;
    m_selectedIndexes.clear();
    m_disabledRows.clear();
    endResetModel();
    if (hadSelection)
        notifySelection();
}

DefaultPopupMenuModel::SelectionMode DefaultPopupMenuModel::selectionMode() const {
    return m_selectionMode;
}

void DefaultPopupMenuModel::setSelectionMode(SelectionMode mode) {
    if (mode == m_selectionMode || (mode != SingleSelection && mode != MultiSelection))
        return;

    m_selectionMode = mode;
    if (mode == SingleSelection && m_selectedIndexes.size() > 1)
        applySelection({m_selectedIndexes.first()});
    else
        notifySelection();
}

int DefaultPopupMenuModel::currentIndex() const {
    return m_selectedIndexes.isEmpty() ? -1 : m_selectedIndexes.first();
}

void DefaultPopupMenuModel::setCurrentIndex(int row) {
    if (row == -1)
        applySelection({});
    else if (row >= 0 && row < rowCount())
        applySelection({row});
}

QList<int> DefaultPopupMenuModel::selectedIndexes() const {
    return m_selectedIndexes;
}

void DefaultPopupMenuModel::setSelectedIndexes(const QList<int>& rows) {
    applySelection(rows);
}

void DefaultPopupMenuModel::setItemEnabled(int row, bool enabled) {
    if (row < 0 || row >= rowCount() || enabled == !m_disabledRows.contains(row))
        return;

    if (enabled)
        m_disabledRows.remove(row);
    else
        m_disabledRows.insert(row);
    emit dataChanged(index(row), index(row), {});
}

void DefaultPopupMenuModel::triggerItem(const QModelIndex& index) {
    if (!ownsIndex(index) || !flags(index).testFlag(Qt::ItemIsEnabled))
        return;

    const bool checked = m_selectedIndexes.contains(index.row());
    setData(index, m_selectionMode == MultiSelection && checked ? Qt::Unchecked : Qt::Checked,
            Qt::CheckStateRole);
}

bool DefaultPopupMenuModel::ownsIndex(const QModelIndex& index) const {
    return index.isValid() && index.model() == this && index.column() == 0
        && !index.parent().isValid() && index.row() >= 0 && index.row() < rowCount();
}

void DefaultPopupMenuModel::applySelection(const QList<int>& rows) {
    QList<int> selected;
    for (int row : rows) {
        if (row >= 0 && row < rowCount() && !selected.contains(row)) {
            selected.append(row);
            if (m_selectionMode == SingleSelection)
                break;
        }
    }
    std::sort(selected.begin(), selected.end());
    if (selected == m_selectedIndexes)
        return;

    m_selectedIndexes = selected;
    if (rowCount() > 0)
        emit dataChanged(index(0), index(rowCount() - 1), {Qt::CheckStateRole});
    notifySelection();
}

void DefaultPopupMenuModel::notifySelection() {
    if (m_selectionMode == SingleSelection)
        emit currentIndexChanged(currentIndex());
    else
        emit selectedIndexesChanged(m_selectedIndexes);
}

} // namespace animatedwidgets
