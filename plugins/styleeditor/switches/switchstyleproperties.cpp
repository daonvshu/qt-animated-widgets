#include "switchstyleproperties.h"

#include "../utils/stylepluginproperties.h"

using animatedwidgets::styleeditor::StylePropertyBuilder;
using animatedwidgets::styleeditor::StylePropertyGroup;
using P = StylePluginProperties;

void SwitchStyleProperties::switchControl(StylePropertyBuilder& builder) {
    builder.addGroups({
        content(),
        track(),
        handle(),
        P::animation()
    });
}

StylePropertyGroup SwitchStyleProperties::content() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Content");
    group.properties = {
        P::field(QStringLiteral("enabled"), QStringLiteral("Enabled")),
        P::field(QStringLiteral("text"), QStringLiteral("Text")),
        P::field(QStringLiteral("checked"), QStringLiteral("Checked")),
        P::field(QStringLiteral("textSpacing"), QStringLiteral("Text spacing"))
    };
    return group;
}

StylePropertyGroup SwitchStyleProperties::track() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Track");
    group.properties = {
        P::field(QStringLiteral("trackSize"), QStringLiteral("Size")),
        P::field(QStringLiteral("uncheckedTrackColor"), QStringLiteral("Unchecked")),
        P::field(QStringLiteral("checkedTrackColor"), QStringLiteral("Checked")),
        P::field(QStringLiteral("disabledTrackColor"), QStringLiteral("Disabled"))
    };
    return group;
}

StylePropertyGroup SwitchStyleProperties::handle() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Handle");
    group.properties = {
        P::field(QStringLiteral("thumbMargin"), QStringLiteral("Margin")),
        P::field(QStringLiteral("handleRadius"), QStringLiteral("Radius")),
        P::icon(QStringLiteral("handleIcon"), QStringLiteral("Icon")),
        P::field(QStringLiteral("iconHint"), QStringLiteral("Tint icon")),
        P::field(QStringLiteral("uncheckedThumbColor"), QStringLiteral("Unchecked")),
        P::field(QStringLiteral("checkedThumbColor"), QStringLiteral("Checked")),
        P::field(QStringLiteral("disabledThumbColor"), QStringLiteral("Disabled"))
    };
    return group;
}
