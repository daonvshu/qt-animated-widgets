#include "animatedwidgets/buttons/hoverbutton.h"

#include "animatedwidgets/buttons/buttonpainter.h"

#include <QPainter>

namespace animatedwidgets {

namespace {
ButtonContentOptions::Placement contentPlacement(AnimatedButtonBase::IconPlacement placement)
{
    switch (placement) {
    case AnimatedButtonBase::IconPlacement::AfterText: return ButtonContentOptions::Placement::AfterText;
    case AnimatedButtonBase::IconPlacement::AboveText: return ButtonContentOptions::Placement::AboveText;
    case AnimatedButtonBase::IconPlacement::BelowText: return ButtonContentOptions::Placement::BelowText;
    default: return ButtonContentOptions::Placement::BeforeText;
    }
}
}

HoverButton::HoverButton(QWidget* parent) : AnimatedButtonBase(parent)
{
    refreshButtonState(true);
}

ButtonStateStyle HoverButton::normalStyle() const { return {{}, {}, m_normalContent}; }
void HoverButton::setNormalStyle(const ButtonStateStyle& style)
{
    m_normalContent = style.content();
    refreshButtonState();
}
ButtonStateStyle HoverButton::hoverStyle() const { return {m_hoverBackground, {}, m_hoverContent}; }
void HoverButton::setHoverStyle(const ButtonStateStyle& style)
{
    m_hoverBackground = style.background();
    m_hoverContent = style.content();
    refreshButtonState();
}
ButtonStateStyle HoverButton::pressedStyle() const { return {m_pressedBackground, {}, m_pressedContent}; }
void HoverButton::setPressedStyle(const ButtonStateStyle& style)
{
    m_pressedBackground = style.background();
    m_pressedContent = style.content();
    refreshButtonState();
}
ButtonStateStyle HoverButton::disabledStyle() const { return {m_disabledBackground, {}, m_disabledContent}; }
void HoverButton::setDisabledStyle(const ButtonStateStyle& style)
{
    m_disabledBackground = style.background();
    m_disabledContent = style.content();
    refreshButtonState();
}
ButtonStateStyle HoverButton::checkedStyle() const { return {m_checkedBackground, {}, m_checkedContent}; }
void HoverButton::setCheckedStyle(const ButtonStateStyle& style)
{
    m_checkedBackground = style.background();
    m_checkedContent = style.content();
    refreshButtonState();
}

QColor HoverButton::hoverBackgroundColor() const { return m_hoverBackground; }
void HoverButton::setHoverBackgroundColor(const QColor& color) { m_hoverBackground = color; refreshButtonState(); }
QColor HoverButton::pressedBackgroundColor() const { return m_pressedBackground; }
void HoverButton::setPressedBackgroundColor(const QColor& color) { m_pressedBackground = color; refreshButtonState(); }
QColor HoverButton::disabledBackgroundColor() const { return m_disabledBackground; }
void HoverButton::setDisabledBackgroundColor(const QColor& color) { m_disabledBackground = color; refreshButtonState(); }
QColor HoverButton::checkedBackgroundColor() const { return m_checkedBackground; }
void HoverButton::setCheckedBackgroundColor(const QColor& color) { m_checkedBackground = color; refreshButtonState(); }
QColor HoverButton::normalContentColor() const { return m_normalContent; }
void HoverButton::setNormalContentColor(const QColor& color) { m_normalContent = color; refreshButtonState(); }
QColor HoverButton::hoverContentColor() const { return m_hoverContent; }
void HoverButton::setHoverContentColor(const QColor& color) { m_hoverContent = color; refreshButtonState(); }
QColor HoverButton::pressedContentColor() const { return m_pressedContent; }
void HoverButton::setPressedContentColor(const QColor& color) { m_pressedContent = color; refreshButtonState(); }
QColor HoverButton::disabledContentColor() const { return m_disabledContent; }
void HoverButton::setDisabledContentColor(const QColor& color) { m_disabledContent = color; refreshButtonState(); }
QColor HoverButton::checkedContentColor() const { return m_checkedContent; }
void HoverButton::setCheckedContentColor(const QColor& color) { m_checkedContent = color; refreshButtonState(); }

int HoverButton::cornerRadius() const { return m_cornerRadius; }
void HoverButton::setCornerRadius(int radius) { m_cornerRadius = qMax(0, radius); update(); }
int HoverButton::iconSpacing() const { return m_iconSpacing; }
void HoverButton::setIconSpacing(int spacing) { m_iconSpacing = qMax(0, spacing); updateGeometry(); update(); }
HoverButton::IconPlacement HoverButton::iconPlacement() const { return m_iconPlacement; }
void HoverButton::setIconPlacement(IconPlacement placement) { m_iconPlacement = placement; updateGeometry(); update(); }

QColor HoverButton::backgroundForState(ButtonState state) const
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

QColor HoverButton::contentForState(ButtonState state) const
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

void HoverButton::updateAnimationTargets(bool immediate)
{
    const ButtonState state = buttonState();
    animateBackgroundColor(backgroundForState(state), immediate);
    const ButtonState contentState = animateContentColor() ? state : ButtonState::Normal;
    animateContentColor(contentForState(contentState), immediate);
}

QSize HoverButton::sizeHint() const
{
    ButtonContentOptions options{icon(), iconSize(), text(), font(), {}, m_iconSpacing, false,
                                 contentPlacement(m_iconPlacement)};
    const QSize content = buttonContentSize(options);
    return QSize(qMax(80, content.width() + 28), qMax(36, content.height() + 16));
}

void HoverButton::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    drawButtonSurface(painter, QRectF(rect()), ButtonSurfaceOptions{
        animatedBackgroundColor(), {}, 0.0, m_cornerRadius, true, false, false});
    drawButtonContent(painter, rect(), ButtonContentOptions{
        icon(), iconSize(), text(), font(), animatedContentColor(), m_iconSpacing, false,
        contentPlacement(m_iconPlacement)});
}

} // namespace animatedwidgets
