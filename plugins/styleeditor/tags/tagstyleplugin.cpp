#include "tagstyleplugin.h"

#include "tagstyleproperties.h"
#include "../utils/styleplugintypeutils.h"
#include "animatedwidgets/tags/tagwidget.h"

using animatedwidgets::styleeditor::StyleEditorWidgetType;
using animatedwidgets::styleeditor::StyleWidgetTypeOptions;

QString TagStylePlugin::pluginId() const {
    return QStringLiteral("animatedwidgets.tags");
}

QString TagStylePlugin::pluginName() const {
    return QStringLiteral("Animated Widgets Tags");
}

QList<StyleEditorWidgetType> TagStylePlugin::widgetTypes() const {
    StyleWidgetTypeOptions tagWidget;
    tagWidget.id = QStringLiteral("tagwidget");
    tagWidget.name = QStringLiteral("Tag Widget");
    tagWidget.category = QStringLiteral("Tags");
    tagWidget.categoryOrder = 800.0;
    tagWidget.order = 100.0;
    tagWidget.suggestedSize = QSize(130, 32);
    tagWidget.defineProperties = &TagStyleProperties::tagWidget;

    return {
        StylePluginTypeUtils::create<animatedwidgets::TagWidget>(tagWidget, [](animatedwidgets::TagWidget* widget) {
            widget->setText(QStringLiteral("Preview"));
        })
    };
}
