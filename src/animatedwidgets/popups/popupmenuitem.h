#pragma once

#include <QIcon>
#include <QMap>
#include <QString>
#include <QVariant>

namespace animatedwidgets {

class PopupMenuItem {
public:
    enum Type { Option, GroupTitle, Separator };
    enum Role { SeparatorRole = Qt::UserRole + 1, ShortcutRole, TypeRole, HighlightRole };

    QString text;
    QIcon icon;
    QString shortcut;
    Type type = Option;
    bool enabled = true;
    QMap<int, QVariant> values; // Additional roles for application data and custom delegates.
};

} // namespace animatedwidgets
