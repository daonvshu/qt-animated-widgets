#include "buttonstyleproperties.h"

using animatedwidgets::styleeditor::StylePropertyBuilder;
using animatedwidgets::styleeditor::StylePropertyGroup;
using animatedwidgets::styleeditor::StylePropertyOptions;

void ButtonStyleProperties::filled(StylePropertyBuilder& builder) {
    builder.addGroups({
        buttonProperties(),
        textContent(),
        background(),
        contentColors(),
        cornerRadius(),
        animation(),
        textLayout()
    });
}

void ButtonStyleProperties::outline(StylePropertyBuilder& builder) {
    builder.addGroups({
        buttonProperties(),
        textContent(),
        border(),
        contentColors(),
        cornerRadius(),
        animation(),
        textLayout()
    });
}

void ButtonStyleProperties::bordered(StylePropertyBuilder& builder) {
    builder.addGroups({
        buttonProperties(),
        textContent(),
        background(),
        border(),
        contentColors(),
        cornerRadius(),
        animation(),
        textLayout()
    });
}

void ButtonStyleProperties::hover(StylePropertyBuilder& builder) {
    builder.addGroups({
        buttonProperties(),
        textContent(),
        hoverBackground(),
        contentColors(),
        cornerRadius(),
        animation(),
        textLayout()
    });
}

void ButtonStyleProperties::filledIcon(StylePropertyBuilder& builder) {
    builder.addGroups({
        buttonProperties(),
        textContent(),
        background(),
        contentColors(true),
        cornerRadius(),
        animation(true),
        iconShape()
    });
}

void ButtonStyleProperties::outlineIcon(StylePropertyBuilder& builder) {
    builder.addGroups({
        buttonProperties(),
        textContent(),
        border(),
        contentColors(true),
        cornerRadius(),
        animation(true),
        iconShape()
    });
}

void ButtonStyleProperties::borderedIcon(StylePropertyBuilder& builder) {
    builder.addGroups({
        buttonProperties(),
        textContent(),
        background(),
        border(),
        contentColors(true),
        cornerRadius(),
        animation(true),
        iconShape()
    });
}

void ButtonStyleProperties::hoverIcon(StylePropertyBuilder& builder) {
    builder.addGroups({
        buttonProperties(),
        textContent(),
        hoverBackground(),
        contentColors(true),
        cornerRadius(),
        animation(true),
        iconShape()
    });
}

StylePropertyGroup ButtonStyleProperties::buttonProperties() {
    StylePropertyOptions enabled;
    enabled.name = QStringLiteral("enabled");
    enabled.label = QStringLiteral("Enabled");

    StylePropertyOptions checkable;
    checkable.name = QStringLiteral("checkable");
    checkable.label = QStringLiteral("Checkable");

    StylePropertyOptions checked;
    checked.name = QStringLiteral("checked");
    checked.label = QStringLiteral("Checked");
    checked.enabledWhen = {{QStringLiteral("checkable"), true}};

    StylePropertyOptions shortcut;
    shortcut.name = QStringLiteral("shortcut");
    shortcut.label = QStringLiteral("Shortcut");
    shortcut.description = QStringLiteral("Keyboard shortcut, for example Ctrl+Return.");

    StylePropertyOptions autoRepeat;
    autoRepeat.name = QStringLiteral("autoRepeat");
    autoRepeat.label = QStringLiteral("Auto repeat");

    StylePropertyOptions repeatDelay;
    repeatDelay.name = QStringLiteral("autoRepeatDelay");
    repeatDelay.label = QStringLiteral("Repeat delay");
    repeatDelay.description = QStringLiteral("Initial repeat delay in milliseconds.");
    repeatDelay.enabledWhen = {{QStringLiteral("autoRepeat"), true}};

    StylePropertyOptions repeatInterval;
    repeatInterval.name = QStringLiteral("autoRepeatInterval");
    repeatInterval.label = QStringLiteral("Repeat interval");
    repeatInterval.description = QStringLiteral("Repeat interval in milliseconds.");
    repeatInterval.enabledWhen = {{QStringLiteral("autoRepeat"), true}};

    StylePropertyOptions autoDefault;
    autoDefault.name = QStringLiteral("autoDefault");
    autoDefault.label = QStringLiteral("Auto default");

    StylePropertyOptions defaultButton;
    defaultButton.name = QStringLiteral("default");
    defaultButton.label = QStringLiteral("Default button");

    StylePropertyOptions toolTip;
    toolTip.name = QStringLiteral("toolTip");
    toolTip.label = QStringLiteral("Tooltip");

    StylePropertyGroup group;
    group.name = QStringLiteral("Button");
    group.properties = {enabled, checkable, checked, shortcut, autoRepeat, repeatDelay, repeatInterval, autoDefault, defaultButton, toolTip};
    return group;
}

