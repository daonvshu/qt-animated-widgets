#include "popupstyleproperties.h"

#include "../utils/stylepluginproperties.h"
#include "animatedwidgets/inputs/iconlineedit.h"
#include "animatedwidgets/popups/searchpopupmenu.h"
#include "animatedwidgets/popups/treepopupmenu.h"

using animatedwidgets::DefaultPopupMenuListStyle;
using animatedwidgets::DefaultPopupMenuStyle;
using animatedwidgets::PopupMenu;
using animatedwidgets::styleeditor::StylePropertyBuilder;
using P = StylePluginProperties;

void PopupStyleProperties::popupMenu(StylePropertyBuilder& builder) {
    builder.add(QStringLiteral("iconHint"), QStringLiteral("Tint icons"));
    StyleObjectProperties::method<PopupMenu, bool>(
            builder, QStringLiteral("Behavior"), QStringLiteral("autoClose"), QStringLiteral("Close on activation"),
            [](PopupMenu* widget) { return widget->autoClose(); }, [](PopupMenu* widget, bool value) { widget->setAutoClose(value); },
            QStringLiteral("setAutoClose(%1)"));
    StyleObjectProperties::method<PopupMenu, QString>(
            builder, QStringLiteral("List"), QStringLiteral("scrollBarStyleSheet"), QStringLiteral("Scrollbar stylesheet"),
            [](PopupMenu* widget) { return widget->scrollBarStyleSheet(); },
            [](PopupMenu* widget, const QString& value) { widget->setScrollBarStyleSheet(value); }, QStringLiteral("setScrollBarStyleSheet(%1)"));

    body(builder);
    list(builder);
    fontsAndMargins(builder);
    checkAlignment(builder);
}

void PopupStyleProperties::searchPopupMenu(StylePropertyBuilder& builder) {
    popupMenu(builder);
    builder.addGroups({
        search()
    });
}

animatedwidgets::styleeditor::StylePropertyGroup PopupStyleProperties::search() {
    animatedwidgets::styleeditor::StylePropertyGroup group;
    group.name = QStringLiteral("Search");
    group.properties = {
        P::field(QStringLiteral("searchText"), QStringLiteral("Search text")),
        P::field(QStringLiteral("searchSpacing"), QStringLiteral("Search spacing")),
        P::field(QStringLiteral("highlightEnabled"), QStringLiteral("Highlight matches")),
        P::when(P::field(QStringLiteral("highlightColor"), QStringLiteral("Highlight color")), QStringLiteral("highlightEnabled")),
        P::when(P::field(QStringLiteral("highlightBold"), QStringLiteral("Highlight bold")), QStringLiteral("highlightEnabled"))
    };
    return group;
}

void PopupStyleProperties::treePopupMenu(StylePropertyBuilder& builder) {
    popupMenu(builder);
    using animatedwidgets::TreePopupMenu;
    StyleObjectProperties::method<TreePopupMenu, bool>(
            builder, QStringLiteral("Tree"), QStringLiteral("leafOnly"), QStringLiteral("Leaf selection only"),
            [](TreePopupMenu* widget) { return widget->leafOnly(); }, [](TreePopupMenu* widget, bool value) { widget->setLeafOnly(value); },
            QStringLiteral("setLeafOnly(%1)"));
    StyleObjectProperties::method<TreePopupMenu, bool>(
            builder, QStringLiteral("Tree"), QStringLiteral("searchable"), QStringLiteral("Searchable"),
            [](TreePopupMenu* widget) { return widget->searchable(); }, [](TreePopupMenu* widget, bool value) { widget->setSearchable(value); },
            QStringLiteral("setSearchable(%1)"));
}

void PopupStyleProperties::cascaderPopupMenu(StylePropertyBuilder& builder) {
    popupMenu(builder);
}

