#include "progressbarstyleproperties.h"

#include "../utils/stylepluginproperties.h"

using animatedwidgets::styleeditor::StylePropertyBuilder;
using animatedwidgets::styleeditor::StylePropertyGroup;
using P = StylePluginProperties;

void ProgressBarStyleProperties::progressBar(StylePropertyBuilder& builder) {
    builder.addGroups({
        content(),
        appearance(),
        P::valueAnimation()
    });
}

StylePropertyGroup ProgressBarStyleProperties::content() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Progress");
    group.properties = {
        P::field(QStringLiteral("enabled"), QStringLiteral("Enabled")),
        P::field(QStringLiteral("minimum"), QStringLiteral("Minimum")),
        P::field(QStringLiteral("maximum"), QStringLiteral("Maximum")),
        P::field(QStringLiteral("realValue"), QStringLiteral("Value")),
        P::field(QStringLiteral("valueDecimals"), QStringLiteral("Decimals")),
        P::field(QStringLiteral("format"), QStringLiteral("Text format")),
        P::field(QStringLiteral("textVisible"), QStringLiteral("Text visible")),
        P::horizontalAlignment(QStringLiteral("alignment"), QStringLiteral("Text alignment"), true),
        P::enumeration(QStringLiteral("orientation"), QStringLiteral("Orientation"), QStringLiteral("Qt")),
        P::field(QStringLiteral("invertedAppearance"), QStringLiteral("Inverted appearance"))
    };
    return group;
}

StylePropertyGroup ProgressBarStyleProperties::appearance() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Appearance");
    group.properties = {
        P::field(QStringLiteral("trackColor"), QStringLiteral("Track")),
        P::field(QStringLiteral("progressColor"), QStringLiteral("Progress")),
        P::field(QStringLiteral("border"), QStringLiteral("Border")),
        P::field(QStringLiteral("textColor"), QStringLiteral("Text")),
        P::field(QStringLiteral("borderWidth"), QStringLiteral("Border width")),
        P::field(QStringLiteral("cornerRadius"), QStringLiteral("Corner radius"))
    };
    return group;
}
