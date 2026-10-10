#pragma once

#include <QModelIndex>
#include <QList>

namespace animatedwidgets {

class TreeModelUtils {
public:
    static bool isLeaf(const QModelIndex& index);
    static bool belongsToRoot(const QModelIndex& index, const QModelIndex& root);
    static bool isSelectable(const QModelIndex& index, bool leafOnly);
    static QList<QModelIndex> path(const QModelIndex& index, const QModelIndex& root = QModelIndex());
    static QList<QModelIndex> columnRoots(const QModelIndex& index, const QModelIndex& root = QModelIndex());
};

} // namespace animatedwidgets
