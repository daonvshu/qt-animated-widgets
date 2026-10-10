#include "animatedwidgets/comboboxes/treepopupmodel.h"

#include "animatedwidgets/popups/popupmenuitem.h"
#include "animatedwidgets/utils/treemodelutils.h"

namespace animatedwidgets {

TreePopupModel::TreePopupModel(QObject* parent)
    : ComboPopupModel(parent)
{
}

QVariant TreePopupModel::data(const QModelIndex& index, int role) const {
    if (role == PopupMenuItem::ChildrenRole)
        return QVariant();
    return ComboPopupModel::data(index, role);
}

Qt::ItemFlags TreePopupModel::flags(const QModelIndex& index) const {
    Qt::ItemFlags itemFlags = ComboPopupModel::flags(index);
    if (!index.isValid() || index.model() != this)
        return itemFlags;
    if (m_leafOnly && !TreeModelUtils::isLeaf(mapToSource(index)))
        itemFlags.setFlag(Qt::ItemIsSelectable, false);
    for (QModelIndex parent = index.parent(); parent.isValid(); parent = parent.parent()) {
        if (!ComboPopupModel::flags(parent).testFlag(Qt::ItemIsEnabled))
            itemFlags.setFlag(Qt::ItemIsEnabled, false);
    }
    return itemFlags;
}

bool TreePopupModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (role == PopupMenuItem::ChildrenRole)
        return false;
    return ComboPopupModel::setData(index, value, role);
}

bool TreePopupModel::leafOnly() const {
    return m_leafOnly;
}

void TreePopupModel::setLeafOnly(bool enabled) {
    if (m_leafOnly == enabled)
        return;
    beginResetModel();
    m_leafOnly = enabled;
    endResetModel();
}

} // namespace animatedwidgets
