#include "switchstyleplugin.h"

#include "switchstyleproperties.h"
#include "../utils/styleplugintypeutils.h"
#include "animatedwidgets/switches/switch.h"

using animatedwidgets::styleeditor::StyleEditorWidgetType;
using animatedwidgets::styleeditor::StyleWidgetTypeOptions;

QString SwitchStylePlugin::pluginId() const {
    return QStringLiteral("animatedwidgets.switches");
}

QString SwitchStylePlugin::pluginName() const {
    return QStringLiteral("Animated Widgets Switches");
}

QList<StyleEditorWidgetType> SwitchStylePlugin::widgetTypes() const {
    StyleWidgetTypeOptions switchOptions;
    switchOptions.id = QStringLiteral("switch");
    switchOptions.name = QStringLiteral("Switch");
    switchOptions.category = QStringLiteral("Switches");
    switchOptions.categoryOrder = 300.0;
    switchOptions.order = 100.0;
    switchOptions.suggestedSize = QSize(150, 36);
    switchOptions.defineProperties = &SwitchStyleProperties::switchControl;

    return {
        StylePluginTypeUtils::create<animatedwidgets::Switch>(switchOptions, [](animatedwidgets::Switch* widget) {
            widget->setText(QStringLiteral("Preview"));
        })
    };
}
