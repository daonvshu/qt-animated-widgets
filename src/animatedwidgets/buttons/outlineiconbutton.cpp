#include "animatedwidgets/buttons/outlineiconbutton.h"

#include "animatedwidgets/utils/buttonpainter.h"
#include "animatedwidgets/utils/buttonstatecolors.h"

#include <QPainter>

namespace animatedwidgets {

OutlineIconButton::OutlineIconButton(QWidget* parent)
    : AnimatedButtonBase(parent)
{
    refreshButtonState(true);
}

ButtonStateStyle OutlineIconButton::normalStyle() const {
    return {{}, m_normalBorder, m_normalContent};
}

void OutlineIconButton::setNormalStyle(const ButtonStateStyle& style) {
    m_normalBorder = style.border();
    m_normalContent = style.content();
    refreshButtonState();
}

ButtonStateStyle OutlineIconButton::hoverStyle() const {
    return {{}, m_hoverBorder, m_hoverContent};
}

void OutlineIconButton::setHoverStyle(const ButtonStateStyle& style) {
    m_hoverBorder = style.border();
    m_hoverContent = style.content();
    refreshButtonState();
}

ButtonStateStyle OutlineIconButton::pressedStyle() const {
    return {{}, m_pressedBorder, m_pressedContent};
}

void OutlineIconButton::setPressedStyle(const ButtonStateStyle& style) {
    m_pressedBorder = style.border();
    m_pressedContent = style.content();
    refreshButtonState();
}

ButtonStateStyle OutlineIconButton::disabledStyle() const {
    return {{}, m_disabledBorder, m_disabledContent};
}

void OutlineIconButton::setDisabledStyle(const ButtonStateStyle& style) {
    m_disabledBorder = style.border();
    m_disabledContent = style.content();
    refreshButtonState();
}

ButtonStateStyle OutlineIconButton::checkedStyle() const {
    return {{}, m_checkedBorder, m_checkedContent};
}

void OutlineIconButton::setCheckedStyle(const ButtonStateStyle& style) {
    m_checkedBorder = style.border();
    m_checkedContent = style.content();
    refreshButtonState();
}

QColor OutlineIconButton::borderNormal() const {
    return m_normalBorder;
}

void OutlineIconButton::setBorderNormal(const QColor& color) {
    m_normalBorder = color;
    refreshButtonState();
}

QColor OutlineIconButton::borderHover() const {
    return m_hoverBorder;
}

void OutlineIconButton::setBorderHover(const QColor& color) {
    m_hoverBorder = color;
    refreshButtonState();
}

QColor OutlineIconButton::borderPressed() const {
    return m_pressedBorder;
}

void OutlineIconButton::setBorderPressed(const QColor& color) {
    m_pressedBorder = color;
    refreshButtonState();
}

QColor OutlineIconButton::borderDisabled() const {
    return m_disabledBorder;
}

void OutlineIconButton::setBorderDisabled(const QColor& color) {
    m_disabledBorder = color;
    refreshButtonState();
}

QColor OutlineIconButton::borderChecked() const {
    return m_checkedBorder;
}

void OutlineIconButton::setBorderChecked(const QColor& color) {
    m_checkedBorder = color;
    refreshButtonState();
}

QColor OutlineIconButton::contentNormal() const {
    return m_normalContent;
}

void OutlineIconButton::setContentNormal(const QColor& color) {
    m_normalContent = color;
    refreshButtonState();
}

QColor OutlineIconButton::contentHover() const {
    return m_hoverContent;
}

void OutlineIconButton::setContentHover(const QColor& color) {
    m_hoverContent = color;
    refreshButtonState();
}

QColor OutlineIconButton::contentPressed() const {
    return m_pressedContent;
}

void OutlineIconButton::setContentPressed(const QColor& color) {
    m_pressedContent = color;
    refreshButtonState();
}

QColor OutlineIconButton::contentDisabled() const {
    return m_disabledContent;
}

void OutlineIconButton::setContentDisabled(const QColor& color) {
    m_disabledContent = color;
    refreshButtonState();
}

QColor OutlineIconButton::contentChecked() const {
    return m_checkedContent;
}

void OutlineIconButton::setContentChecked(const QColor& color) {
    m_checkedContent = color;
    refreshButtonState();
}

OutlineIconButton::Shape OutlineIconButton::shape() const {
    return m_shape;
}

void OutlineIconButton::setShape(Shape shape) {
    m_shape = shape;
    update();
}

int OutlineIconButton::cornerRadius() const {
    return m_cornerRadius;
}

void OutlineIconButton::setCornerRadius(int radius) {
    m_cornerRadius = qMax(0, radius);
    update();
}

qreal OutlineIconButton::borderWidth() const {
    return m_borderWidth;
}

void OutlineIconButton::setBorderWidth(qreal width) {
    m_borderWidth = qMax(qreal(0), width);
    update();
}

QColor OutlineIconButton::borderForState(ButtonState state) const {
    return ButtonStateColorUtils::resolve({m_normalBorder, m_hoverBorder, m_pressedBorder, m_checkedBorder, m_disabledBorder}, state, darkerStep());
}

QColor OutlineIconButton::contentForState(ButtonState state) const {
    return ButtonStateColorUtils::resolve({m_normalContent, m_hoverContent, m_pressedContent, m_checkedContent, m_disabledContent}, state, darkerStep());
}

void OutlineIconButton::updateAnimationTargets(bool immediate) {
    const ButtonState state = buttonState();
    animateBorderColor(borderForState(state), immediate);
    const ButtonState contentState = animateContentColor() ? state : ButtonState::Normal;
    animateContentColor(contentForState(contentState), immediate);
}

QSize OutlineIconButton::sizeHint() const {
    return ButtonPainter::iconSizeHint(iconSize());
}

void OutlineIconButton::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    const qreal inset = m_borderWidth / 2;
    ButtonPainter::drawSurface(painter, QRectF(rect()).adjusted(inset, inset, -inset, -inset),
                               ButtonSurfaceOptions{{}, animatedBorderColor(), m_borderWidth, m_cornerRadius, false, true, m_shape == Shape::Circle});
    ButtonPainter::drawContent(
            painter, rect(),
            ButtonContentOptions{icon(), iconSize(), {}, font(), animatedContentColor(), 0, true, ButtonContentOptions::Placement::BeforeText,
                                 Qt::AlignCenter, 0, 0, layoutDirection(), iconHint(), isEnabled()});
}

} // namespace animatedwidgets
