#include "inputstyleproperties.h"

#include "../utils/stylepluginproperties.h"
#include "animatedwidgets/inputs/compositedoublespinbox.h"
#include "animatedwidgets/inputs/compositelineedit.h"
#include "animatedwidgets/inputs/compositespinbox.h"
#include "animatedwidgets/inputs/doublespinbox.h"
#include "animatedwidgets/inputs/iconlineedit.h"
#include "animatedwidgets/inputs/spinbox.h"

using animatedwidgets::styleeditor::StylePropertyBuilder;
using animatedwidgets::styleeditor::StylePropertyGroup;
using P = StylePluginProperties;

void InputStyleProperties::lineEdit(StylePropertyBuilder& builder) {
    builder.addGroups({
        interaction(),
        P::inputBackground(),
        P::inputBorder(),
        P::inputShape(),
        P::animation(),
        lineContent(),
        P::textColors()
    });
}

void InputStyleProperties::iconLineEdit(StylePropertyBuilder& builder) {
    builder.addGroups({
        interaction(),
        P::inputBackground(),
        P::inputBorder(),
        P::inputShape(),
        P::animation(),
        lineContent(),
        P::textColors(),
        iconLayout(),
        P::iconButtonColors(QStringLiteral("leadingIcon"), QStringLiteral("Leading icon colors")),
        P::iconButtonColors(QStringLiteral("trailingIcon"), QStringLiteral("Trailing icon colors"))
    });
}

void InputStyleProperties::passwordLineEdit(StylePropertyBuilder& builder) {
    builder.addGroups({
        interaction(),
        P::inputBackground(),
        P::inputBorder(),
        P::inputShape(),
        P::animation(),
        lineContent(),
        P::textColors(),
        password(),
        passwordIconLayout(),
        P::iconButtonColors(QStringLiteral("leadingIcon"), QStringLiteral("Leading icon colors")),
        P::iconButtonColors(QStringLiteral("trailingIcon"), QStringLiteral("Trailing icon colors"))
    });
}

void InputStyleProperties::searchLineEdit(StylePropertyBuilder& builder) {
    builder.addGroups({
        interaction(),
        P::inputBackground(),
        P::inputBorder(),
        P::inputShape(),
        P::animation(),
        lineContent(),
        P::textColors(),
        suggestionPopup()
    });
}

void InputStyleProperties::plainTextEdit(StylePropertyBuilder& builder) {
    builder.addGroups({
        interaction(),
        P::inputBackground(),
        P::inputBorder(),
        P::inputShape(),
        P::animation(),
        multilineContent(),
        P::textColors()
    });
}

void InputStyleProperties::spinBox(StylePropertyBuilder& builder) {
    builder.addGroups({
        interaction(),
        P::inputBackground(),
        P::inputBorder(),
        P::inputShape(),
        P::animation(),
        numberContent(),
        P::textColors(),
        stepButtons(),
        P::iconButtonColors(QStringLiteral("stepButton"), QStringLiteral("Step button colors"))
    });
}

void InputStyleProperties::doubleSpinBox(StylePropertyBuilder& builder) {
    builder.addGroups({
        interaction(),
        P::inputBackground(),
        P::inputBorder(),
        P::inputShape(),
        P::animation(),
        numberContent(),
        P::textColors(),
        stepButtons(),
        P::iconButtonColors(QStringLiteral("stepButton"), QStringLiteral("Step button colors")),
        decimals()
    });
}

void InputStyleProperties::compositeLineEdit(StylePropertyBuilder& builder) {
    builder.addGroups({
        interaction(),
        P::inputBackground(),
        P::inputBorder(),
        P::inputShape(),
        P::animation(),
        P::compositeLayout(),
        compositeLineContent()
    });

    auto* widget = qobject_cast<CompositeLineEdit*>(builder.object());
    auto* defaults = qobject_cast<CompositeLineEdit*>(builder.defaults());
    if (widget && defaults) {
        P::editorProperties(builder, widget->editor(), defaults->editor(),
                            {iconLayout(), P::textColors(), P::iconButtonColors(QStringLiteral("leadingIcon"), QStringLiteral("Leading icon colors")),
                             P::iconButtonColors(QStringLiteral("trailingIcon"), QStringLiteral("Trailing icon colors"))});
    }
}

void InputStyleProperties::compositePlainTextEdit(StylePropertyBuilder& builder) {
    builder.addGroups({
        interaction(),
        P::inputBackground(),
        P::inputBorder(),
        P::inputShape(),
        P::animation(),
        P::compositeLayout(),
        multilineContent(),
        P::textColors()
    });
}