StylePropertyGroup ButtonStyleProperties::textContent() {
    StylePropertyOptions text;
    text.name = QStringLiteral("text");
    text.label = QStringLiteral("Text");
    text.description = QStringLiteral("Button label text.");

    StylePropertyOptions icon;
    icon.name = QStringLiteral("icon");
    icon.label = QStringLiteral("Icon");
    icon.description = QStringLiteral("Choose an icon from project resources.");
    icon.binding = StylePropertyOptions::Binding::Icon;

    StylePropertyOptions iconSize;
    iconSize.name = QStringLiteral("iconSize");
    iconSize.label = QStringLiteral("Icon size");
    iconSize.description = QStringLiteral("Icon width and height in pixels.");

    StylePropertyOptions iconHint;
    iconHint.name = QStringLiteral("iconHint");
    iconHint.label = QStringLiteral("Tint icon");
    iconHint.description = QStringLiteral("Tint the icon with the content color. Disabled by default to preserve the original icon colors.");

    StylePropertyGroup group;
    group.name = QStringLiteral("Content");
    group.properties = {text, icon, iconSize, iconHint};
    return group;
}

StylePropertyGroup ButtonStyleProperties::background() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Background");
    for (const QString& state : {QStringLiteral("Normal"), QStringLiteral("Hover"), QStringLiteral("Pressed"),
                                QStringLiteral("Disabled"), QStringLiteral("Checked")}) {
        StylePropertyOptions color;
        color.name = QStringLiteral("background") + state;
        color.label = state;
        color.description = QStringLiteral("Background color for the %1 state.").arg(state);
        group.properties.append(color);
    }
    return group;
}

StylePropertyGroup ButtonStyleProperties::hoverBackground() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Background");
    for (const QString& state : {QStringLiteral("Hover"), QStringLiteral("Pressed"), QStringLiteral("Disabled"), QStringLiteral("Checked")}) {
        StylePropertyOptions color;
        color.name = QStringLiteral("background") + state;
        color.label = state;
        color.description = QStringLiteral("Background color for the %1 state.").arg(state);
        group.properties.append(color);
    }
    return group;
}

StylePropertyGroup ButtonStyleProperties::border() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Border");
    for (const QString& state : {QStringLiteral("Normal"), QStringLiteral("Hover"), QStringLiteral("Pressed"),
                                QStringLiteral("Disabled"), QStringLiteral("Checked")}) {
        StylePropertyOptions color;
        color.name = QStringLiteral("border") + state;
        color.label = state;
        color.description = QStringLiteral("Border color for the %1 state.").arg(state);
        group.properties.append(color);
    }

    StylePropertyOptions width;
    width.name = QStringLiteral("borderWidth");
    width.label = QStringLiteral("Width");
    width.description = QStringLiteral("Animated border width in pixels.");
    group.properties.append(width);
    return group;
}

StylePropertyGroup ButtonStyleProperties::contentColors(bool iconOnly) {
    StylePropertyGroup group;
    group.name = QStringLiteral("Content");
    for (const QString& state : {QStringLiteral("Normal"), QStringLiteral("Hover"), QStringLiteral("Pressed"),
                                QStringLiteral("Disabled"), QStringLiteral("Checked")}) {
        StylePropertyOptions color;
        color.name = QStringLiteral("content") + state;
        color.label = QStringLiteral("Color %1").arg(state);
        color.description = QStringLiteral("Text color for the %1 state; also applies to icons when iconHint is enabled.").arg(state);
        if (iconOnly)
            color.enabledWhen.append({QStringLiteral("iconHint"), true});
        if (state != QStringLiteral("Normal"))
            color.enabledWhen.append({QStringLiteral("animateContentColor"), true});
        group.properties.append(color);
    }
    return group;
}

