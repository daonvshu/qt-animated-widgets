#include "animatedwidgets/buttons/fillediconbutton.h"

#include "animatedwidgets/buttons/buttonpainter.h"

#include <QPainter>

namespace animatedwidgets {

FilledIconButton::FilledIconButton(QWidget* parent) : AnimatedButtonBase(parent)
{
    refreshButtonState(true);
}

ButtonStateStyle FilledIconButton::normalStyle() const { return {m_normalBackground, {}, m_normalContent}; }
void FilledIconButton::setNormalStyle(const ButtonStateStyle& style)
{
    m_normalBackground = style.background();
    m_normalContent = style.content();
    refreshButtonState();
}
ButtonStateStyle FilledIconButton::hoverStyle() const { return {m_hoverBackground, {}, m_hoverContent}; }
void FilledIconButton::setHoverStyle(const ButtonStateStyle& style)
{
    m_hoverBackground = style.background();
    m_hoverContent = style.content();
    refreshButtonState();
}
ButtonStateStyle FilledIconButton::pressedStyle() const { return {m_pressedBackground, {}, m_pressedContent}; }
void FilledIconButton::setPressedStyle(const ButtonStateStyle& style)
{
    m_pressedBackground = style.background();
    m_pressedContent = style.content();
    refreshButtonState();
}
ButtonStateStyle FilledIconButton::disabledStyle() const { return {m_disabledBackground, {}, m_disabledContent}; }
void FilledIconButton::setDisabledStyle(const ButtonStateStyle& style)
{
    m_disabledBackground = style.background();
    m_disabledContent = style.content();
    refreshButtonState();
}
ButtonStateStyle FilledIconButton::checkedStyle() const { return {m_checkedBackground, {}, m_checkedContent}; }
void FilledIconButton::setCheckedStyle(const ButtonStateStyle& style)
{
    m_checkedBackground = style.background();
    m_checkedContent = style.content();
    refreshButtonState();
}

QColor FilledIconButton::normalBackgroundColor() const { return m_normalBackground; }
void FilledIconButton::setNormalBackgroundColor(const QColor& color) { m_normalBackground = color; refreshButtonState(); }
QColor FilledIconButton::hoverBackgroundColor() const { return m_hoverBackground; }
void FilledIconButton::setHoverBackgroundColor(const QColor& color) { m_hoverBackground = color; refreshButtonState(); }
QColor FilledIconButton::pressedBackgroundColor() const { return m_pressedBackground; }
void FilledIconButton::setPressedBackgroundColor(const QColor& color) { m_pressedBackground = color; refreshButtonState(); }
QColor FilledIconButton::disabledBackgroundColor() const { return m_disabledBackground; }
void FilledIconButton::setDisabledBackgroundColor(const QColor& color) { m_disabledBackground = color; refreshButtonState(); }
QColor FilledIconButton::checkedBackgroundColor() const { return m_checkedBackground; }
void FilledIconButton::setCheckedBackgroundColor(const QColor& color) { m_checkedBackground = color; refreshButtonState(); }
QColor FilledIconButton::normalContentColor() const { return m_normalContent; }
void FilledIconButton::setNormalContentColor(const QColor& color) { m_normalContent = color; refreshButtonState(); }
QColor FilledIconButton::hoverContentColor() const { return m_hoverContent; }
void FilledIconButton::setHoverContentColor(const QColor& color) { m_hoverContent = color; refreshButtonState(); }
QColor FilledIconButton::pressedContentColor() const { return m_pressedContent; }
void FilledIconButton::setPressedContentColor(const QColor& color) { m_pressedContent = color; refreshButtonState(); }
QColor FilledIconButton::disabledContentColor() const { return m_disabledContent; }
void FilledIconButton::setDisabledContentColor(const QColor& color) { m_disabledContent = color; refreshButtonState(); }
QColor FilledIconButton::checkedContentColor() const { return m_checkedContent; }
void FilledIconButton::setCheckedContentColor(const QColor& color) { m_checkedContent = color; refreshButtonState(); }

FilledIconButton::Shape FilledIconButton::shape() const { return m_shape; }
void FilledIconButton::setShape(Shape shape) { m_shape = shape; update(); }
int FilledIconButton::cornerRadius() const { return m_cornerRadius; }
void FilledIconButton::setCornerRadius(int radius) { m_cornerRadius = qMax(0, radius); update(); }

QColor FilledIconButton::backgroundForState(ButtonState state) const
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

QColor FilledIconButton::contentForState(ButtonState state) const
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

void FilledIconButton::updateAnimationTargets(bool immediate)
{
    const ButtonState state = buttonState();
    animateBackgroundColor(backgroundForState(state), immediate);
    const ButtonState contentState = animateContentColor() ? state : ButtonState::Normal;
    animateContentColor(contentForState(contentState), immediate);
}

QSize FilledIconButton::sizeHint() const
{
    const int padding = 12;
    const QSize icon = iconSize();
    return QSize(qMax(icon.width(), icon.height()) + padding * 2,
                 qMax(icon.width(), icon.height()) + padding * 2);
}

void FilledIconButton::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    drawButtonSurface(painter, QRectF(rect()), ButtonSurfaceOptions{
        animatedBackgroundColor(), {}, 0.0, m_cornerRadius, true, false,
        m_shape == Shape::Circle});
    drawButtonContent(painter, rect(), ButtonContentOptions{
        icon(), iconSize(), {}, font(), animatedContentColor(), 0, true,
        ButtonContentOptions::Placement::BeforeText});
}

} // namespace animatedwidgets
