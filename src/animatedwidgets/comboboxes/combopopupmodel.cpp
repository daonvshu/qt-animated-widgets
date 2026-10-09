#include "animatedwidgets/comboboxes/combopopupmodel.h"

#include "animatedwidgets/popups/popupmenuitemdelegate.h"

namespace animatedwidgets {

ComboPopupModel::ComboPopupModel(QObject* parent)
    : QIdentityProxyModel(parent)
{
}

QVariant ComboPopupModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.model() != this)
        return QVariant();

    if (role == PopupMenuItemDelegate::SeparatorRole)
        return QIdentityProxyModel::data(index, Qt::AccessibleDescriptionRole).toString() == QStringLiteral("separator");
    if (role == PopupMenuItemDelegate::ShortcutRole || role == PopupMenuItem::HighlightRole)
        return QVariant();
    if (role == PopupMenuItemDelegate::TypeRole) {
        const QString description = QIdentityProxyModel::data(index, Qt::AccessibleDescriptionRole).toString();
        if (description == QStringLiteral("groupTitle"))
            return PopupMenuItem::GroupTitle;
        return description == QStringLiteral("separator") ? PopupMenuItem::Separator : PopupMenuItem::Option;
    }
    if (role == Qt::CheckStateRole) {
        if (data(index, PopupMenuItem::TypeRole).toInt() != PopupMenuItem::Option)
            return QVariant();
        return mapToSource(index) == m_currentIndex ? Qt::Checked : Qt::Unchecked;
    }
    return QIdentityProxyModel::data(index, role);
}

Qt::ItemFlags ComboPopupModel::flags(const QModelIndex& index) const {
    if (!index.isValid() || index.model() != this
        || data(index, PopupMenuItem::TypeRole).toInt() != PopupMenuItem::Option)
        return Qt::NoItemFlags;
    Qt::ItemFlags itemFlags = QIdentityProxyModel::flags(index);
    itemFlags.setFlag(Qt::ItemIsUserCheckable, false);
    return itemFlags;
}

bool ComboPopupModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!index.isValid() || index.model() != this || role == Qt::CheckStateRole
        || role == PopupMenuItemDelegate::SeparatorRole || role == PopupMenuItemDelegate::ShortcutRole
        || role == PopupMenuItemDelegate::TypeRole || role == PopupMenuItem::HighlightRole)
        return false;
    return QIdentityProxyModel::setData(index, value, role);
}

void ComboPopupModel::setSourceModel(QAbstractItemModel* sourceModel) {
    if (sourceModel == this->sourceModel())
        return;

    m_currentIndex = QModelIndex();
    QIdentityProxyModel::setSourceModel(sourceModel);
}

void ComboPopupModel::setCurrentIndex(const QModelIndex& sourceIndex) {
    if (sourceIndex.isValid() && sourceIndex.model() != sourceModel())
        return;
    if (m_currentIndex == sourceIndex)
        return;

    const QModelIndex previous = mapFromSource(m_currentIndex);
    m_currentIndex = sourceIndex;
    const QModelIndex current = mapFromSource(m_currentIndex);
    if (previous.isValid())
        emit dataChanged(previous, previous, {Qt::CheckStateRole});
    if (current.isValid())
        emit dataChanged(current, current, {Qt::CheckStateRole});
}

} // namespace animatedwidgets
