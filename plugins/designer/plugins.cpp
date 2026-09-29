#include "plugins.h"

#include "buttons/variantbuttonplugin.h"
#include "switches/switchplugin.h"

AnimatedWidgetsDesignerPlugin::AnimatedWidgetsDesignerPlugin(QObject* parent)
    : QObject(parent)
{
    for (int kind = 0; kind < 10; ++kind) {
        m_widgets.append(new VariantButtonPlugin(kind, this));
    }
    m_widgets.append(new SwitchPlugin(this));
}

QList<QDesignerCustomWidgetInterface*> AnimatedWidgetsDesignerPlugin::customWidgets() const
{
    return m_widgets;
}
