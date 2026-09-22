#include "animatedwidgets/buttons/filledbutton.h"

#include "animatedwidgets/buttons/buttonpainter.h"

#include <QPainter>

namespace animatedwidgets {

namespace {
ButtonContentOptions::Placement contentPlacement(AnimatedButtonBase::IconPlacement placement)
{
    switch (placement) {
    case AnimatedButtonBase::IconPlacement::AfterText:
        return ButtonContentOptions::Placement::AfterText;
    case AnimatedButtonBase::IconPlacement::AboveText:
        return ButtonContentOptions::Placement::AboveText;
    case AnimatedButtonBase::IconPlacement::BelowText:
        return ButtonContentOptions::Placement::BelowText;
    default:
        return ButtonContentOptions::Placement::BeforeText;
    }
}
}

FilledButton::FilledButton(QWidget* parent)
    : AnimatedButtonBase(parent)
{
    refreshButtonState(true);
}

ButtonStateStyle FilledButton::normalStyle() const { return {m_normalBackground, {}, m_normalContent}; }
void FilledButton::setNormalStyle(const ButtonStateStyle& style)
{
    m_normalBackground = style.background();
    m_normalContent = style.content();
    refreshButtonState();
}
ButtonStateStyle FilledButton::hoverStyle() const { return {m_hoverBackground, {}, m_hoverContent}; }
void FilledButton::setHoverStyle(const ButtonStateStyle& style)
{
    m_hoverBackground = style.background();
    m_hoverContent = style.content();
    refreshButtonState();
}
ButtonStateStyle FilledButton::pressedStyle() const { return {m_pressedBackground, {}, m_pressedContent}; }
void FilledButton::setPressedStyle(const ButtonStateStyle& style)
{
    m_pressedBackground = style.background();
    m_pressedContent = style.content();
    refreshButtonState();
}
ButtonStateStyle FilledButton::disabledStyle() const { return {m_disabledBackground, {}, m_disabledContent}; }
void FilledButton::setDisabledStyle(const ButtonStateStyle& style)
{
    m_disabledBackground = style.background();
    m_disabledContent = style.content();
    refreshButtonState();
}
ButtonStateStyle FilledButton::checkedStyle() const { return {m_checkedBackground, {}, m_checkedContent}; }
void FilledButton::setCheckedStyle(const ButtonStateStyle& style)
{
    m_checkedBackground = style.background();
    m_checkedContent = style.content();
    refreshButtonState();
}

QColor FilledButton::normalBackgroundColor() const { return m_normalBackground; }
void FilledButton::setNormalBackgroundColor(const QColor& color) { m_normalBackground = color; refreshButtonState(); }
QColor FilledButton::hoverBackgroundColor() const { return m_hoverBackground; }
void FilledButton::setHoverBackgroundColor(const QColor& color) { m_hoverBackground = color; refreshButtonState(); }
QColor FilledButton::pressedBackgroundColor() const { return m_pressedBackground; }
void FilledButton::setPressedBackgroundColor(const QColor& color) { m_pressedBackground = color; refreshButtonState(); }
QColor FilledButton::disabledBackgroundColor() const { return m_disabledBackground; }
void FilledButton::setDisabledBackgroundColor(const QColor& color) { m_disabledBackground = color; refreshButtonState(); }
QColor FilledButton::checkedBackgroundColor() const { return m_checkedBackground; }
void FilledButton::setCheckedBackgroundColor(const QColor& color) { m_checkedBackground = color; refreshButtonState(); }
QColor FilledButton::normalContentColor() const { return m_normalContent; }
void FilledButton::setNormalContentColor(const QColor& color) { m_normalContent = color; refreshButtonState(); }
QColor FilledButton::hoverContentColor() const { return m_hoverContent; }
void FilledButton::setHoverContentColor(const QColor& color) { m_hoverContent = color; refreshButtonState(); }
QColor FilledButton::pressedContentColor() const { return m_pressedContent; }
void FilledButton::setPressedContentColor(const QColor& color) { m_pressedContent = color; refreshButtonState(); }
QColor FilledButton::disabledContentColor() const { return m_disabledContent; }
void FilledButton::setDisabledContentColor(const QColor& color) { m_disabledContent = color; refreshButtonState(); }
QColor FilledButton::checkedContentColor() const { return m_checkedContent; }
void FilledButton::setCheckedContentColor(const QColor& color) { m_checkedContent = color; refreshButtonState(); }

int FilledButton::cornerRadius() const { return m_cornerRadius; }
void FilledButton::setCornerRadius(int radius) { m_cornerRadius = qMax(0, radius); update(); }
int FilledButton::iconSpacing() const { return m_iconSpacing; }
void FilledButton::setIconSpacing(int spacing) { m_iconSpacing = qMax(0, spacing); updateGeometry(); update(); }
FilledButton::IconPlacement FilledButton::iconPlacement() const { return m_iconPlacement; }
void FilledButton::setIconPlacement(IconPlacement placement) { m_iconPlacement = placement; updateGeometry(); update(); }

QColor FilledButton::backgroundForState(ButtonState state) const
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

QColor FilledButton::contentForState(ButtonState state) const
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

void FilledButton::updateAnimationTargets(bool immediate)
{
    const ButtonState state = buttonState();
    animateBackgroundColor(backgroundForState(state), immediate);
    const ButtonState contentState = animateContentColor() ? state : ButtonState::Normal;
    animateContentColor(contentForState(contentState), immediate);
}

QSize FilledButton::sizeHint() const
{
    ButtonContentOptions options{icon(), iconSize(), text(), font(), {}, m_iconSpacing, false,
                                 contentPlacement(m_iconPlacement)};
    const QSize content = buttonContentSize(options);
    return QSize(qMax(80, content.width() + 28), qMax(36, content.height() + 16));
}

void FilledButton::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    drawButtonSurface(painter, QRectF(rect()), ButtonSurfaceOptions{
        animatedBackgroundColor(), {}, 0.0, m_cornerRadius, true, false, false});
    drawButtonContent(painter, rect(), ButtonContentOptions{
        icon(), iconSize(), text(), font(), animatedContentColor(), m_iconSpacing, false,
        contentPlacement(m_iconPlacement)});
}

} // namespace animatedwidgets
