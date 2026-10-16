#include "buttonstyleproperties.h"

using animatedwidgets::styleeditor::StylePropertyBuilder;

void ButtonStyleProperties::filled(StylePropertyBuilder& builder) {
    addTextContent(builder);
    addBackground(builder);
    addContentColors(builder);
    addCornerRadius(builder);
    addAnimation(builder);
    addTextLayout(builder);
}

void ButtonStyleProperties::outline(StylePropertyBuilder& builder) {
    addTextContent(builder);
    addBorder(builder);
    addContentColors(builder);
    addCornerRadius(builder);
    addAnimation(builder);
    addTextLayout(builder);
}

void ButtonStyleProperties::bordered(StylePropertyBuilder& builder) {
    addTextContent(builder);
    addBackground(builder);
    addBorder(builder);
    addContentColors(builder);
    addCornerRadius(builder);
    addAnimation(builder);
    addTextLayout(builder);
}

void ButtonStyleProperties::hover(StylePropertyBuilder& builder) {
    addTextContent(builder);
    addHoverBackground(builder);
    addContentColors(builder);
    addCornerRadius(builder);
    addAnimation(builder);
    addTextLayout(builder);
}

void ButtonStyleProperties::filledIcon(StylePropertyBuilder& builder) {
    addIconContent(builder);
    addBackground(builder);
    addContentColors(builder, true);
    addCornerRadius(builder);
    addAnimation(builder);
    addIconShape(builder);
}

void ButtonStyleProperties::outlineIcon(StylePropertyBuilder& builder) {
    addIconContent(builder);
    addBorder(builder);
    addContentColors(builder, true);
    addCornerRadius(builder);
    addAnimation(builder);
    addIconShape(builder);
}

void ButtonStyleProperties::borderedIcon(StylePropertyBuilder& builder) {
    addIconContent(builder);
    addBackground(builder);
    addBorder(builder);
    addContentColors(builder, true);
    addCornerRadius(builder);
    addAnimation(builder);
    addIconShape(builder);
}

void ButtonStyleProperties::hoverIcon(StylePropertyBuilder& builder) {
    addIconContent(builder);
    addHoverBackground(builder);
    addContentColors(builder, true);
    addCornerRadius(builder);
    addAnimation(builder);
    addIconShape(builder);
}

void ButtonStyleProperties::addTextContent(StylePropertyBuilder& builder) {
    addButtonProperties(builder);
    builder.setGroup(QStringLiteral("Content"));
    builder.add(QStringLiteral("text"), QStringLiteral("Text"), QStringLiteral("Button label text."));
    builder.icon(QStringLiteral("icon"), QStringLiteral("Icon"), {}, {}, QStringLiteral("Choose an icon from project resources."));
    builder.add(QStringLiteral("iconSize"), QStringLiteral("Icon size"), QStringLiteral("Icon width and height in pixels."));
    builder.add(QStringLiteral("iconHint"), QStringLiteral("Tint icon"),
                QStringLiteral("Tint the icon with the content color. Disabled by default to preserve the original icon colors."));
}

void ButtonStyleProperties::addIconContent(StylePropertyBuilder& builder) {
    addTextContent(builder);
}

void ButtonStyleProperties::addButtonProperties(StylePropertyBuilder& builder) {
    builder.setGroup(QStringLiteral("Button"));
    builder.add(QStringLiteral("enabled"), QStringLiteral("Enabled"));
    builder.add(QStringLiteral("checkable"), QStringLiteral("Checkable"));
    builder.add(QStringLiteral("checked"), QStringLiteral("Checked"));
    builder.enabledWhen(QStringLiteral("checked"), QStringLiteral("checkable"));
    builder.add(QStringLiteral("shortcut"), QStringLiteral("Shortcut"), QStringLiteral("Keyboard shortcut, for example Ctrl+Return."));
    builder.add(QStringLiteral("autoRepeat"), QStringLiteral("Auto repeat"));
    builder.add(QStringLiteral("autoRepeatDelay"), QStringLiteral("Repeat delay"), QStringLiteral("Initial repeat delay in milliseconds."));
    builder.add(QStringLiteral("autoRepeatInterval"), QStringLiteral("Repeat interval"), QStringLiteral("Repeat interval in milliseconds."));
    builder.enabledWhen(QStringLiteral("autoRepeatDelay"), QStringLiteral("autoRepeat"));
    builder.enabledWhen(QStringLiteral("autoRepeatInterval"), QStringLiteral("autoRepeat"));
    builder.add(QStringLiteral("autoDefault"), QStringLiteral("Auto default"));
    builder.add(QStringLiteral("default"), QStringLiteral("Default button"));
    builder.add(QStringLiteral("toolTip"), QStringLiteral("Tooltip"));
}

