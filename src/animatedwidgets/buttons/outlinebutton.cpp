#include "animatedwidgets/buttons/outlinebutton.h"

#include "animatedwidgets/utils/buttonpainter.h"
#include "animatedwidgets/utils/buttonstatecolors.h"

#include <QPainter>

namespace animatedwidgets {

OutlineButton::OutlineButton(QWidget* parent)
    : AnimatedButtonBase(parent)
{
    refreshButtonState(true);
}

ButtonStateStyle OutlineButton::normalStyle() const {
    return {{}, m_normalBorder, m_normalContent};
}

void OutlineButton::setNormalStyle(const ButtonStateStyle& style) {
    m_normalBorder = style.border();
    m_normalContent = style.content();
    refreshButtonState();
}

ButtonStateStyle OutlineButton::hoverStyle() const {
    return {{}, m_hoverBorder, m_hoverContent};
}

void OutlineButton::setHoverStyle(const ButtonStateStyle& style) {
    m_hoverBorder = style.border();
    m_hoverContent = style.content();
    refreshButtonState();
}

ButtonStateStyle OutlineButton::pressedStyle() const {
    return {{}, m_pressedBorder, m_pressedContent};
}

void OutlineButton::setPressedStyle(const ButtonStateStyle& style) {
    m_pressedBorder = style.border();
    m_pressedContent = style.content();
    refreshButtonState();
}

ButtonStateStyle OutlineButton::disabledStyle() const {
    return {{}, m_disabledBorder, m_disabledContent};
}

void OutlineButton::setDisabledStyle(const ButtonStateStyle& style) {
    m_disabledBorder = style.border();
    m_disabledContent = style.content();
    refreshButtonState();
}

ButtonStateStyle OutlineButton::checkedStyle() const {
    return {{}, m_checkedBorder, m_checkedContent};
}

void OutlineButton::setCheckedStyle(const ButtonStateStyle& style) {
    m_checkedBorder = style.border();
    m_checkedContent = style.content();
    refreshButtonState();
}

QColor OutlineButton::borderNormal() const {
    return m_normalBorder;
}

void OutlineButton::setBorderNormal(const QColor& color) {
    m_normalBorder = color;
    refreshButtonState();
}

QColor OutlineButton::borderHover() const {
    return m_hoverBorder;
}

void OutlineButton::setBorderHover(const QColor& color) {
    m_hoverBorder = color;
    refreshButtonState();
}

QColor OutlineButton::borderPressed() const {
    return m_pressedBorder;
}

void OutlineButton::setBorderPressed(const QColor& color) {
    m_pressedBorder = color;
    refreshButtonState();
}

QColor OutlineButton::borderDisabled() const {
    return m_disabledBorder;
}

void OutlineButton::setBorderDisabled(const QColor& color) {
    m_disabledBorder = color;
    refreshButtonState();
}

QColor OutlineButton::borderChecked() const {
    return m_checkedBorder;
}

void OutlineButton::setBorderChecked(const QColor& color) {
    m_checkedBorder = color;
    refreshButtonState();
}

QColor OutlineButton::contentNormal() const {
    return m_normalContent;
}

void OutlineButton::setContentNormal(const QColor& color) {
    m_normalContent = color;
    refreshButtonState();
}

QColor OutlineButton::contentHover() const {
    return m_hoverContent;
}

void OutlineButton::setContentHover(const QColor& color) {
    m_hoverContent = color;
    refreshButtonState();
}

QColor OutlineButton::contentPressed() const {
    return m_pressedContent;
}

void OutlineButton::setContentPressed(const QColor& color) {
    m_pressedContent = color;
    refreshButtonState();
}

QColor OutlineButton::contentDisabled() const {
    return m_disabledContent;
}

void OutlineButton::setContentDisabled(const QColor& color) {
    m_disabledContent = color;
    refreshButtonState();
}

QColor OutlineButton::contentChecked() const {
    return m_checkedContent;
}

void OutlineButton::setContentChecked(const QColor& color) {
    m_checkedContent = color;
    refreshButtonState();
}

int OutlineButton::cornerRadius() const {
    return m_cornerRadius;
}

void OutlineButton::setCornerRadius(int radius) {
    m_cornerRadius = qMax(0, radius);
    update();
}

qreal OutlineButton::borderWidth() const {
    return m_borderWidth;
}

void OutlineButton::setBorderWidth(qreal width) {
    m_borderWidth = qMax(qreal(0), width);
    update();
}

int OutlineButton::iconSpacing() const {
    return m_iconSpacing;
}

void OutlineButton::setIconSpacing(int spacing) {
    m_iconSpacing = qMax(0, spacing);
    updateGeometry();
    update();
}

OutlineButton::IconPlacement OutlineButton::iconPlacement() const {
    return m_iconPlacement;
}

void OutlineButton::setIconPlacement(IconPlacement placement) {
    m_iconPlacement = placement;
    updateGeometry();
    update();
}

QColor OutlineButton::borderForState(ButtonState state) const {
    return ButtonStateColorUtils::resolve({m_normalBorder, m_hoverBorder, m_pressedBorder, m_checkedBorder, m_disabledBorder}, state, darkerStep());
}

QColor OutlineButton::contentForState(ButtonState state) const {
    return ButtonStateColorUtils::resolve({m_normalContent, m_hoverContent, m_pressedContent, m_checkedContent, m_disabledContent}, state, darkerStep());
}

void OutlineButton::updateAnimationTargets(bool immediate) {
    const ButtonState state = buttonState();
    animateBorderColor(borderForState(state), immediate);
    const ButtonState contentState = animateContentColor() ? state : ButtonState::Normal;
    animateContentColor(contentForState(contentState), immediate);
}

QSize OutlineButton::sizeHint() const {
    const QSize content =
            ButtonPainter::contentSize(ButtonContentOptions{icon(), iconSize(), text(), font(), {}, m_iconSpacing, false, contentPlacement(m_iconPlacement)});
    return ButtonPainter::textSizeHint(content);
}

void OutlineButton::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    const qreal inset = m_borderWidth / 2;
    ButtonPainter::drawSurface(painter, QRectF(rect()).adjusted(inset, inset, -inset, -inset),
                               ButtonSurfaceOptions{{}, animatedBorderColor(), m_borderWidth, m_cornerRadius, false, true, false});
    ButtonPainter::drawContent(
            painter, rect(),
            ButtonContentOptions{icon(), iconSize(), text(), font(), animatedContentColor(), m_iconSpacing, false, contentPlacement(m_iconPlacement)});
}

} // namespace animatedwidgets
