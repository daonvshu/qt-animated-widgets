#include "sliderstyleplugin.h"

#include "sliderstyleproperties.h"
#include "../utils/styleplugintypeutils.h"
#include "animatedwidgets/sliders/slider.h"

using animatedwidgets::styleeditor::StyleEditorWidgetType;
using animatedwidgets::styleeditor::StyleWidgetTypeOptions;

QString SliderStylePlugin::pluginId() const {
    return QStringLiteral("animatedwidgets.sliders");
}

QString SliderStylePlugin::pluginName() const {
    return QStringLiteral("Animated Widgets Sliders");
}

QList<StyleEditorWidgetType> SliderStylePlugin::widgetTypes() const {
    StyleWidgetTypeOptions slider;
    slider.id = QStringLiteral("slider");
    slider.name = QStringLiteral("Slider");
    slider.category = QStringLiteral("Sliders");
    slider.categoryOrder = 600.0;
    slider.order = 100.0;
    slider.suggestedSize = QSize(280, 36);
    slider.defineProperties = &SliderStyleProperties::slider;

    return {
        StylePluginTypeUtils::create<animatedwidgets::Slider>(slider, [](animatedwidgets::Slider* widget) {
            widget->setOrientation(Qt::Horizontal);
            widget->setValue(40);
        })
    };
}
