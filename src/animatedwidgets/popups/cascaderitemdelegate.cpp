#include "animatedwidgets/popups/cascaderitemdelegate.h"

#include "animatedwidgets/utils/treemodelutils.h"

namespace animatedwidgets {

CascaderItemDelegate::CascaderItemDelegate(QObject* parent)
    : PopupMenuItemDelegate(parent)
{
}

void CascaderItemDelegate::setPathEndIndex(const QModelIndex& index) {
    m_pathEndIndex = index;
}

bool CascaderItemDelegate::hasChildItems(const QModelIndex& index) const {
    return index.isValid() && !TreeModelUtils::isLeaf(index);
}

QVariant CascaderItemDelegate::itemCheckState(const QModelIndex& index) const {
    const QVariant checkState = PopupMenuItemDelegate::itemCheckState(index);
    if (checkState.isValid() && checkState.toInt() == Qt::Checked && m_pathEndIndex.isValid()
        && index != m_pathEndIndex)
        return Qt::Unchecked;
    return checkState;
}

bool CascaderItemDelegate::isItemSelected(const QStyleOptionViewItem& option, const QModelIndex& index) const {
    if (hasChildItems(index))
        return index.flags().testFlag(Qt::ItemIsEnabled)
            && (index == m_pathEndIndex || TreeModelUtils::belongsToRoot(m_pathEndIndex, index));
    return PopupMenuItemDelegate::isItemSelected(option, index);
}

} // namespace animatedwidgets
