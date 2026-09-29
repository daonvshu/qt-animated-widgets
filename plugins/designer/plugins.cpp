#include "plugins.h"

#include "buttons/variantbuttonplugin.h"
#include "inputs/inputplugin.h"
#include "progressbars/progressbarplugin.h"
#include "sliders/sliderplugin.h"
#include "switches/switchplugin.h"

AnimatedWidgetsDesignerPlugin::AnimatedWidgetsDesignerPlugin(QObject* parent)
    : QObject(parent)
{
    for (int kind = 0; kind < 10; ++kind) {
        m_widgets.append(new VariantButtonPlugin(kind, this));
    }
    m_widgets.append(new SwitchPlugin(this));
    m_widgets.append(new ProgressBarPlugin(this));
    m_widgets.append(new SliderPlugin(this));
    for (int kind = 0; kind < 4; ++kind) {
        m_widgets.append(new InputPlugin(kind, this));
    }
}

QList<QDesignerCustomWidgetInterface*> AnimatedWidgetsDesignerPlugin::customWidgets() const {
    return m_widgets;
}
