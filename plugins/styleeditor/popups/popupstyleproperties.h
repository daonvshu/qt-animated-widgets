#pragma once

#include "../utils/styleobjectproperties.h"
#include "animatedwidgets/popups/defaultpopupmenuliststyle.h"
#include "animatedwidgets/popups/defaultpopupmenustyle.h"
#include "animatedwidgets/popups/popupmenu.h"

class PopupStyleProperties {
public:
    static void popupMenu(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void searchPopupMenu(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void treePopupMenu(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void cascaderPopupMenu(animatedwidgets::styleeditor::StylePropertyBuilder& builder);

private:
    static animatedwidgets::styleeditor::StylePropertyGroup search();
    static void body(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void list(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void fontsAndMargins(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void checkAlignment(animatedwidgets::styleeditor::StylePropertyBuilder& builder);

    template <typename Value>
    static void bodyMember(animatedwidgets::styleeditor::StylePropertyBuilder& builder, const QString& name, const QString& label,
                           Value animatedwidgets::DefaultPopupMenuStyle::* field) {
        using animatedwidgets::DefaultPopupMenuStyle;
        using animatedwidgets::PopupMenu;
        StyleObjectProperties::add<PopupMenu>(
                builder, QStringLiteral("Popup body"), QStringLiteral("popup.") + name, label, field,
                [](PopupMenu* widget) { return *static_cast<DefaultPopupMenuStyle*>(widget->popupStyle()); },
                [](PopupMenu* widget, const DefaultPopupMenuStyle& style) { widget->setPopupStyle(new DefaultPopupMenuStyle(style)); },
                QStringLiteral("*static_cast<animatedwidgets::DefaultPopupMenuStyle*>(widget->popupStyle())"),
                QStringLiteral("setPopupStyle(new animatedwidgets::DefaultPopupMenuStyle(style))"));
    }

    template <typename Value>
    static void listMember(animatedwidgets::styleeditor::StylePropertyBuilder& builder, const QString& name, const QString& label,
                           Value animatedwidgets::DefaultPopupMenuListStyle::* field) {
        using animatedwidgets::DefaultPopupMenuListStyle;
        using animatedwidgets::PopupMenu;
        StyleObjectProperties::add<PopupMenu>(
                builder, QStringLiteral("List items"), QStringLiteral("list.") + name, label, field,
                [](PopupMenu* widget) { return *static_cast<DefaultPopupMenuListStyle*>(widget->listStyle()); },
                [](PopupMenu* widget, const DefaultPopupMenuListStyle& style) { widget->setListStyle(new DefaultPopupMenuListStyle(style)); },
                QStringLiteral("*static_cast<animatedwidgets::DefaultPopupMenuListStyle*>(widget->listStyle())"),
                QStringLiteral("setListStyle(new animatedwidgets::DefaultPopupMenuListStyle(style))"));
    }
};
