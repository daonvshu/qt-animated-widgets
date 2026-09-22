#include "animatedwidgets/buttons/borderediconbutton.h"

#include "animatedwidgets/buttons/buttonpainter.h"

#include <QPainter>

namespace animatedwidgets {

BorderedIconButton::BorderedIconButton(QWidget* parent) : AnimatedButtonBase(parent)
{
    refreshButtonState(true);
}

ButtonStateStyle BorderedIconButton::normalStyle() const { return {m_normalBackground, m_normalBorder, m_normalContent}; }
void BorderedIconButton::setNormalStyle(const ButtonStateStyle& style)
{
    m_normalBackground = style.background();
    m_normalBorder = style.border();
    m_normalContent = style.content();
    refreshButtonState();
}
ButtonStateStyle BorderedIconButton::hoverStyle() const { return {m_hoverBackground, m_hoverBorder, m_hoverContent}; }
void BorderedIconButton::setHoverStyle(const ButtonStateStyle& style)
{
    m_hoverBackground = style.background();
    m_hoverBorder = style.border();
    m_hoverContent = style.content();
    refreshButtonState();
}
ButtonStateStyle BorderedIconButton::pressedStyle() const { return {m_pressedBackground, m_pressedBorder, m_pressedContent}; }
void BorderedIconButton::setPressedStyle(const ButtonStateStyle& style)
{
    m_pressedBackground = style.background();
    m_pressedBorder = style.border();
    m_pressedContent = style.content();
    refreshButtonState();
}
ButtonStateStyle BorderedIconButton::disabledStyle() const { return {m_disabledBackground, m_disabledBorder, m_disabledContent}; }
void BorderedIconButton::setDisabledStyle(const ButtonStateStyle& style)
{
    m_disabledBackground = style.background();
    m_disabledBorder = style.border();
    m_disabledContent = style.content();
    refreshButtonState();
}
ButtonStateStyle BorderedIconButton::checkedStyle() const { return {m_checkedBackground, m_checkedBorder, m_checkedContent}; }
void BorderedIconButton::setCheckedStyle(const ButtonStateStyle& style)
{
    m_checkedBackground = style.background();
    m_checkedBorder = style.border();
    m_checkedContent = style.content();
    refreshButtonState();
}

QColor BorderedIconButton::normalBackgroundColor() const { return m_normalBackground; }
void BorderedIconButton::setNormalBackgroundColor(const QColor& color) { m_normalBackground = color; refreshButtonState(); }
QColor BorderedIconButton::hoverBackgroundColor() const { return m_hoverBackground; }
void BorderedIconButton::setHoverBackgroundColor(const QColor& color) { m_hoverBackground = color; refreshButtonState(); }
QColor BorderedIconButton::pressedBackgroundColor() const { return m_pressedBackground; }
void BorderedIconButton::setPressedBackgroundColor(const QColor& color) { m_pressedBackground = color; refreshButtonState(); }
QColor BorderedIconButton::disabledBackgroundColor() const { return m_disabledBackground; }
void BorderedIconButton::setDisabledBackgroundColor(const QColor& color) { m_disabledBackground = color; refreshButtonState(); }
QColor BorderedIconButton::checkedBackgroundColor() const { return m_checkedBackground; }
void BorderedIconButton::setCheckedBackgroundColor(const QColor& color) { m_checkedBackground = color; refreshButtonState(); }
QColor BorderedIconButton::normalBorderColor() const { return m_normalBorder; }
void BorderedIconButton::setNormalBorderColor(const QColor& color) { m_normalBorder = color; refreshButtonState(); }
QColor BorderedIconButton::hoverBorderColor() const { return m_hoverBorder; }
void BorderedIconButton::setHoverBorderColor(const QColor& color) { m_hoverBorder = color; refreshButtonState(); }
QColor BorderedIconButton::pressedBorderColor() const { return m_pressedBorder; }
void BorderedIconButton::setPressedBorderColor(const QColor& color) { m_pressedBorder = color; refreshButtonState(); }
QColor BorderedIconButton::disabledBorderColor() const { return m_disabledBorder; }
void BorderedIconButton::setDisabledBorderColor(const QColor& color) { m_disabledBorder = color; refreshButtonState(); }
QColor BorderedIconButton::checkedBorderColor() const { return m_checkedBorder; }
void BorderedIconButton::setCheckedBorderColor(const QColor& color) { m_checkedBorder = color; refreshButtonState(); }
QColor BorderedIconButton::normalContentColor() const { return m_normalContent; }
void BorderedIconButton::setNormalContentColor(const QColor& color) { m_normalContent = color; refreshButtonState(); }
QColor BorderedIconButton::hoverContentColor() const { return m_hoverContent; }
void BorderedIconButton::setHoverContentColor(const QColor& color) { m_hoverContent = color; refreshButtonState(); }
QColor BorderedIconButton::pressedContentColor() const { return m_pressedContent; }
void BorderedIconButton::setPressedContentColor(const QColor& color) { m_pressedContent = color; refreshButtonState(); }
QColor BorderedIconButton::disabledContentColor() const { return m_disabledContent; }
void BorderedIconButton::setDisabledContentColor(const QColor& color) { m_disabledContent = color; refreshButtonState(); }
QColor BorderedIconButton::checkedContentColor() const { return m_checkedContent; }
void BorderedIconButton::setCheckedContentColor(const QColor& color) { m_checkedContent = color; refreshButtonState(); }

BorderedIconButton::Shape BorderedIconButton::shape() const { return m_shape; }
void BorderedIconButton::setShape(Shape shape) { m_shape = shape; update(); }
int BorderedIconButton::cornerRadius() const { return m_cornerRadius; }
void BorderedIconButton::setCornerRadius(int radius) { m_cornerRadius = qMax(0, radius); update(); }
qreal BorderedIconButton::borderWidth() const { return m_borderWidth; }
void BorderedIconButton::setBorderWidth(qreal width) { m_borderWidth = qMax(qreal(0), width); update(); }

QColor BorderedIconButton::backgroundForState(ButtonState state) const
{
    switch (state) {
    case ButtonState::Hover: return m_hoverBackground.isValid() ? m_hoverBackground : darkerStateColor(m_normalBackground);
    case ButtonState::Pressed: return m_pressedBackground.isValid() ? m_pressedBackground : (m_hoverBackground.isValid() ? m_hoverBackground.darker(100 + darkerStep()) : darkerStateColor(m_normalBackground, 2));
    case ButtonState::Disabled: return m_disabledBackground.isValid() ? m_disabledBackground : m_normalBackground;
    case ButtonState::Checked: return m_checkedBackground.isValid() ? m_checkedBackground : (m_pressedBackground.isValid() ? m_pressedBackground : darkerStateColor(m_normalBackground, 2));
    case ButtonState::Normal: return m_normalBackground;
    }
    return m_normalBackground;
}

QColor BorderedIconButton::borderForState(ButtonState state) const
{
    switch (state) {
    case ButtonState::Hover: return m_hoverBorder.isValid() ? m_hoverBorder : darkerStateColor(m_normalBorder);
    case ButtonState::Pressed: return m_pressedBorder.isValid() ? m_pressedBorder : (m_hoverBorder.isValid() ? m_hoverBorder.darker(100 + darkerStep()) : darkerStateColor(m_normalBorder, 2));
    case ButtonState::Disabled: return m_disabledBorder.isValid() ? m_disabledBorder : m_normalBorder;
    case ButtonState::Checked: return m_checkedBorder.isValid() ? m_checkedBorder : (m_pressedBorder.isValid() ? m_pressedBorder : darkerStateColor(m_normalBorder, 2));
    case ButtonState::Normal: return m_normalBorder;
    }
    return m_normalBorder;
}

QColor BorderedIconButton::contentForState(ButtonState state) const
{
    switch (state) {
    case ButtonState::Hover: return m_hoverContent.isValid() ? m_hoverContent : darkerStateColor(m_normalContent);
    case ButtonState::Pressed: return m_pressedContent.isValid() ? m_pressedContent : (m_hoverContent.isValid() ? m_hoverContent.darker(100 + darkerStep()) : darkerStateColor(m_normalContent, 2));
    case ButtonState::Disabled: return m_disabledContent.isValid() ? m_disabledContent : m_normalContent;
    case ButtonState::Checked: return m_checkedContent.isValid() ? m_checkedContent : (m_pressedContent.isValid() ? m_pressedContent : darkerStateColor(m_normalContent, 2));
    case ButtonState::Normal: return m_normalContent;
    }
    return m_normalContent;
}

void BorderedIconButton::updateAnimationTargets(bool immediate)
{
    const ButtonState state = buttonState();
    animateBackgroundColor(backgroundForState(state), immediate);
    animateBorderColor(borderForState(state), immediate);
    const ButtonState contentState = animateContentColor() ? state : ButtonState::Normal;
    animateContentColor(contentForState(contentState), immediate);
}

QSize BorderedIconButton::sizeHint() const
{
    const int padding = 12;
    const QSize icon = iconSize();
    return QSize(qMax(icon.width(), icon.height()) + padding * 2,
                 qMax(icon.width(), icon.height()) + padding * 2);
}

void BorderedIconButton::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    const qreal inset = m_borderWidth / 2;
    drawButtonSurface(painter, QRectF(rect()).adjusted(inset, inset, -inset, -inset),
                      ButtonSurfaceOptions{animatedBackgroundColor(), animatedBorderColor(),
                                           m_borderWidth, m_cornerRadius, true, true,
                                           m_shape == Shape::Circle});
    drawButtonContent(painter, rect(), ButtonContentOptions{
        icon(), iconSize(), {}, font(), animatedContentColor(), 0, true,
        ButtonContentOptions::Placement::BeforeText});
}

} // namespace animatedwidgets
