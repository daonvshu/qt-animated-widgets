#include "sliderstyleproperties.h"

#include "../utils/stylepluginproperties.h"

using animatedwidgets::styleeditor::StylePropertyBuilder;
using animatedwidgets::styleeditor::StylePropertyGroup;
using P = StylePluginProperties;

void SliderStyleProperties::slider(StylePropertyBuilder& builder) {
    builder.addGroups({
        range(),
        layout(),
        P::valueAnimation()
    });
}

StylePropertyGroup SliderStyleProperties::range() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Slider");
    group.properties = {
        P::field(QStringLiteral("enabled"), QStringLiteral("Enabled")),
        P::field(QStringLiteral("minimum"), QStringLiteral("Minimum")),
        P::field(QStringLiteral("maximum"), QStringLiteral("Maximum")),
        P::field(QStringLiteral("value"), QStringLiteral("Value")),
        P::field(QStringLiteral("singleStep"), QStringLiteral("Single step")),
        P::field(QStringLiteral("pageStep"), QStringLiteral("Page step")),
        P::field(QStringLiteral("tracking"), QStringLiteral("Tracking"))
    };
    return group;
}

StylePropertyGroup SliderStyleProperties::layout() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Appearance");
    group.properties = {
        P::enumeration(QStringLiteral("orientation"), QStringLiteral("Orientation"), QStringLiteral("Qt")),
        P::enumeration(QStringLiteral("tickPosition"), QStringLiteral("Tick position"), QStringLiteral("QSlider")),
        P::field(QStringLiteral("tickInterval"), QStringLiteral("Tick interval")),
        P::field(QStringLiteral("invertedAppearance"), QStringLiteral("Inverted appearance")),
        P::field(QStringLiteral("invertedControls"), QStringLiteral("Inverted controls")),
        P::field(QStringLiteral("styleSheet"), QStringLiteral("Stylesheet"))
    };
    return group;
}