StylePropertyGroup ButtonStyleProperties::cornerRadius() {
    StylePropertyOptions radius;
    radius.name = QStringLiteral("cornerRadius");
    radius.label = QStringLiteral("Corner radius");
    radius.description = QStringLiteral("Rounded corner radius in pixels.");

    StylePropertyGroup group;
    group.name = QStringLiteral("Shape");
    group.properties = {radius};
    return group;
}

StylePropertyGroup ButtonStyleProperties::animation(bool iconOnly) {
    StylePropertyOptions duration;
    duration.name = QStringLiteral("animationDuration");
    duration.label = QStringLiteral("Duration");
    duration.description = QStringLiteral("Color transition duration in milliseconds.");

    StylePropertyOptions darkerStep;
    darkerStep.name = QStringLiteral("darkerStep");
    darkerStep.label = QStringLiteral("Darker step");
    darkerStep.description = QStringLiteral("Fallback darkening step for undefined states.");

    StylePropertyOptions animateContent;
    animateContent.name = QStringLiteral("animateContentColor");
    animateContent.label = QStringLiteral("Animate content");
    animateContent.description = QStringLiteral("Animate content colors with the button state. Icons follow only when iconHint is enabled.");
    if (iconOnly)
        animateContent.enabledWhen = {{QStringLiteral("iconHint"), true}};

    StylePropertyGroup group;
    group.name = QStringLiteral("Animation");
    group.properties = {duration, darkerStep, animateContent};
    return group;
}

StylePropertyGroup ButtonStyleProperties::textLayout() {
    StylePropertyOptions alignment;
    alignment.name = QStringLiteral("contentAlignment");
    alignment.label = QStringLiteral("Content alignment");
    alignment.description = QStringLiteral("Horizontal and vertical alignment of the icon and text within the padded button area.");
    alignment.binding = StylePropertyOptions::Binding::Alignment;

    StylePropertyOptions horizontalPadding;
    horizontalPadding.name = QStringLiteral("contentHorizontalPadding");
    horizontalPadding.label = QStringLiteral("Horizontal padding");
    horizontalPadding.description = QStringLiteral("Padding between the content and the left and right button edges in pixels.");

    StylePropertyOptions verticalPadding;
    verticalPadding.name = QStringLiteral("contentVerticalPadding");
    verticalPadding.label = QStringLiteral("Vertical padding");
    verticalPadding.description = QStringLiteral("Padding between the content and the top and bottom button edges in pixels.");

    StylePropertyOptions iconSpacing;
    iconSpacing.name = QStringLiteral("iconSpacing");
    iconSpacing.label = QStringLiteral("Icon spacing");
    iconSpacing.description = QStringLiteral("Spacing between icon and text in pixels.");

    StylePropertyOptions iconPlacement;
    iconPlacement.name = QStringLiteral("iconPlacement");
    iconPlacement.label = QStringLiteral("Icon placement");
    iconPlacement.description = QStringLiteral("Position of the icon relative to the text.");
    iconPlacement.binding = StylePropertyOptions::Binding::Enumeration;
    iconPlacement.enumScope = QStringLiteral("animatedwidgets::AnimatedButtonBase::IconPlacement");

    StylePropertyGroup group;
    group.name = QStringLiteral("Layout");
    group.properties = {alignment, horizontalPadding, verticalPadding, iconSpacing, iconPlacement};
    return group;
}

StylePropertyGroup ButtonStyleProperties::iconShape() {
    StylePropertyOptions shape;
    shape.name = QStringLiteral("shape");
    shape.label = QStringLiteral("Shape");
    shape.description = QStringLiteral("Circle or rounded rectangle icon button shape.");
    shape.binding = StylePropertyOptions::Binding::Enumeration;
    shape.enumScope = QStringLiteral("animatedwidgets::AnimatedButtonBase::Shape");

    StylePropertyGroup group;
    group.name = QStringLiteral("Shape");
    group.properties = {shape};
    return group;
}
