#include "animatedwidgets/buttons/hoverbutton.h"

#include "animatedwidgets/utils/buttonpainter.h"
#include "animatedwidgets/utils/buttonstatecolors.h"

#include <QPainter>

namespace animatedwidgets {

HoverButton::HoverButton(QWidget* parent)
    : AnimatedButtonBase(parent)
{
    refreshButtonState(true);
}

ButtonStateStyle HoverButton::normalStyle() const {
    return {{}, {}, m_normalContent};
}

void HoverButton::setNormalStyle(const ButtonStateStyle& style) {
    m_normalContent = style.content();
    refreshButtonState();
}

ButtonStateStyle HoverButton::hoverStyle() const {
    return {m_hoverBackground, {}, m_hoverContent};
}

void HoverButton::setHoverStyle(const ButtonStateStyle& style) {
    m_hoverBackground = style.background();
    m_hoverContent = style.content();
    refreshButtonState();
}

ButtonStateStyle HoverButton::pressedStyle() const {
    return {m_pressedBackground, {}, m_pressedContent};
}

void HoverButton::setPressedStyle(const ButtonStateStyle& style) {
    m_pressedBackground = style.background();
    m_pressedContent = style.content();
    refreshButtonState();
}

ButtonStateStyle HoverButton::disabledStyle() const {
    return {m_disabledBackground, {}, m_disabledContent};
}

void HoverButton::setDisabledStyle(const ButtonStateStyle& style) {
    m_disabledBackground = style.background();
    m_disabledContent = style.content();
    refreshButtonState();
}

ButtonStateStyle HoverButton::checkedStyle() const {
    return {m_checkedBackground, {}, m_checkedContent};
}

void HoverButton::setCheckedStyle(const ButtonStateStyle& style) {
    m_checkedBackground = style.background();
    m_checkedContent = style.content();
    refreshButtonState();
}

QColor HoverButton::backgroundHover() const {
    return m_hoverBackground;
}

void HoverButton::setBackgroundHover(const QColor& color) {
    m_hoverBackground = color;
    refreshButtonState();
}

QColor HoverButton::backgroundPressed() const {
    return m_pressedBackground;
}

void HoverButton::setBackgroundPressed(const QColor& color) {
    m_pressedBackground = color;
    refreshButtonState();
}

QColor HoverButton::backgroundDisabled() const {
    return m_disabledBackground;
}

void HoverButton::setBackgroundDisabled(const QColor& color) {
    m_disabledBackground = color;
    refreshButtonState();
}

QColor HoverButton::backgroundChecked() const {
    return m_checkedBackground;
}

void HoverButton::setBackgroundChecked(const QColor& color) {
    m_checkedBackground = color;
    refreshButtonState();
}

QColor HoverButton::contentNormal() const {
    return m_normalContent;
}

void HoverButton::setContentNormal(const QColor& color) {
    m_normalContent = color;
    refreshButtonState();
}

QColor HoverButton::contentHover() const {
    return m_hoverContent;
}

void HoverButton::setContentHover(const QColor& color) {
    m_hoverContent = color;
    refreshButtonState();
}

QColor HoverButton::contentPressed() const {
    return m_pressedContent;
}

void HoverButton::setContentPressed(const QColor& color) {
    m_pressedContent = color;
    refreshButtonState();
}

QColor HoverButton::contentDisabled() const {
    return m_disabledContent;
}

void HoverButton::setContentDisabled(const QColor& color) {
    m_disabledContent = color;
    refreshButtonState();
}

QColor HoverButton::contentChecked() const {
    return m_checkedContent;
}

void HoverButton::setContentChecked(const QColor& color) {
    m_checkedContent = color;
    refreshButtonState();
}

int HoverButton::cornerRadius() const {
    return m_cornerRadius;
}

void HoverButton::setCornerRadius(int radius) {
    m_cornerRadius = qMax(0, radius);
    update();
}

int HoverButton::iconSpacing() const {
    return m_iconSpacing;
}

void HoverButton::setIconSpacing(int spacing) {
    m_iconSpacing = qMax(0, spacing);
    updateGeometry();
    update();
}

HoverButton::IconPlacement HoverButton::iconPlacement() const {
    return m_iconPlacement;
}

void HoverButton::setIconPlacement(IconPlacement placement) {
    m_iconPlacement = placement;
    updateGeometry();
    update();
}

Qt::Alignment HoverButton::contentAlignment() const {
    return m_contentAlignment;
}

void HoverButton::setContentAlignment(Qt::Alignment alignment) {
    m_contentAlignment = alignment;
    update();
}

int HoverButton::contentHorizontalPadding() const {
    return m_contentHorizontalPadding;
}

void HoverButton::setContentHorizontalPadding(int padding) {
    m_contentHorizontalPadding = qMax(0, padding);
    updateGeometry();
    update();
}

int HoverButton::contentVerticalPadding() const {
    return m_contentVerticalPadding;
}

void HoverButton::setContentVerticalPadding(int padding) {
    m_contentVerticalPadding = qMax(0, padding);
    updateGeometry();
    update();
}

QColor HoverButton::backgroundForState(ButtonState state) const {
    return ButtonStateColorUtils::resolveHoverOnly(
            {ButtonStateColorUtils::defaultActiveFill(), m_hoverBackground, m_pressedBackground, m_checkedBackground, m_disabledBackground}, state,
            darkerStep());
}

QColor HoverButton::contentForState(ButtonState state) const {
    return ButtonStateColorUtils::resolve({m_normalContent, m_hoverContent, m_pressedContent, m_checkedContent, m_disabledContent}, state, darkerStep());
}

void HoverButton::updateAnimationTargets(bool immediate) {
    const ButtonState state = buttonState();
    animateBackgroundColor(backgroundForState(state), immediate);
    const ButtonState contentState = animateContentColor() ? state : ButtonState::Normal;
    animateContentColor(contentForState(contentState), immediate);
}

QSize HoverButton::sizeHint() const {
    const QSize content =
            ButtonPainter::contentSize(ButtonContentOptions{icon(), iconSize(), text(), font(), {}, m_iconSpacing, false, contentPlacement(m_iconPlacement)});
    return ButtonPainter::textSizeHint(content, m_contentHorizontalPadding, m_contentVerticalPadding);
}

void HoverButton::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    ButtonPainter::drawSurface(painter, QRectF(rect()), ButtonSurfaceOptions{animatedBackgroundColor(), {}, 0.0, m_cornerRadius, true, false, false});
    ButtonPainter::drawContent(
            painter, rect(),
            ButtonContentOptions{icon(), iconSize(), text(), font(), animatedContentColor(), m_iconSpacing, false, contentPlacement(m_iconPlacement),
                                 m_contentAlignment, m_contentHorizontalPadding, m_contentVerticalPadding, layoutDirection(), iconHint(), isEnabled()});
}

} // namespace animatedwidgets
