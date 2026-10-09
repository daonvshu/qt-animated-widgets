#pragma once

#include <QIcon>
#include <QMap>
#include <QString>
#include <QVariant>

namespace animatedwidgets {

class PopupMenuItem {
public:
    enum Role { SeparatorRole = Qt::UserRole + 1, ShortcutRole, TypeRole, HighlightRole, GroupIndexRole };

    QString text;
    QIcon icon;
    QString shortcut;
    bool separator = false;
    bool enabled = true;
    QMap<int, QVariant> values; // Additional roles for application data and custom delegates.
};

} // namespace animatedwidgets