void PopupStyleProperties::body(StylePropertyBuilder& builder) {
    bodyMember(builder, QStringLiteral("popupBackground"), QStringLiteral("Popup Background"), &DefaultPopupMenuStyle::popupBackground);
    bodyMember(builder, QStringLiteral("popupBorder"), QStringLiteral("Popup Border"), &DefaultPopupMenuStyle::popupBorder);
    bodyMember(builder, QStringLiteral("shadowColor"), QStringLiteral("Shadow Color"), &DefaultPopupMenuStyle::shadowColor);
    bodyMember(builder, QStringLiteral("popupBorderWidth"), QStringLiteral("Popup Border Width"), &DefaultPopupMenuStyle::popupBorderWidth);
    bodyMember(builder, QStringLiteral("popupCornerRadius"), QStringLiteral("Popup Corner Radius"), &DefaultPopupMenuStyle::popupCornerRadius);
    bodyMember(builder, QStringLiteral("shadowRadius"), QStringLiteral("Shadow Radius"), &DefaultPopupMenuStyle::shadowRadius);
    bodyMember(builder, QStringLiteral("shadowVisible"), QStringLiteral("Shadow Visible"), &DefaultPopupMenuStyle::shadowVisible);
    bodyMember(builder, QStringLiteral("borderVisible"), QStringLiteral("Border Visible"), &DefaultPopupMenuStyle::borderVisible);
    builder.enabledWhen(QStringLiteral("popup.popupBorder"), QStringLiteral("popup.borderVisible"));
    builder.enabledWhen(QStringLiteral("popup.popupBorderWidth"), QStringLiteral("popup.borderVisible"));
    builder.enabledWhen(QStringLiteral("popup.shadowColor"), QStringLiteral("popup.shadowVisible"));
    builder.enabledWhen(QStringLiteral("popup.shadowRadius"), QStringLiteral("popup.shadowVisible"));
}

void PopupStyleProperties::list(StylePropertyBuilder& builder) {
    listMember(builder, QStringLiteral("background"), QStringLiteral("Background"), &DefaultPopupMenuListStyle::background);
    listMember(builder, QStringLiteral("itemBackground"), QStringLiteral("Item Background"), &DefaultPopupMenuListStyle::itemBackground);
    listMember(builder, QStringLiteral("itemHoverBackground"), QStringLiteral("Item Hover Background"), &DefaultPopupMenuListStyle::itemHoverBackground);
    listMember(builder, QStringLiteral("itemSelectedBackground"), QStringLiteral("Item Selected Background"),
               &DefaultPopupMenuListStyle::itemSelectedBackground);
    listMember(builder, QStringLiteral("itemText"), QStringLiteral("Item Text"), &DefaultPopupMenuListStyle::itemText);
    listMember(builder, QStringLiteral("itemDisabledText"), QStringLiteral("Item Disabled Text"), &DefaultPopupMenuListStyle::itemDisabledText);
    listMember(builder, QStringLiteral("separator"), QStringLiteral("Separator"), &DefaultPopupMenuListStyle::separator);
    listMember(builder, QStringLiteral("groupTitleText"), QStringLiteral("Group Title Text"), &DefaultPopupMenuListStyle::groupTitleText);
    listMember(builder, QStringLiteral("groupTitleHeight"), QStringLiteral("Group Title Height"), &DefaultPopupMenuListStyle::groupTitleHeight);
    listMember(builder, QStringLiteral("checkedIcon"), QStringLiteral("Checked Icon"), &DefaultPopupMenuListStyle::checkedIcon);
    listMember(builder, QStringLiteral("uncheckedIcon"), QStringLiteral("Unchecked Icon"), &DefaultPopupMenuListStyle::uncheckedIcon);
    listMember(builder, QStringLiteral("checkedIconSize"), QStringLiteral("Checked Icon Size"), &DefaultPopupMenuListStyle::checkedIconSize);
    listMember(builder, QStringLiteral("submenuIcon"), QStringLiteral("Submenu Icon"), &DefaultPopupMenuListStyle::submenuIcon);
    listMember(builder, QStringLiteral("submenuIconSize"), QStringLiteral("Submenu Icon Size"), &DefaultPopupMenuListStyle::submenuIconSize);
    listMember(builder, QStringLiteral("submenuIconSpacing"), QStringLiteral("Submenu Icon Spacing"), &DefaultPopupMenuListStyle::submenuIconSpacing);
    listMember(builder, QStringLiteral("itemHeight"), QStringLiteral("Item Height"), &DefaultPopupMenuListStyle::itemHeight);
    listMember(builder, QStringLiteral("itemSpacing"), QStringLiteral("Item Spacing"), &DefaultPopupMenuListStyle::itemSpacing);
    listMember(builder, QStringLiteral("itemCornerRadius"), QStringLiteral("Item Corner Radius"), &DefaultPopupMenuListStyle::itemCornerRadius);
    listMember(builder, QStringLiteral("cornerRadius"), QStringLiteral("Corner Radius"), &DefaultPopupMenuListStyle::cornerRadius);
    listMember(builder, QStringLiteral("contentPadding"), QStringLiteral("Content Padding"), &DefaultPopupMenuListStyle::contentPadding);
    listMember(builder, QStringLiteral("itemIconSize"), QStringLiteral("Item Icon Size"), &DefaultPopupMenuListStyle::itemIconSize);
    listMember(builder, QStringLiteral("itemIconSpacing"), QStringLiteral("Item Icon Spacing"), &DefaultPopupMenuListStyle::itemIconSpacing);
    listMember(builder, QStringLiteral("iconSpacing"), QStringLiteral("Icon Spacing"), &DefaultPopupMenuListStyle::iconSpacing);
    listMember(builder, QStringLiteral("checkedIconVisible"), QStringLiteral("Checked Icon Visible"), &DefaultPopupMenuListStyle::checkedIconVisible);
    listMember(builder, QStringLiteral("separatorVisible"), QStringLiteral("Separator Visible"), &DefaultPopupMenuListStyle::separatorVisible);
    builder.enabledWhen(QStringLiteral("list.checkedIcon"), QStringLiteral("list.checkedIconVisible"));
    builder.enabledWhen(QStringLiteral("list.uncheckedIcon"), QStringLiteral("list.checkedIconVisible"));
    builder.enabledWhen(QStringLiteral("list.checkedIconSize"), QStringLiteral("list.checkedIconVisible"));
    builder.enabledWhen(QStringLiteral("list.separator"), QStringLiteral("list.separatorVisible"));
}

