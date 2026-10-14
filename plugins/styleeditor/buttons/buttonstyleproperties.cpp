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
    addContentColors(builder);
    addCornerRadius(builder);
    addAnimation(builder);
    addIconShape(builder);
}

void ButtonStyleProperties::outlineIcon(StylePropertyBuilder& builder) {
    addIconContent(builder);
    addBorder(builder);
    addContentColors(builder);
    addCornerRadius(builder);
    addAnimation(builder);
    addIconShape(builder);
}

void ButtonStyleProperties::borderedIcon(StylePropertyBuilder& builder) {
    addIconContent(builder);
    addBackground(builder);
    addBorder(builder);
    addContentColors(builder);
    addCornerRadius(builder);
    addAnimation(builder);
    addIconShape(builder);
}

void ButtonStyleProperties::hoverIcon(StylePropertyBuilder& builder) {
    addIconContent(builder);
    addHoverBackground(builder);
    addContentColors(builder);
    addCornerRadius(builder);
    addAnimation(builder);
    addIconShape(builder);
}

void ButtonStyleProperties::addTextContent(StylePropertyBuilder& builder) {
    builder.setGroup(QStringLiteral("Content"));
    builder.add(QStringLiteral("text"), QStringLiteral("Text"), QStringLiteral("Button label text."));
}

void ButtonStyleProperties::addIconContent(StylePropertyBuilder& builder) {
    addTextContent(builder);
    builder.icon(QStringLiteral("icon"), QStringLiteral("Icon resource"), {}, {}, QStringLiteral("Qt resource path or file path for the icon."));
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

void ButtonStyleProperties::addContentColors(StylePropertyBuilder& builder) {
    builder.setGroup(QStringLiteral("Content"));
    for (const QString& state : {QStringLiteral("Normal"), QStringLiteral("Hover"), QStringLiteral("Pressed"),
                                QStringLiteral("Disabled"), QStringLiteral("Checked")}) {
        builder.add(QStringLiteral("content") + state, QStringLiteral("Color %1").arg(state),
                    QStringLiteral("Text and icon color for the %1 state.").arg(state));
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
                QStringLiteral("Animate text and icon colors with the button state."));

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
    builder.setGroup(QStringLiteral("Shape"));
    builder.enumeration(QStringLiteral("shape"), QStringLiteral("Shape"), QStringLiteral("animatedwidgets::AnimatedButtonBase::Shape"),
                        QStringLiteral("Circle or rounded rectangle icon button shape."));
}
