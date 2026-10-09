#pragma once

#include <QList>
#include <QMap>
#include <QString>
#include <QVariant>

#include "animatedwidgets/popups/popupmenuitem.h"

namespace animatedwidgets {

class PopupMenuGroup {
public:
    QString title;
    QList<PopupMenuItem> items;
    bool multiSelection = false;

    QMap<int, QVariant> values; // Additional roles for group data and custom delegates.

    QList<PopupMenuNode> children; // Appended after items; nested groups become submenu entries.
};

} // namespace animatedwidgets
