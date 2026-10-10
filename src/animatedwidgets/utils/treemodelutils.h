#pragma once

#include <QModelIndex>

namespace animatedwidgets {

class TreeModelUtils {
public:
    static bool isLeaf(const QModelIndex& index);
    static bool belongsToRoot(const QModelIndex& index, const QModelIndex& root);
    static bool isSelectable(const QModelIndex& index, bool leafOnly);
};

} // namespace animatedwidgets