void PopupStyleProperties::fontsAndMargins(StylePropertyBuilder& builder) {
    const auto getter = [](PopupMenu* widget) { return *static_cast<DefaultPopupMenuListStyle*>(widget->listStyle()); };
    const auto setter = [](PopupMenu* widget, const DefaultPopupMenuListStyle& style) { widget->setListStyle(new DefaultPopupMenuListStyle(style)); };
    const QString getCode = QStringLiteral("*static_cast<animatedwidgets::DefaultPopupMenuListStyle*>(widget->listStyle())");
    const QString setCode = QStringLiteral("setListStyle(new animatedwidgets::DefaultPopupMenuListStyle(style))");

    StyleObjectProperties::margins<PopupMenu, DefaultPopupMenuListStyle>(builder, QStringLiteral("List margins"), QStringLiteral("list.viewMargins"),
                                                                         &DefaultPopupMenuListStyle::viewMargins, getter, setter, getCode, setCode);
    StyleObjectProperties::font<PopupMenu, DefaultPopupMenuListStyle>(builder, QStringLiteral("Item font"), QStringLiteral("list.itemFont"),
                                                                      &DefaultPopupMenuListStyle::itemFont, getter, setter, getCode, setCode);
    StyleObjectProperties::font<PopupMenu, DefaultPopupMenuListStyle>(builder, QStringLiteral("Group title font"), QStringLiteral("list.groupTitleFont"),
                                                                      &DefaultPopupMenuListStyle::groupTitleFont, getter, setter, getCode, setCode);
}

void PopupStyleProperties::checkAlignment(StylePropertyBuilder& builder) {
    const QList<animatedwidgets::styleeditor::StyleEnumOption> options = {{QStringLiteral("Left"), Qt::AlignLeft, QStringLiteral("Qt::AlignLeft")},
                                                                          {QStringLiteral("Right"), Qt::AlignRight, QStringLiteral("Qt::AlignRight")}};
    StyleObjectProperties::choice<PopupMenu>(
            builder, QStringLiteral("List items"), QStringLiteral("list.checkedIconAlignment"), QStringLiteral("Check icon alignment"),
            &DefaultPopupMenuListStyle::checkedIconAlignment, [](PopupMenu* widget) { return *static_cast<DefaultPopupMenuListStyle*>(widget->listStyle()); },
            [](PopupMenu* widget, const DefaultPopupMenuListStyle& style) { widget->setListStyle(new DefaultPopupMenuListStyle(style)); }, options,
            QStringLiteral("*static_cast<animatedwidgets::DefaultPopupMenuListStyle*>(widget->listStyle())"),
            QStringLiteral("setListStyle(new animatedwidgets::DefaultPopupMenuListStyle(style))"));
    builder.enabledWhen(QStringLiteral("list.checkedIconAlignment"), QStringLiteral("list.checkedIconVisible"));
}
