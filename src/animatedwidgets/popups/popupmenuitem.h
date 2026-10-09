#pragma once

#include "animatedwidgets/popups/popupmenunode.h"

#include <QIcon>
#include <QMap>
#include <QString>
#include <QVariant>

namespace animatedwidgets {

class PopupMenuItem {
public:
    enum Role { SeparatorRole = Qt::UserRole + 1, ShortcutRole, TypeRole, HighlightRole, GroupIndexRole,
                ChildrenRole, SubmenuMultiSelectionRole };

    QString text;
    QIcon icon;
    QString shortcut;
    bool separator = false;
    bool enabled = true;
    QMap<int, QVariant> values; // Additional roles for application data and custom delegates.

    QList<PopupMenuNode> children; // Items and groups displayed in the next menu level.
    bool submenuMultiSelection = false;
};

} // namespace animatedwidgets
