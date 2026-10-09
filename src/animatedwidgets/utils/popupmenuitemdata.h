#pragma once

#include "animatedwidgets/popups/popupmenuitem.h"

namespace animatedwidgets {

class PopupMenuItemData {
public:
    static QVariant data(const PopupMenuItem& item, int role);
    static bool setData(PopupMenuItem& item, const QVariant& value, int role);
};

} // namespace animatedwidgets
