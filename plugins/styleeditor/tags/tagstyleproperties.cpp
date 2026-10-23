#include "tagstyleproperties.h"

#include "../utils/stylepluginproperties.h"

using animatedwidgets::styleeditor::StylePropertyBuilder;
using animatedwidgets::styleeditor::StylePropertyGroup;
using P = StylePluginProperties;

void TagStyleProperties::tagWidget(StylePropertyBuilder& builder) {
    builder.addGroups({
        content(),
        colors(),
        shape(),
        closeButton()
    });
}

StylePropertyGroup TagStyleProperties::content() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Content");
    group.properties = {
        P::field(QStringLiteral("enabled"), QStringLiteral("Enabled")),
        P::field(QStringLiteral("text"), QStringLiteral("Text")),
        P::icon(QStringLiteral("icon"), QStringLiteral("Icon")),
        P::field(QStringLiteral("iconHint"), QStringLiteral("Tint icon")),
        P::field(QStringLiteral("iconSize"), QStringLiteral("Icon size")),
        P::field(QStringLiteral("iconSpacing"), QStringLiteral("Icon spacing"))
    };
    return group;
}

StylePropertyGroup TagStyleProperties::colors() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Colors");
    group.properties = {
        P::field(QStringLiteral("background"), QStringLiteral("Background")),
        P::field(QStringLiteral("border"), QStringLiteral("Border")),
        P::field(QStringLiteral("content"), QStringLiteral("Content")),
        P::field(QStringLiteral("disabledContent"), QStringLiteral("Disabled content"))
    };
    return group;
}

StylePropertyGroup TagStyleProperties::shape() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Shape");
    group.properties = {
        P::field(QStringLiteral("borderWidth"), QStringLiteral("Border width")),
        P::field(QStringLiteral("cornerRadius"), QStringLiteral("Corner radius")),
        P::field(QStringLiteral("contentPadding"), QStringLiteral("Content padding"))
    };
    return group;
}

StylePropertyGroup TagStyleProperties::closeButton() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Close button");
    group.properties = {
        P::field(QStringLiteral("closeButtonVisible"), QStringLiteral("Visible")),
        P::when(P::icon(QStringLiteral("closeIcon"), QStringLiteral("Icon")), QStringLiteral("closeButtonVisible")),
        P::when(P::field(QStringLiteral("closeIconSize"), QStringLiteral("Icon size")), QStringLiteral("closeButtonVisible")),
        P::when(P::field(QStringLiteral("closeButtonSize"), QStringLiteral("Button size")), QStringLiteral("closeButtonVisible"))
    };
    return group;
}