void InputStyleProperties::compositeSpinBox(StylePropertyBuilder& builder) {
    builder.addGroups({
        interaction(),
        P::inputBackground(),
        P::inputBorder(),
        P::inputShape(),
        P::animation(),
        P::compositeLayout(),
        compositeNumberContent()
    });

    auto* widget = qobject_cast<CompositeSpinBox*>(builder.object());
    auto* defaults = qobject_cast<CompositeSpinBox*>(builder.defaults());
    if (widget && defaults) {
        P::editorProperties(builder, widget->editor(), defaults->editor(),
                            {stepButtons(), P::textColors(), P::iconButtonColors(QStringLiteral("stepButton"), QStringLiteral("Step button colors"))});
    }
}

void InputStyleProperties::compositeDoubleSpinBox(StylePropertyBuilder& builder) {
    builder.addGroups({
        interaction(),
        P::inputBackground(),
        P::inputBorder(),
        P::inputShape(),
        P::animation(),
        P::compositeLayout(),
        compositeNumberContent(),
        decimals()
    });

    auto* widget = qobject_cast<CompositeDoubleSpinBox*>(builder.object());
    auto* defaults = qobject_cast<CompositeDoubleSpinBox*>(builder.defaults());
    if (widget && defaults) {
        P::editorProperties(builder, widget->editor(), defaults->editor(),
                            {stepButtons(), P::textColors(), P::iconButtonColors(QStringLiteral("stepButton"), QStringLiteral("Step button colors"))});
    }
}

StylePropertyGroup InputStyleProperties::interaction() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Input");
    group.properties = {
        P::field(QStringLiteral("enabled"), QStringLiteral("Enabled")),
        P::field(QStringLiteral("readOnly"), QStringLiteral("Read only")),
        P::field(QStringLiteral("hasError"), QStringLiteral("Error state"))
    };
    return group;
}

StylePropertyGroup InputStyleProperties::lineContent() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Content");
    group.properties = {
        P::field(QStringLiteral("text"), QStringLiteral("Text")),
        P::field(QStringLiteral("placeholderText"), QStringLiteral("Placeholder")),
        P::field(QStringLiteral("maxLength"), QStringLiteral("Maximum length")),
        P::field(QStringLiteral("clearButtonEnabled"), QStringLiteral("Clear button")),
        P::alignment(QStringLiteral("alignment"), QStringLiteral("Alignment"))
    };
    return group;
}

StylePropertyGroup InputStyleProperties::compositeLineContent() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Content");
    group.properties = {
        P::field(QStringLiteral("text"), QStringLiteral("Text")),
        P::field(QStringLiteral("placeholderText"), QStringLiteral("Placeholder"))
    };
    return group;
}

StylePropertyGroup InputStyleProperties::multilineContent() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Content");
    group.properties = {
        P::field(QStringLiteral("plainText"), QStringLiteral("Text")),
        P::field(QStringLiteral("placeholderText"), QStringLiteral("Placeholder"))
    };
    return group;
}

StylePropertyGroup InputStyleProperties::numberContent() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Number");
    group.properties = {
        P::field(QStringLiteral("minimum"), QStringLiteral("Minimum")),
        P::field(QStringLiteral("maximum"), QStringLiteral("Maximum")),
        P::field(QStringLiteral("singleStep"), QStringLiteral("Single step")),
        P::field(QStringLiteral("value"), QStringLiteral("Value")),
        P::field(QStringLiteral("prefix"), QStringLiteral("Prefix")),
        P::field(QStringLiteral("suffix"), QStringLiteral("Suffix")),
        P::field(QStringLiteral("wrapping"), QStringLiteral("Wrapping")),
        P::alignment(QStringLiteral("alignment"), QStringLiteral("Alignment"))
    };
    return group;
}

StylePropertyGroup InputStyleProperties::compositeNumberContent() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Number");
    group.properties = {
        P::field(QStringLiteral("minimum"), QStringLiteral("Minimum")),
        P::field(QStringLiteral("maximum"), QStringLiteral("Maximum")),
        P::field(QStringLiteral("singleStep"), QStringLiteral("Single step")),
        P::field(QStringLiteral("value"), QStringLiteral("Value")),
        P::field(QStringLiteral("prefix"), QStringLiteral("Prefix")),
        P::field(QStringLiteral("suffix"), QStringLiteral("Suffix")),
        P::field(QStringLiteral("stepButtonsVisible"), QStringLiteral("Step buttons visible"))
    };
    return group;
}

StylePropertyGroup InputStyleProperties::decimals() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Number");
    group.properties = {
        P::field(QStringLiteral("decimals"), QStringLiteral("Decimals"))
    };
    return group;
}

