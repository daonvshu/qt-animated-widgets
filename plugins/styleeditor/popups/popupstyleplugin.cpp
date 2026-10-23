#include "popupstyleplugin.h"

#include "popupstyleproperties.h"
#include "../utils/styleplugintypeutils.h"
#include "animatedwidgets/popups/popupmenu.h"
#include "animatedwidgets/popups/searchpopupmenu.h"
#include "animatedwidgets/popups/treepopupmenu.h"
#include "animatedwidgets/popups/cascaderpopupmenu.h"
#include "../utils/stylepreviewdata.h"

using animatedwidgets::styleeditor::StyleEditorWidgetType;
using animatedwidgets::styleeditor::StyleWidgetTypeOptions;

QString PopupStylePlugin::pluginId() const {
    return QStringLiteral("animatedwidgets.popups");
}

QString PopupStylePlugin::pluginName() const {
    return QStringLiteral("Animated Widgets Popups");
}

QList<StyleEditorWidgetType> PopupStylePlugin::widgetTypes() const {
    StyleWidgetTypeOptions popupMenu;
    popupMenu.id = QStringLiteral("popupmenu");
    popupMenu.name = QStringLiteral("Popup Menu");
    popupMenu.category = QStringLiteral("Popups");
    popupMenu.categoryOrder = 1000.0;
    popupMenu.order = 100.0;
    popupMenu.suggestedSize = QSize(300, 240);
    popupMenu.defineProperties = &PopupStyleProperties::popupMenu;

    StyleWidgetTypeOptions searchPopupMenu;
    searchPopupMenu.id = QStringLiteral("searchpopupmenu");
    searchPopupMenu.name = QStringLiteral("Search Popup Menu");
    searchPopupMenu.category = QStringLiteral("Popups");
    searchPopupMenu.categoryOrder = 1000.0;
    searchPopupMenu.order = 200.0;
    searchPopupMenu.suggestedSize = QSize(300, 240);
    searchPopupMenu.defineProperties = &PopupStyleProperties::searchPopupMenu;

    StyleWidgetTypeOptions treePopupMenu;
    treePopupMenu.id = QStringLiteral("treepopupmenu");
    treePopupMenu.name = QStringLiteral("Tree Popup Menu");
    treePopupMenu.category = QStringLiteral("Popups");
    treePopupMenu.categoryOrder = 1000.0;
    treePopupMenu.order = 300.0;
    treePopupMenu.suggestedSize = QSize(300, 240);
    treePopupMenu.defineProperties = &PopupStyleProperties::treePopupMenu;

    StyleWidgetTypeOptions cascaderPopupMenu;
    cascaderPopupMenu.id = QStringLiteral("cascaderpopupmenu");
    cascaderPopupMenu.name = QStringLiteral("Cascader Popup Menu");
    cascaderPopupMenu.category = QStringLiteral("Popups");
    cascaderPopupMenu.categoryOrder = 1000.0;
    cascaderPopupMenu.order = 400.0;
    cascaderPopupMenu.suggestedSize = QSize(540, 240);
    cascaderPopupMenu.defineProperties = &PopupStyleProperties::cascaderPopupMenu;

    return {
        StylePluginTypeUtils::create<animatedwidgets::PopupMenu>(popupMenu, [](animatedwidgets::PopupMenu* widget) {
            widget->setStrings(StylePreviewData::choices());
            widget->setAutoClose(false);
        }),
        StylePluginTypeUtils::create<animatedwidgets::SearchPopupMenu>(searchPopupMenu, [](animatedwidgets::SearchPopupMenu* widget) {
            widget->setStrings(StylePreviewData::choices());
            widget->setAutoClose(false);
        }),
        StylePluginTypeUtils::create<animatedwidgets::TreePopupMenu>(treePopupMenu, [](animatedwidgets::TreePopupMenu* widget) {
            widget->setItems(StylePreviewData::tree(widget), QModelIndex(), 0, 0, 6, 300);
            widget->setAutoClose(false);
        }),
        StylePluginTypeUtils::create<animatedwidgets::CascaderPopupMenu>(cascaderPopupMenu, [](animatedwidgets::CascaderPopupMenu* widget) {
            widget->setItems(StylePreviewData::tree(widget), QModelIndex(), 0, 0, 6, 540);
            widget->setAutoClose(false);
        })
    };
}
