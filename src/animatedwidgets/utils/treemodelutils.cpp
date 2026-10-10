#include "animatedwidgets/utils/treemodelutils.h"

#include <QAbstractItemModel>

namespace animatedwidgets {

bool TreeModelUtils::isLeaf(const QModelIndex& index) {
    return index.isValid() && !index.model()->hasChildren(index.sibling(index.row(), 0));
}

bool TreeModelUtils::belongsToRoot(const QModelIndex& index, const QModelIndex& root) {
    if (!index.isValid() || (root.isValid() && index.model() != root.model()))
        return false;
    for (QModelIndex parent = index.parent(); parent.isValid(); parent = parent.parent()) {
        if (parent == root)
            return true;
    }
    return !root.isValid();
}

bool TreeModelUtils::isSelectable(const QModelIndex& index, bool leafOnly) {
    if (!index.isValid() || !index.flags().testFlag(Qt::ItemIsEnabled)
        || !index.flags().testFlag(Qt::ItemIsSelectable) || (leafOnly && !isLeaf(index)))
        return false;
    for (QModelIndex parent = index.parent(); parent.isValid(); parent = parent.parent()) {
        if (!parent.flags().testFlag(Qt::ItemIsEnabled))
            return false;
    }
    return true;
}

} // namespace animatedwidgets
