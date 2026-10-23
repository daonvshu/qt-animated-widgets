#include "selectionstyleproperties.h"

#include "../utils/stylepluginproperties.h"
#include "animatedwidgets/buttons/checkbox.h"

using animatedwidgets::styleeditor::StylePropertyBuilder;
using animatedwidgets::styleeditor::StylePropertyGroup;
using P = StylePluginProperties;

void SelectionStyleProperties::checkBox(StylePropertyBuilder& builder) {
    builder.addGroups({
        selection(),
        checkOptions(),
        checkIcons(),
        iconColors(),
        partialColor(),
        P::animation()
    });
    checkState(builder);
}

void SelectionStyleProperties::radioButton(StylePropertyBuilder& builder) {
    builder.addGroups({
        selection(),
        radioOptions(),
        radioIcons(),
        iconColors(),
        P::animation()
    });
}

StylePropertyGroup SelectionStyleProperties::selection() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Selection");
    group.properties = {
        P::field(QStringLiteral("enabled"), QStringLiteral("Enabled")),
        P::field(QStringLiteral("text"), QStringLiteral("Text")),
        P::field(QStringLiteral("iconHint"), QStringLiteral("Tint icon")),
        P::field(QStringLiteral("iconSize"), QStringLiteral("Icon size")),
        P::field(QStringLiteral("iconSpacing"), QStringLiteral("Icon spacing"))
    };
    return group;
}

StylePropertyGroup SelectionStyleProperties::checkOptions() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Check box");
    group.properties = {
        P::field(QStringLiteral("tristate"), QStringLiteral("Three states"))
    };
    return group;
}

StylePropertyGroup SelectionStyleProperties::radioOptions() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Radio button");
    group.properties = {
        P::field(QStringLiteral("autoExclusive"), QStringLiteral("Auto exclusive")),
        P::field(QStringLiteral("checked"), QStringLiteral("Checked"))
    };
    return group;
}

void SelectionStyleProperties::checkState(StylePropertyBuilder& builder) {
    using animatedwidgets::CheckBox;
    auto* widget = qobject_cast<CheckBox*>(builder.object());
    auto* defaults = qobject_cast<CheckBox*>(builder.defaults());
    if (!widget || !defaults)
        return;

    animatedwidgets::styleeditor::StyleProperty property;
    property.name = QStringLiteral("checkState");
    property.group = QStringLiteral("Check box");
    property.label = QStringLiteral("Check state");
    property.type = animatedwidgets::styleeditor::StylePropertyType::Enum;
    property.enumNames = QStringList{QStringLiteral("Unchecked"), QStringLiteral("PartiallyChecked"), QStringLiteral("Checked")};
    property.enumExpressions = QStringList{QStringLiteral("Qt::Unchecked"), QStringLiteral("Qt::PartiallyChecked"), QStringLiteral("Qt::Checked")};
    property.value = property.enumNames.at(int(widget->checkState()));
    property.defaultValue = property.enumNames.at(int(defaults->checkState()));
    property.apply = [target = QPointer<CheckBox>(widget), names = property.enumNames](const QVariant& value) {
        const int index = names.indexOf(value.toString());
        if (target && index >= 0)
            target->setCheckState(static_cast<Qt::CheckState>(index));
    };
    property.codeExpression = [](const QVariant& value) { return QStringLiteral("Qt::") + value.toString(); };
    builder.add(property);
}

StylePropertyGroup SelectionStyleProperties::checkIcons() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Icons");
    group.properties = {
        P::icon(QStringLiteral("checkedIcon"), QStringLiteral("Checked"), QStringLiteral(":/animatedwidgets/styleeditor/selections/checkboxchecked.svg")),
        P::icon(QStringLiteral("uncheckedIcon"), QStringLiteral("Unchecked"), QStringLiteral(":/animatedwidgets/styleeditor/selections/checkboxunchecked.svg")),
        P::when(P::icon(QStringLiteral("partiallyCheckedIcon"), QStringLiteral("Partially checked"), QStringLiteral(":/animatedwidgets/styleeditor/selections/checkboxpartial.svg")), QStringLiteral("tristate"))
    };
    return group;
}

StylePropertyGroup SelectionStyleProperties::radioIcons() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Icons");
    group.properties = {
        P::icon(QStringLiteral("checkedIcon"), QStringLiteral("Checked"), QStringLiteral(":/animatedwidgets/styleeditor/selections/radiochecked.svg")),
        P::icon(QStringLiteral("uncheckedIcon"), QStringLiteral("Unchecked"), QStringLiteral(":/animatedwidgets/styleeditor/selections/radiounchecked.svg"))
    };
    return group;
}

StylePropertyGroup SelectionStyleProperties::iconColors() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Icon colors");
    group.properties = {
        P::when(P::field(QStringLiteral("checkedIconColor"), QStringLiteral("Checked")), QStringLiteral("iconHint")),
        P::when(P::field(QStringLiteral("uncheckedIconColor"), QStringLiteral("Unchecked")), QStringLiteral("iconHint"))
    };
    return group;
}

StylePropertyGroup SelectionStyleProperties::partialColor() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Icon colors");
    group.properties = {
        P::when(P::when(P::field(QStringLiteral("partiallyCheckedIconColor"), QStringLiteral("Partially checked")), QStringLiteral("iconHint")), QStringLiteral("tristate"))
    };
    return group;
}
