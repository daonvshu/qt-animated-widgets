#include "animatedwidgets/buttons/hovericonbutton.h"

#include "animatedwidgets/utils/buttonpainter.h"
#include "animatedwidgets/utils/buttonstatecolors.h"

#include <QPainter>

namespace animatedwidgets {

HoverIconButton::HoverIconButton(QWidget* parent)
    : AnimatedButtonBase(parent)
{
    refreshButtonState(true);
}

ButtonStateStyle HoverIconButton::normalStyle() const {
    return {{}, {}, m_normalContent};
}

void HoverIconButton::setNormalStyle(const ButtonStateStyle& style) {
    m_normalContent = style.content();
    refreshButtonState();
}

ButtonStateStyle HoverIconButton::hoverStyle() const {
    return {m_hoverBackground, {}, m_hoverContent};
}

void HoverIconButton::setHoverStyle(const ButtonStateStyle& style) {
    m_hoverBackground = style.background();
    m_hoverContent = style.content();
    refreshButtonState();
}

ButtonStateStyle HoverIconButton::pressedStyle() const {
    return {m_pressedBackground, {}, m_pressedContent};
}

void HoverIconButton::setPressedStyle(const ButtonStateStyle& style) {
    m_pressedBackground = style.background();
    m_pressedContent = style.content();
    refreshButtonState();
}

ButtonStateStyle HoverIconButton::disabledStyle() const {
    return {m_disabledBackground, {}, m_disabledContent};
}

void HoverIconButton::setDisabledStyle(const ButtonStateStyle& style) {
    m_disabledBackground = style.background();
    m_disabledContent = style.content();
    refreshButtonState();
}

ButtonStateStyle HoverIconButton::checkedStyle() const {
    return {m_checkedBackground, {}, m_checkedContent};
}

void HoverIconButton::setCheckedStyle(const ButtonStateStyle& style) {
    m_checkedBackground = style.background();
    m_checkedContent = style.content();
    refreshButtonState();
}

QColor HoverIconButton::backgroundHover() const {
    return m_hoverBackground;
}

void HoverIconButton::setBackgroundHover(const QColor& color) {
    m_hoverBackground = color;
    refreshButtonState();
}

QColor HoverIconButton::backgroundPressed() const {
    return m_pressedBackground;
}

void HoverIconButton::setBackgroundPressed(const QColor& color) {
    m_pressedBackground = color;
    refreshButtonState();
}

QColor HoverIconButton::backgroundDisabled() const {
    return m_disabledBackground;
}

void HoverIconButton::setBackgroundDisabled(const QColor& color) {
    m_disabledBackground = color;
    refreshButtonState();
}

QColor HoverIconButton::backgroundChecked() const {
    return m_checkedBackground;
}

void HoverIconButton::setBackgroundChecked(const QColor& color) {
    m_checkedBackground = color;
    refreshButtonState();
}

QColor HoverIconButton::contentNormal() const {
    return m_normalContent;
}

void HoverIconButton::setContentNormal(const QColor& color) {
    m_normalContent = color;
    refreshButtonState();
}

QColor HoverIconButton::contentHover() const {
    return m_hoverContent;
}

void HoverIconButton::setContentHover(const QColor& color) {
    m_hoverContent = color;
    refreshButtonState();
}

QColor HoverIconButton::contentPressed() const {
    return m_pressedContent;
}

void HoverIconButton::setContentPressed(const QColor& color) {
    m_pressedContent = color;
    refreshButtonState();
}

QColor HoverIconButton::contentDisabled() const {
    return m_disabledContent;
}

void HoverIconButton::setContentDisabled(const QColor& color) {
    m_disabledContent = color;
    refreshButtonState();
}

QColor HoverIconButton::contentChecked() const {
    return m_checkedContent;
}

void HoverIconButton::setContentChecked(const QColor& color) {
    m_checkedContent = color;
    refreshButtonState();
}

HoverIconButton::Shape HoverIconButton::shape() const {
    return m_shape;
}

void HoverIconButton::setShape(Shape shape) {
    m_shape = shape;
    update();
}

int HoverIconButton::cornerRadius() const {
    return m_cornerRadius;
}

void HoverIconButton::setCornerRadius(int radius) {
    m_cornerRadius = qMax(0, radius);
    update();
}

QColor HoverIconButton::backgroundForState(ButtonState state) const {
    return ButtonStateColorUtils::resolveHoverOnly(
            {ButtonStateColorUtils::defaultActiveFill(), m_hoverBackground, m_pressedBackground, m_checkedBackground, m_disabledBackground}, state,
            darkerStep());
}

QColor HoverIconButton::contentForState(ButtonState state) const {
    return ButtonStateColorUtils::resolve({m_normalContent, m_hoverContent, m_pressedContent, m_checkedContent, m_disabledContent}, state, darkerStep());
}

void HoverIconButton::updateAnimationTargets(bool immediate) {
    const ButtonState state = buttonState();
    animateBackgroundColor(backgroundForState(state), immediate);
    const ButtonState contentState = animateContentColor() ? state : ButtonState::Normal;
    animateContentColor(contentForState(contentState), immediate);
}

QSize HoverIconButton::sizeHint() const {
    return ButtonPainter::iconSizeHint(iconSize());
}

void HoverIconButton::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    ButtonPainter::drawSurface(painter, QRectF(rect()),
                               ButtonSurfaceOptions{animatedBackgroundColor(), {}, 0.0, m_cornerRadius, true, false, m_shape == Shape::Circle});
    ButtonPainter::drawContent(
            painter, rect(),
            ButtonContentOptions{icon(), iconSize(), {}, font(), animatedContentColor(), 0, true, ButtonContentOptions::Placement::BeforeText});
}

} // namespace animatedwidgets
