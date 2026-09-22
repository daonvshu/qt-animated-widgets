#include "animatedwidgets/buttons/hovericonbutton.h"

#include "animatedwidgets/buttons/buttonpainter.h"

#include <QPainter>

namespace animatedwidgets {

HoverIconButton::HoverIconButton(QWidget* parent) : AnimatedButtonBase(parent)
{
    refreshButtonState(true);
}

ButtonStateStyle HoverIconButton::normalStyle() const { return {{}, {}, m_normalContent}; }
void HoverIconButton::setNormalStyle(const ButtonStateStyle& style)
{
    m_normalContent = style.content();
    refreshButtonState();
}
ButtonStateStyle HoverIconButton::hoverStyle() const { return {m_hoverBackground, {}, m_hoverContent}; }
void HoverIconButton::setHoverStyle(const ButtonStateStyle& style)
{
    m_hoverBackground = style.background();
    m_hoverContent = style.content();
    refreshButtonState();
}
ButtonStateStyle HoverIconButton::pressedStyle() const { return {m_pressedBackground, {}, m_pressedContent}; }
void HoverIconButton::setPressedStyle(const ButtonStateStyle& style)
{
    m_pressedBackground = style.background();
    m_pressedContent = style.content();
    refreshButtonState();
}
ButtonStateStyle HoverIconButton::disabledStyle() const { return {m_disabledBackground, {}, m_disabledContent}; }
void HoverIconButton::setDisabledStyle(const ButtonStateStyle& style)
{
    m_disabledBackground = style.background();
    m_disabledContent = style.content();
    refreshButtonState();
}
ButtonStateStyle HoverIconButton::checkedStyle() const { return {m_checkedBackground, {}, m_checkedContent}; }
void HoverIconButton::setCheckedStyle(const ButtonStateStyle& style)
{
    m_checkedBackground = style.background();
    m_checkedContent = style.content();
    refreshButtonState();
}

QColor HoverIconButton::hoverBackgroundColor() const { return m_hoverBackground; }
void HoverIconButton::setHoverBackgroundColor(const QColor& color) { m_hoverBackground = color; refreshButtonState(); }
QColor HoverIconButton::pressedBackgroundColor() const { return m_pressedBackground; }
void HoverIconButton::setPressedBackgroundColor(const QColor& color) { m_pressedBackground = color; refreshButtonState(); }
QColor HoverIconButton::disabledBackgroundColor() const { return m_disabledBackground; }
void HoverIconButton::setDisabledBackgroundColor(const QColor& color) { m_disabledBackground = color; refreshButtonState(); }
QColor HoverIconButton::checkedBackgroundColor() const { return m_checkedBackground; }
void HoverIconButton::setCheckedBackgroundColor(const QColor& color) { m_checkedBackground = color; refreshButtonState(); }
QColor HoverIconButton::normalContentColor() const { return m_normalContent; }
void HoverIconButton::setNormalContentColor(const QColor& color) { m_normalContent = color; refreshButtonState(); }
QColor HoverIconButton::hoverContentColor() const { return m_hoverContent; }
void HoverIconButton::setHoverContentColor(const QColor& color) { m_hoverContent = color; refreshButtonState(); }
QColor HoverIconButton::pressedContentColor() const { return m_pressedContent; }
void HoverIconButton::setPressedContentColor(const QColor& color) { m_pressedContent = color; refreshButtonState(); }
QColor HoverIconButton::disabledContentColor() const { return m_disabledContent; }
void HoverIconButton::setDisabledContentColor(const QColor& color) { m_disabledContent = color; refreshButtonState(); }
QColor HoverIconButton::checkedContentColor() const { return m_checkedContent; }
void HoverIconButton::setCheckedContentColor(const QColor& color) { m_checkedContent = color; refreshButtonState(); }

HoverIconButton::Shape HoverIconButton::shape() const { return m_shape; }
void HoverIconButton::setShape(Shape shape) { m_shape = shape; update(); }
int HoverIconButton::cornerRadius() const { return m_cornerRadius; }
void HoverIconButton::setCornerRadius(int radius) { m_cornerRadius = qMax(0, radius); update(); }

QColor HoverIconButton::backgroundForState(ButtonState state) const
{
    const QColor activeBackground("#3478f6");
    switch (state) {
    case ButtonState::Hover: return m_hoverBackground.isValid() ? m_hoverBackground : activeBackground;
    case ButtonState::Pressed: return m_pressedBackground.isValid() ? m_pressedBackground : (m_hoverBackground.isValid() ? m_hoverBackground.darker(100 + darkerStep()) : activeBackground.darker(100 + 2 * darkerStep()));
    case ButtonState::Disabled: return m_disabledBackground.isValid() ? m_disabledBackground : Qt::transparent;
    case ButtonState::Checked: return m_checkedBackground.isValid() ? m_checkedBackground : (m_pressedBackground.isValid() ? m_pressedBackground : (m_hoverBackground.isValid() ? m_hoverBackground.darker(100 + darkerStep()) : activeBackground.darker(100 + 2 * darkerStep())));
    case ButtonState::Normal: return Qt::transparent;
    }
    return Qt::transparent;
}

QColor HoverIconButton::contentForState(ButtonState state) const
{
    switch (state) {
    case ButtonState::Hover: return m_hoverContent.isValid() ? m_hoverContent : darkerStateColor(m_normalContent);
    case ButtonState::Pressed: return m_pressedContent.isValid() ? m_pressedContent : (m_hoverContent.isValid() ? m_hoverContent.darker(100 + darkerStep()) : darkerStateColor(m_normalContent, 2));
    case ButtonState::Disabled: return m_disabledContent.isValid() ? m_disabledContent : m_normalContent;
    case ButtonState::Checked: return m_checkedContent.isValid() ? m_checkedContent : (m_pressedContent.isValid() ? m_pressedContent : (m_hoverContent.isValid() ? m_hoverContent.darker(100 + darkerStep()) : darkerStateColor(m_normalContent, 2)));
    case ButtonState::Normal: return m_normalContent;
    }
    return m_normalContent;
}

void HoverIconButton::updateAnimationTargets(bool immediate)
{
    const ButtonState state = buttonState();
    animateBackgroundColor(backgroundForState(state), immediate);
    const ButtonState contentState = animateContentColor() ? state : ButtonState::Normal;
    animateContentColor(contentForState(contentState), immediate);
}

QSize HoverIconButton::sizeHint() const
{
    const int padding = 12;
    const QSize icon = iconSize();
    return QSize(qMax(icon.width(), icon.height()) + padding * 2,
                 qMax(icon.width(), icon.height()) + padding * 2);
}

void HoverIconButton::paintEvent(QPaintEvent*)
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
