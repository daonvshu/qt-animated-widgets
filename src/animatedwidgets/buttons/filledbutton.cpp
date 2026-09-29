#include "animatedwidgets/buttons/filledbutton.h"

#include "animatedwidgets/utils/buttonpainter.h"
#include "animatedwidgets/utils/buttonstatecolors.h"

#include <QPainter>

namespace animatedwidgets {

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
    return ButtonStateColorUtils::resolve({m_normalBackground, m_hoverBackground, m_pressedBackground,
                       m_checkedBackground, m_disabledBackground}, state, darkerStep());
}

QColor FilledButton::contentForState(ButtonState state) const
{
    return ButtonStateColorUtils::resolve({m_normalContent, m_hoverContent, m_pressedContent,
                       m_checkedContent, m_disabledContent}, state, darkerStep());
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
    const QSize content = ButtonPainter::contentSize(ButtonContentOptions{
        icon(), iconSize(), text(), font(), {}, m_iconSpacing, false,
        contentPlacement(m_iconPlacement)});
    return ButtonPainter::textSizeHint(content);
}

void FilledButton::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    ButtonPainter::drawSurface(painter, QRectF(rect()), ButtonSurfaceOptions{
        animatedBackgroundColor(), {}, 0.0, m_cornerRadius, true, false, false});
    ButtonPainter::drawContent(painter, rect(), ButtonContentOptions{
        icon(), iconSize(), text(), font(), animatedContentColor(), m_iconSpacing, false,
        contentPlacement(m_iconPlacement)});
}

} // namespace animatedwidgets
