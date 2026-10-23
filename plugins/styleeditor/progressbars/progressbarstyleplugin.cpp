#include "progressbarstyleplugin.h"

#include "progressbarstyleproperties.h"
#include "../utils/styleplugintypeutils.h"
#include "animatedwidgets/progressbars/progressbar.h"

using animatedwidgets::styleeditor::StyleEditorWidgetType;
using animatedwidgets::styleeditor::StyleWidgetTypeOptions;

QString ProgressBarStylePlugin::pluginId() const {
    return QStringLiteral("animatedwidgets.progressbars");
}

QString ProgressBarStylePlugin::pluginName() const {
    return QStringLiteral("Animated Widgets Progressbars");
}

QList<StyleEditorWidgetType> ProgressBarStylePlugin::widgetTypes() const {
    StyleWidgetTypeOptions progressBar;
    progressBar.id = QStringLiteral("progressbar");
    progressBar.name = QStringLiteral("Progress Bar");
    progressBar.category = QStringLiteral("Progress bars");
    progressBar.categoryOrder = 700.0;
    progressBar.order = 100.0;
    progressBar.suggestedSize = QSize(280, 28);
    progressBar.defineProperties = &ProgressBarStyleProperties::progressBar;

    return {
        StylePluginTypeUtils::create<animatedwidgets::ProgressBar>(progressBar, [](animatedwidgets::ProgressBar* widget) {
            widget->setRealValue(65.5);
        })
    };
}
