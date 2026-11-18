#include "animatedwidgets/buttons/borderediconbutton.h"

#include "animatedwidgets/utils/buttonpainter.h"
#include "animatedwidgets/utils/buttonstatecolors.h"

#include <QPainter>

namespace animatedwidgets {

BorderedIconButton::BorderedIconButton(QWidget* parent)
    : AnimatedButtonBase(parent)
{
    refreshButtonState(true);
}

ButtonStateStyle BorderedIconButton::normalStyle() const {
    return {m_normalBackground, m_normalBorder, m_normalContent};
}

void BorderedIconButton::setNormalStyle(const ButtonStateStyle& style) {
    m_normalBackground = style.background();
    m_normalBorder = style.border();
    m_normalContent = style.content();
    refreshButtonState();
}

ButtonStateStyle BorderedIconButton::hoverStyle() const {
    return {m_hoverBackground, m_hoverBorder, m_hoverContent};
}

void BorderedIconButton::setHoverStyle(const ButtonStateStyle& style) {
    m_hoverBackground = style.background();
    m_hoverBorder = style.border();
    m_hoverContent = style.content();
    refreshButtonState();
}

ButtonStateStyle BorderedIconButton::pressedStyle() const {
    return {m_pressedBackground, m_pressedBorder, m_pressedContent};
}

void BorderedIconButton::setPressedStyle(const ButtonStateStyle& style) {
    m_pressedBackground = style.background();
    m_pressedBorder = style.border();
    m_pressedContent = style.content();
    refreshButtonState();
}

ButtonStateStyle BorderedIconButton::disabledStyle() const {
    return {m_disabledBackground, m_disabledBorder, m_disabledContent};
}

void BorderedIconButton::setDisabledStyle(const ButtonStateStyle& style) {
    m_disabledBackground = style.background();
    m_disabledBorder = style.border();
    m_disabledContent = style.content();
    refreshButtonState();
}

ButtonStateStyle BorderedIconButton::checkedStyle() const {
    return {m_checkedBackground, m_checkedBorder, m_checkedContent};
}

void BorderedIconButton::setCheckedStyle(const ButtonStateStyle& style) {
    m_checkedBackground = style.background();
    m_checkedBorder = style.border();
    m_checkedContent = style.content();
    refreshButtonState();
}

QColor BorderedIconButton::backgroundNormal() const {
    return m_normalBackground;
}

void BorderedIconButton::setBackgroundNormal(const QColor& color) {
    m_normalBackground = color;
    refreshButtonState();
}

QColor BorderedIconButton::backgroundHover() const {
    return m_hoverBackground;
}

void BorderedIconButton::setBackgroundHover(const QColor& color) {
    m_hoverBackground = color;
    refreshButtonState();
}

QColor BorderedIconButton::backgroundPressed() const {
    return m_pressedBackground;
}

void BorderedIconButton::setBackgroundPressed(const QColor& color) {
    m_pressedBackground = color;
    refreshButtonState();
}

QColor BorderedIconButton::backgroundDisabled() const {
    return m_disabledBackground;
}

void BorderedIconButton::setBackgroundDisabled(const QColor& color) {
    m_disabledBackground = color;
    refreshButtonState();
}

QColor BorderedIconButton::backgroundChecked() const {
    return m_checkedBackground;
}

void BorderedIconButton::setBackgroundChecked(const QColor& color) {
    m_checkedBackground = color;
    refreshButtonState();
}

QColor BorderedIconButton::borderNormal() const {
    return m_normalBorder;
}

void BorderedIconButton::setBorderNormal(const QColor& color) {
    m_normalBorder = color;
    refreshButtonState();
}

QColor BorderedIconButton::borderHover() const {
    return m_hoverBorder;
}

void BorderedIconButton::setBorderHover(const QColor& color) {
    m_hoverBorder = color;
    refreshButtonState();
}

QColor BorderedIconButton::borderPressed() const {
    return m_pressedBorder;
}

void BorderedIconButton::setBorderPressed(const QColor& color) {
    m_pressedBorder = color;
    refreshButtonState();
}

QColor BorderedIconButton::borderDisabled() const {
    return m_disabledBorder;
}

void BorderedIconButton::setBorderDisabled(const QColor& color) {
    m_disabledBorder = color;
    refreshButtonState();
}

QColor BorderedIconButton::borderChecked() const {
    return m_checkedBorder;
}

void BorderedIconButton::setBorderChecked(const QColor& color) {
    m_checkedBorder = color;
    refreshButtonState();
}

QColor BorderedIconButton::contentNormal() const {
    return m_normalContent;
}

void BorderedIconButton::setContentNormal(const QColor& color) {
    m_normalContent = color;
    refreshButtonState();
}

QColor BorderedIconButton::contentHover() const {
    return m_hoverContent;
}

void BorderedIconButton::setContentHover(const QColor& color) {
    m_hoverContent = color;
    refreshButtonState();
}

QColor BorderedIconButton::contentPressed() const {
    return m_pressedContent;
}

void BorderedIconButton::setContentPressed(const QColor& color) {
    m_pressedContent = color;
    refreshButtonState();
}

QColor BorderedIconButton::contentDisabled() const {
    return m_disabledContent;
}

void BorderedIconButton::setContentDisabled(const QColor& color) {
    m_disabledContent = color;
    refreshButtonState();
}

QColor BorderedIconButton::contentChecked() const {
    return m_checkedContent;
}

void BorderedIconButton::setContentChecked(const QColor& color) {
    m_checkedContent = color;
    refreshButtonState();
}

BorderedIconButton::Shape BorderedIconButton::shape() const {
    return m_shape;
}

void BorderedIconButton::setShape(Shape shape) {
    m_shape = shape;
    update();
}

int BorderedIconButton::cornerRadius() const {
    return m_cornerRadius;
}

void BorderedIconButton::setCornerRadius(int radius) {
    m_cornerRadius = qMax(0, radius);
    update();
}

qreal BorderedIconButton::borderWidth() const {
    return m_borderWidth;
}

void BorderedIconButton::setBorderWidth(qreal width) {
    m_borderWidth = qMax(qreal(0), width);
    update();
}

QColor BorderedIconButton::backgroundForState(ButtonState state) const {
    return ButtonStateColorUtils::resolve({m_normalBackground, m_hoverBackground, m_pressedBackground, m_checkedBackground, m_disabledBackground}, state,
                                          darkerStep());
}

QColor BorderedIconButton::borderForState(ButtonState state) const {
    return ButtonStateColorUtils::resolve({m_normalBorder, m_hoverBorder, m_pressedBorder, m_checkedBorder, m_disabledBorder}, state, darkerStep());
}

QColor BorderedIconButton::contentForState(ButtonState state) const {
    return ButtonStateColorUtils::resolve({m_normalContent, m_hoverContent, m_pressedContent, m_checkedContent, m_disabledContent}, state, darkerStep());
}

void BorderedIconButton::updateAnimationTargets(bool immediate) {
    const ButtonState state = buttonState();
    animateBackgroundColor(backgroundForState(state), immediate);
    animateBorderColor(borderForState(state), immediate);
    const ButtonState contentState = animateContentColor() ? state : ButtonState::Normal;
    animateContentColor(contentForState(contentState), immediate);
}

QSize BorderedIconButton::sizeHint() const {
    return ButtonPainter::iconSizeHint(iconSize());
}

void BorderedIconButton::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    ButtonPainter::drawSurface(
            painter, QRectF(rect()),
            ButtonSurfaceOptions{animatedBackgroundColor(), animatedBorderColor(), m_borderWidth, m_cornerRadius, true, true, m_shape == Shape::Circle});
    ButtonPainter::drawContent(
            painter, rect(),
            ButtonContentOptions{icon(), iconSize(), {}, font(), animatedContentColor(), 0, true, ButtonContentOptions::Placement::BeforeText,
                                 Qt::AlignCenter, 0, 0, layoutDirection(), iconHint(), isEnabled()});
}

} // namespace animatedwidgets