StylePropertyGroup InputStyleProperties::iconLayout() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Icons");
    group.properties = {
        P::icon(QStringLiteral("leadingIcon"), QStringLiteral("Leading icon")),
        P::field(QStringLiteral("leadingIconVisible"), QStringLiteral("Leading visible")),
        P::field(QStringLiteral("leadingIconSize"), QStringLiteral("Leading size")),
        P::icon(QStringLiteral("trailingIcon"), QStringLiteral("Trailing icon")),
        P::field(QStringLiteral("trailingIconVisible"), QStringLiteral("Trailing visible")),
        P::field(QStringLiteral("trailingIconSize"), QStringLiteral("Trailing size")),
        P::field(QStringLiteral("iconSpacing"), QStringLiteral("Icon spacing")),
        P::field(QStringLiteral("iconHint"), QStringLiteral("Tint icons"))
    };
    return group;
}

StylePropertyGroup InputStyleProperties::passwordIconLayout() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Icons");
    group.properties = {
        P::icon(QStringLiteral("leadingIcon"), QStringLiteral("Leading icon")),
        P::field(QStringLiteral("leadingIconVisible"), QStringLiteral("Leading visible")),
        P::field(QStringLiteral("leadingIconSize"), QStringLiteral("Leading size")),
        P::field(QStringLiteral("trailingIconVisible"), QStringLiteral("Password toggle visible")),
        P::field(QStringLiteral("trailingIconSize"), QStringLiteral("Password toggle size")),
        P::field(QStringLiteral("iconSpacing"), QStringLiteral("Icon spacing")),
        P::field(QStringLiteral("iconHint"), QStringLiteral("Tint icons"))
    };
    return group;
}

StylePropertyGroup InputStyleProperties::suggestionPopup() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Suggestion popup");
    group.properties = {
        P::field(QStringLiteral("popupBackground"), QStringLiteral("Background")),
        P::field(QStringLiteral("popupBorder"), QStringLiteral("Border")),
        P::field(QStringLiteral("itemText"), QStringLiteral("Item text")),
        P::field(QStringLiteral("itemHoverBackground"), QStringLiteral("Item hover")),
        P::field(QStringLiteral("itemSelectedBackground"), QStringLiteral("Item selected")),
        P::field(QStringLiteral("highlightEnabled"), QStringLiteral("Highlight enabled")),
        P::field(QStringLiteral("highlightColor"), QStringLiteral("Highlight color")),
        P::field(QStringLiteral("highlightBold"), QStringLiteral("Highlight bold"))
    };
    return group;
}

StylePropertyGroup InputStyleProperties::password() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Password");
    group.properties = {
        P::field(QStringLiteral("passwordVisible"), QStringLiteral("Visible")),
        P::icon(QStringLiteral("showPasswordIcon"), QStringLiteral("Show icon"), QStringLiteral(":/animatedwidgets/inputs/eyeopen.svg")),
        P::icon(QStringLiteral("hidePasswordIcon"), QStringLiteral("Hide icon"), QStringLiteral(":/animatedwidgets/inputs/eyeclosed.svg")),
        P::field(QStringLiteral("autoHideEnabled"), QStringLiteral("Auto hide")),
        P::when(P::field(QStringLiteral("autoHideDuration"), QStringLiteral("Auto hide delay (ms)")), QStringLiteral("autoHideEnabled"))
    };
    return group;
}

StylePropertyGroup InputStyleProperties::stepButtons() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Step buttons");
    group.properties = {
        P::field(QStringLiteral("stepButtonsVisible"), QStringLiteral("Visible")),
        P::when(P::field(QStringLiteral("stepButtonSize"), QStringLiteral("Button size")), QStringLiteral("stepButtonsVisible")),
        P::when(P::field(QStringLiteral("stepButtonSpacing"), QStringLiteral("Spacing")), QStringLiteral("stepButtonsVisible")),
        P::when(P::field(QStringLiteral("stepButtonRightMargin"), QStringLiteral("Right margin")), QStringLiteral("stepButtonsVisible")),
        P::when(P::field(QStringLiteral("stepIconSize"), QStringLiteral("Icon size")), QStringLiteral("stepButtonsVisible")),
        P::when(P::icon(QStringLiteral("stepUpIcon"), QStringLiteral("Up icon"), QStringLiteral(":/animatedwidgets/inputs/stepup.svg")), QStringLiteral("stepButtonsVisible")),
        P::when(P::icon(QStringLiteral("stepDownIcon"), QStringLiteral("Down icon"), QStringLiteral(":/animatedwidgets/inputs/stepdown.svg")), QStringLiteral("stepButtonsVisible")),
        P::field(QStringLiteral("iconHint"), QStringLiteral("Tint icons"))
    };
    return group;
}