void ButtonStyleProperties::addBackground(StylePropertyBuilder& builder) {
    builder.setGroup(QStringLiteral("Background"));
    for (const QString& state : {QStringLiteral("Normal"), QStringLiteral("Hover"), QStringLiteral("Pressed"),
                                QStringLiteral("Disabled"), QStringLiteral("Checked")}) {
        builder.add(QStringLiteral("background") + state, state, QStringLiteral("Background color for the %1 state.").arg(state));
    }
}

void ButtonStyleProperties::addBorder(StylePropertyBuilder& builder) {
    builder.setGroup(QStringLiteral("Border"));
    for (const QString& state : {QStringLiteral("Normal"), QStringLiteral("Hover"), QStringLiteral("Pressed"),
                                QStringLiteral("Disabled"), QStringLiteral("Checked")}) {
        builder.add(QStringLiteral("border") + state, state, QStringLiteral("Border color for the %1 state.").arg(state));
    }
    builder.add(QStringLiteral("borderWidth"), QStringLiteral("Width"), QStringLiteral("Animated border width in pixels."));
}

void ButtonStyleProperties::addHoverBackground(StylePropertyBuilder& builder) {
    builder.setGroup(QStringLiteral("Background"));
    for (const QString& state : {QStringLiteral("Hover"), QStringLiteral("Pressed"), QStringLiteral("Disabled"), QStringLiteral("Checked")}) {
        builder.add(QStringLiteral("background") + state, state, QStringLiteral("Background color for the %1 state.").arg(state));
    }
}

void ButtonStyleProperties::addContentColors(StylePropertyBuilder& builder, bool iconOnly) {
    builder.setGroup(QStringLiteral("Content"));
    for (const QString& state : {QStringLiteral("Normal"), QStringLiteral("Hover"), QStringLiteral("Pressed"),
                                QStringLiteral("Disabled"), QStringLiteral("Checked")}) {
        builder.add(QStringLiteral("content") + state, QStringLiteral("Color %1").arg(state),
                    QStringLiteral("Text color for the %1 state; also applies to icons when iconHint is enabled.").arg(state));
        if (iconOnly)
            builder.enabledWhen(QStringLiteral("content") + state, QStringLiteral("iconHint"));
    }
}

void ButtonStyleProperties::addCornerRadius(StylePropertyBuilder& builder) {
    builder.setGroup(QStringLiteral("Shape"));
    builder.add(QStringLiteral("cornerRadius"), QStringLiteral("Corner radius"), QStringLiteral("Rounded corner radius in pixels."));
}

void ButtonStyleProperties::addAnimation(StylePropertyBuilder& builder) {
    builder.setGroup(QStringLiteral("Animation"));
    builder.add(QStringLiteral("animationDuration"), QStringLiteral("Duration"), QStringLiteral("Color transition duration in milliseconds."));
    builder.add(QStringLiteral("darkerStep"), QStringLiteral("Darker step"), QStringLiteral("Fallback darkening step for undefined states."));
    builder.add(QStringLiteral("animateContentColor"), QStringLiteral("Animate content"),
                QStringLiteral("Animate content colors with the button state. Icons follow only when iconHint is enabled."));

    for (const QString& state : {QStringLiteral("Hover"), QStringLiteral("Pressed"), QStringLiteral("Disabled"), QStringLiteral("Checked")})
        builder.enabledWhen(QStringLiteral("content") + state, QStringLiteral("animateContentColor"));
}

void ButtonStyleProperties::addTextLayout(StylePropertyBuilder& builder) {
    builder.setGroup(QStringLiteral("Layout"));
    builder.alignment(QStringLiteral("contentAlignment"), QStringLiteral("Content alignment"),
                      QStringLiteral("Horizontal and vertical alignment of the icon and text within the padded button area."));

    builder.add(QStringLiteral("contentHorizontalPadding"), QStringLiteral("Horizontal padding"),
                QStringLiteral("Padding between the content and the left and right button edges in pixels."));
    builder.add(QStringLiteral("contentVerticalPadding"), QStringLiteral("Vertical padding"),
                QStringLiteral("Padding between the content and the top and bottom button edges in pixels."));
    builder.add(QStringLiteral("iconSpacing"), QStringLiteral("Icon spacing"), QStringLiteral("Spacing between icon and text in pixels."));
    builder.enumeration(QStringLiteral("iconPlacement"), QStringLiteral("Icon placement"), QStringLiteral("animatedwidgets::AnimatedButtonBase::IconPlacement"),
                        QStringLiteral("Position of the icon relative to the text."));
}

void ButtonStyleProperties::addIconShape(StylePropertyBuilder& builder) {
    builder.enabledWhen(QStringLiteral("animateContentColor"), QStringLiteral("iconHint"));
    builder.setGroup(QStringLiteral("Shape"));
    builder.enumeration(QStringLiteral("shape"), QStringLiteral("Shape"), QStringLiteral("animatedwidgets::AnimatedButtonBase::Shape"),
                        QStringLiteral("Circle or rounded rectangle icon button shape."));
}
