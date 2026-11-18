#include "animatedwidgets/buttons/borderedbutton.h"

#include "animatedwidgets/utils/buttonpainter.h"
#include "animatedwidgets/utils/buttonstatecolors.h"

#include <QPainter>

namespace animatedwidgets {

BorderedButton::BorderedButton(QWidget* parent)
    : AnimatedButtonBase(parent)
{
    refreshButtonState(true);
}

ButtonStateStyle BorderedButton::normalStyle() const {
    return {m_normalBackground, m_normalBorder, m_normalContent};
}

void BorderedButton::setNormalStyle(const ButtonStateStyle& style) {
    m_normalBackground = style.background();
    m_normalBorder = style.border();
    m_normalContent = style.content();
    refreshButtonState();
}

ButtonStateStyle BorderedButton::hoverStyle() const {
    return {m_hoverBackground, m_hoverBorder, m_hoverContent};
}

void BorderedButton::setHoverStyle(const ButtonStateStyle& style) {
    m_hoverBackground = style.background();
    m_hoverBorder = style.border();
    m_hoverContent = style.content();
    refreshButtonState();
}

ButtonStateStyle BorderedButton::pressedStyle() const {
    return {m_pressedBackground, m_pressedBorder, m_pressedContent};
}

void BorderedButton::setPressedStyle(const ButtonStateStyle& style) {
    m_pressedBackground = style.background();
    m_pressedBorder = style.border();
    m_pressedContent = style.content();
    refreshButtonState();
}

ButtonStateStyle BorderedButton::disabledStyle() const {
    return {m_disabledBackground, m_disabledBorder, m_disabledContent};
}

void BorderedButton::setDisabledStyle(const ButtonStateStyle& style) {
    m_disabledBackground = style.background();
    m_disabledBorder = style.border();
    m_disabledContent = style.content();
    refreshButtonState();
}

ButtonStateStyle BorderedButton::checkedStyle() const {
    return {m_checkedBackground, m_checkedBorder, m_checkedContent};
}

void BorderedButton::setCheckedStyle(const ButtonStateStyle& style) {
    m_checkedBackground = style.background();
    m_checkedBorder = style.border();
    m_checkedContent = style.content();
    refreshButtonState();
}

QColor BorderedButton::backgroundNormal() const {
    return m_normalBackground;
}

void BorderedButton::setBackgroundNormal(const QColor& color) {
    m_normalBackground = color;
    refreshButtonState();
}

QColor BorderedButton::backgroundHover() const {
    return m_hoverBackground;
}

void BorderedButton::setBackgroundHover(const QColor& color) {
    m_hoverBackground = color;
    refreshButtonState();
}

QColor BorderedButton::backgroundPressed() const {
    return m_pressedBackground;
}

void BorderedButton::setBackgroundPressed(const QColor& color) {
    m_pressedBackground = color;
    refreshButtonState();
}

QColor BorderedButton::backgroundDisabled() const {
    return m_disabledBackground;
}

void BorderedButton::setBackgroundDisabled(const QColor& color) {
    m_disabledBackground = color;
    refreshButtonState();
}

QColor BorderedButton::backgroundChecked() const {
    return m_checkedBackground;
}

void BorderedButton::setBackgroundChecked(const QColor& color) {
    m_checkedBackground = color;
    refreshButtonState();
}

QColor BorderedButton::borderNormal() const {
    return m_normalBorder;
}

void BorderedButton::setBorderNormal(const QColor& color) {
    m_normalBorder = color;
    refreshButtonState();
}

QColor BorderedButton::borderHover() const {
    return m_hoverBorder;
}

void BorderedButton::setBorderHover(const QColor& color) {
    m_hoverBorder = color;
    refreshButtonState();
}

QColor BorderedButton::borderPressed() const {
    return m_pressedBorder;
}

void BorderedButton::setBorderPressed(const QColor& color) {
    m_pressedBorder = color;
    refreshButtonState();
}

QColor BorderedButton::borderDisabled() const {
    return m_disabledBorder;
}

void BorderedButton::setBorderDisabled(const QColor& color) {
    m_disabledBorder = color;
    refreshButtonState();
}

QColor BorderedButton::borderChecked() const {
    return m_checkedBorder;
}

void BorderedButton::setBorderChecked(const QColor& color) {
    m_checkedBorder = color;
    refreshButtonState();
}

QColor BorderedButton::contentNormal() const {
    return m_normalContent;
}

void BorderedButton::setContentNormal(const QColor& color) {
    m_normalContent = color;
    refreshButtonState();
}

QColor BorderedButton::contentHover() const {
    return m_hoverContent;
}

void BorderedButton::setContentHover(const QColor& color) {
    m_hoverContent = color;
    refreshButtonState();
}

QColor BorderedButton::contentPressed() const {
    return m_pressedContent;
}

void BorderedButton::setContentPressed(const QColor& color) {
    m_pressedContent = color;
    refreshButtonState();
}

QColor BorderedButton::contentDisabled() const {
    return m_disabledContent;
}

void BorderedButton::setContentDisabled(const QColor& color) {
    m_disabledContent = color;
    refreshButtonState();
}

QColor BorderedButton::contentChecked() const {
    return m_checkedContent;
}

void BorderedButton::setContentChecked(const QColor& color) {
    m_checkedContent = color;
    refreshButtonState();
}

int BorderedButton::cornerRadius() const {
    return m_cornerRadius;
}

void BorderedButton::setCornerRadius(int radius) {
    m_cornerRadius = qMax(0, radius);
    update();
}

qreal BorderedButton::borderWidth() const {
    return m_borderWidth;
}

void BorderedButton::setBorderWidth(qreal width) {
    m_borderWidth = qMax(qreal(0), width);
    update();
}

int BorderedButton::iconSpacing() const {
    return m_iconSpacing;
}

void BorderedButton::setIconSpacing(int spacing) {
    m_iconSpacing = qMax(0, spacing);
    updateGeometry();
    update();
}

BorderedButton::IconPlacement BorderedButton::iconPlacement() const {
    return m_iconPlacement;
}

void BorderedButton::setIconPlacement(IconPlacement placement) {
    m_iconPlacement = placement;
    updateGeometry();
    update();
}

Qt::Alignment BorderedButton::contentAlignment() const {
    return m_contentAlignment;
}

void BorderedButton::setContentAlignment(Qt::Alignment alignment) {
    m_contentAlignment = alignment;
    update();
}

int BorderedButton::contentHorizontalPadding() const {
    return m_contentHorizontalPadding;
}

void BorderedButton::setContentHorizontalPadding(int padding) {
    m_contentHorizontalPadding = qMax(0, padding);
    updateGeometry();
    update();
}

int BorderedButton::contentVerticalPadding() const {
    return m_contentVerticalPadding;
}

void BorderedButton::setContentVerticalPadding(int padding) {
    m_contentVerticalPadding = qMax(0, padding);
    updateGeometry();
    update();
}

QColor BorderedButton::backgroundForState(ButtonState state) const {
    return ButtonStateColorUtils::resolve({m_normalBackground, m_hoverBackground, m_pressedBackground, m_checkedBackground, m_disabledBackground}, state,
                                          darkerStep());
}

QColor BorderedButton::borderForState(ButtonState state) const {
    return ButtonStateColorUtils::resolve({m_normalBorder, m_hoverBorder, m_pressedBorder, m_checkedBorder, m_disabledBorder}, state, darkerStep());
}

QColor BorderedButton::contentForState(ButtonState state) const {
    return ButtonStateColorUtils::resolve({m_normalContent, m_hoverContent, m_pressedContent, m_checkedContent, m_disabledContent}, state, darkerStep());
}

void BorderedButton::updateAnimationTargets(bool immediate) {
    const ButtonState state = buttonState();
    animateBackgroundColor(backgroundForState(state), immediate);
    animateBorderColor(borderForState(state), immediate);
    const ButtonState contentState = animateContentColor() ? state : ButtonState::Normal;
    animateContentColor(contentForState(contentState), immediate);
}

QSize BorderedButton::sizeHint() const {
    const QSize content =
            ButtonPainter::contentSize(ButtonContentOptions{icon(), iconSize(), text(), font(), {}, m_iconSpacing, false, contentPlacement(m_iconPlacement)});
    return ButtonPainter::textSizeHint(content, m_contentHorizontalPadding, m_contentVerticalPadding);
}

void BorderedButton::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    ButtonPainter::drawSurface(painter, QRectF(rect()),
                               ButtonSurfaceOptions{animatedBackgroundColor(), animatedBorderColor(), m_borderWidth, m_cornerRadius, true, true, false});
    ButtonPainter::drawContent(
            painter, rect(),
            ButtonContentOptions{icon(), iconSize(), text(), font(), animatedContentColor(), m_iconSpacing, false, contentPlacement(m_iconPlacement),
                                 m_contentAlignment, m_contentHorizontalPadding, m_contentVerticalPadding, layoutDirection(), iconHint(), isEnabled()});
}

} // namespace animatedwidgets
