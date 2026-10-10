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

QList<QModelIndex> TreeModelUtils::path(const QModelIndex& index, const QModelIndex& root) {
    QList<QModelIndex> result;
    if (!belongsToRoot(index, root))
        return result;

    for (QModelIndex node = index; node.isValid() && node != root; node = node.parent())
        result.prepend(node);
    return result;
}

QList<QModelIndex> TreeModelUtils::columnRoots(const QModelIndex& index, const QModelIndex& root) {
    QList<QModelIndex> roots;
    roots.append(root);
    const auto nodes = path(index, root);
    for (const QModelIndex& node : nodes)
        if (!isLeaf(node))
            roots.append(node.sibling(node.row(), 0));
    return roots;
}

} // namespace animatedwidgets
