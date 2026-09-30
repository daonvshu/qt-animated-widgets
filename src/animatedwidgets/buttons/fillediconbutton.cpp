#include "animatedwidgets/buttons/fillediconbutton.h"

#include "animatedwidgets/utils/buttonpainter.h"
#include "animatedwidgets/utils/buttonstatecolors.h"

#include <QPainter>

namespace animatedwidgets {

FilledIconButton::FilledIconButton(QWidget* parent)
    : AnimatedButtonBase(parent)
{
    refreshButtonState(true);
}

ButtonStateStyle FilledIconButton::normalStyle() const {
    return {m_normalBackground, {}, m_normalContent};
}

void FilledIconButton::setNormalStyle(const ButtonStateStyle& style) {
    m_normalBackground = style.background();
    m_normalContent = style.content();
    refreshButtonState();
}

ButtonStateStyle FilledIconButton::hoverStyle() const {
    return {m_hoverBackground, {}, m_hoverContent};
}

void FilledIconButton::setHoverStyle(const ButtonStateStyle& style) {
    m_hoverBackground = style.background();
    m_hoverContent = style.content();
    refreshButtonState();
}

ButtonStateStyle FilledIconButton::pressedStyle() const {
    return {m_pressedBackground, {}, m_pressedContent};
}

void FilledIconButton::setPressedStyle(const ButtonStateStyle& style) {
    m_pressedBackground = style.background();
    m_pressedContent = style.content();
    refreshButtonState();
}

ButtonStateStyle FilledIconButton::disabledStyle() const {
    return {m_disabledBackground, {}, m_disabledContent};
}

void FilledIconButton::setDisabledStyle(const ButtonStateStyle& style) {
    m_disabledBackground = style.background();
    m_disabledContent = style.content();
    refreshButtonState();
}

ButtonStateStyle FilledIconButton::checkedStyle() const {
    return {m_checkedBackground, {}, m_checkedContent};
}

void FilledIconButton::setCheckedStyle(const ButtonStateStyle& style) {
    m_checkedBackground = style.background();
    m_checkedContent = style.content();
    refreshButtonState();
}

QColor FilledIconButton::backgroundNormal() const {
    return m_normalBackground;
}

void FilledIconButton::setBackgroundNormal(const QColor& color) {
    m_normalBackground = color;
    refreshButtonState();
}

QColor FilledIconButton::backgroundHover() const {
    return m_hoverBackground;
}

void FilledIconButton::setBackgroundHover(const QColor& color) {
    m_hoverBackground = color;
    refreshButtonState();
}

QColor FilledIconButton::backgroundPressed() const {
    return m_pressedBackground;
}

void FilledIconButton::setBackgroundPressed(const QColor& color) {
    m_pressedBackground = color;
    refreshButtonState();
}

QColor FilledIconButton::backgroundDisabled() const {
    return m_disabledBackground;
}

void FilledIconButton::setBackgroundDisabled(const QColor& color) {
    m_disabledBackground = color;
    refreshButtonState();
}

QColor FilledIconButton::backgroundChecked() const {
    return m_checkedBackground;
}

void FilledIconButton::setBackgroundChecked(const QColor& color) {
    m_checkedBackground = color;
    refreshButtonState();
}

QColor FilledIconButton::contentNormal() const {
    return m_normalContent;
}

void FilledIconButton::setContentNormal(const QColor& color) {
    m_normalContent = color;
    refreshButtonState();
}

QColor FilledIconButton::contentHover() const {
    return m_hoverContent;
}

void FilledIconButton::setContentHover(const QColor& color) {
    m_hoverContent = color;
    refreshButtonState();
}

QColor FilledIconButton::contentPressed() const {
    return m_pressedContent;
}

void FilledIconButton::setContentPressed(const QColor& color) {
    m_pressedContent = color;
    refreshButtonState();
}

QColor FilledIconButton::contentDisabled() const {
    return m_disabledContent;
}

void FilledIconButton::setContentDisabled(const QColor& color) {
    m_disabledContent = color;
    refreshButtonState();
}

QColor FilledIconButton::contentChecked() const {
    return m_checkedContent;
}

void FilledIconButton::setContentChecked(const QColor& color) {
    m_checkedContent = color;
    refreshButtonState();
}

FilledIconButton::Shape FilledIconButton::shape() const {
    return m_shape;
}

void FilledIconButton::setShape(Shape shape) {
    m_shape = shape;
    update();
}

int FilledIconButton::cornerRadius() const {
    return m_cornerRadius;
}

void FilledIconButton::setCornerRadius(int radius) {
    m_cornerRadius = qMax(0, radius);
    update();
}

QColor FilledIconButton::backgroundForState(ButtonState state) const {
    return ButtonStateColorUtils::resolve({m_normalBackground, m_hoverBackground, m_pressedBackground, m_checkedBackground, m_disabledBackground}, state,
                                          darkerStep());
}

QColor FilledIconButton::contentForState(ButtonState state) const {
    return ButtonStateColorUtils::resolve({m_normalContent, m_hoverContent, m_pressedContent, m_checkedContent, m_disabledContent}, state, darkerStep());
}

void FilledIconButton::updateAnimationTargets(bool immediate) {
    const ButtonState state = buttonState();
    animateBackgroundColor(backgroundForState(state), immediate);
    const ButtonState contentState = animateContentColor() ? state : ButtonState::Normal;
    animateContentColor(contentForState(contentState), immediate);
}

QSize FilledIconButton::sizeHint() const {
    return ButtonPainter::iconSizeHint(iconSize());
}

void FilledIconButton::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    ButtonPainter::drawSurface(painter, QRectF(rect()),
                               ButtonSurfaceOptions{animatedBackgroundColor(), {}, 0.0, m_cornerRadius, true, false, m_shape == Shape::Circle});
    ButtonPainter::drawContent(
            painter, rect(),
            ButtonContentOptions{icon(), iconSize(), {}, font(), animatedContentColor(), 0, true, ButtonContentOptions::Placement::BeforeText});
}

} // namespace animatedwidgets
