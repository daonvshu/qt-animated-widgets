#include "animatedwidgets/buttons/borderedbutton.h"

#include "animatedwidgets/utils/buttonpainter.h"
#include "animatedwidgets/utils/buttonstatecolors.h"

#include <QPainter>

namespace animatedwidgets {

BorderedButton::BorderedButton(QWidget* parent) : AnimatedButtonBase(parent)
{
    refreshButtonState(true);
}

ButtonStateStyle BorderedButton::normalStyle() const { return {m_normalBackground, m_normalBorder, m_normalContent}; }
void BorderedButton::setNormalStyle(const ButtonStateStyle& style)
{
    m_normalBackground = style.background();
    m_normalBorder = style.border();
    m_normalContent = style.content();
    refreshButtonState();
}
ButtonStateStyle BorderedButton::hoverStyle() const { return {m_hoverBackground, m_hoverBorder, m_hoverContent}; }
void BorderedButton::setHoverStyle(const ButtonStateStyle& style)
{
    m_hoverBackground = style.background();
    m_hoverBorder = style.border();
    m_hoverContent = style.content();
    refreshButtonState();
}
ButtonStateStyle BorderedButton::pressedStyle() const { return {m_pressedBackground, m_pressedBorder, m_pressedContent}; }
void BorderedButton::setPressedStyle(const ButtonStateStyle& style)
{
    m_pressedBackground = style.background();
    m_pressedBorder = style.border();
    m_pressedContent = style.content();
    refreshButtonState();
}
ButtonStateStyle BorderedButton::disabledStyle() const { return {m_disabledBackground, m_disabledBorder, m_disabledContent}; }
void BorderedButton::setDisabledStyle(const ButtonStateStyle& style)
{
    m_disabledBackground = style.background();
    m_disabledBorder = style.border();
    m_disabledContent = style.content();
    refreshButtonState();
}
ButtonStateStyle BorderedButton::checkedStyle() const { return {m_checkedBackground, m_checkedBorder, m_checkedContent}; }
void BorderedButton::setCheckedStyle(const ButtonStateStyle& style)
{
    m_checkedBackground = style.background();
    m_checkedBorder = style.border();
    m_checkedContent = style.content();
    refreshButtonState();
}

QColor BorderedButton::normalBackgroundColor() const { return m_normalBackground; }
void BorderedButton::setNormalBackgroundColor(const QColor& color) { m_normalBackground = color; refreshButtonState(); }
QColor BorderedButton::hoverBackgroundColor() const { return m_hoverBackground; }
void BorderedButton::setHoverBackgroundColor(const QColor& color) { m_hoverBackground = color; refreshButtonState(); }
QColor BorderedButton::pressedBackgroundColor() const { return m_pressedBackground; }
void BorderedButton::setPressedBackgroundColor(const QColor& color) { m_pressedBackground = color; refreshButtonState(); }
QColor BorderedButton::disabledBackgroundColor() const { return m_disabledBackground; }
void BorderedButton::setDisabledBackgroundColor(const QColor& color) { m_disabledBackground = color; refreshButtonState(); }
QColor BorderedButton::checkedBackgroundColor() const { return m_checkedBackground; }
void BorderedButton::setCheckedBackgroundColor(const QColor& color) { m_checkedBackground = color; refreshButtonState(); }
QColor BorderedButton::normalBorderColor() const { return m_normalBorder; }
void BorderedButton::setNormalBorderColor(const QColor& color) { m_normalBorder = color; refreshButtonState(); }
QColor BorderedButton::hoverBorderColor() const { return m_hoverBorder; }
void BorderedButton::setHoverBorderColor(const QColor& color) { m_hoverBorder = color; refreshButtonState(); }
QColor BorderedButton::pressedBorderColor() const { return m_pressedBorder; }
void BorderedButton::setPressedBorderColor(const QColor& color) { m_pressedBorder = color; refreshButtonState(); }
QColor BorderedButton::disabledBorderColor() const { return m_disabledBorder; }
void BorderedButton::setDisabledBorderColor(const QColor& color) { m_disabledBorder = color; refreshButtonState(); }
QColor BorderedButton::checkedBorderColor() const { return m_checkedBorder; }
void BorderedButton::setCheckedBorderColor(const QColor& color) { m_checkedBorder = color; refreshButtonState(); }
QColor BorderedButton::normalContentColor() const { return m_normalContent; }
void BorderedButton::setNormalContentColor(const QColor& color) { m_normalContent = color; refreshButtonState(); }
QColor BorderedButton::hoverContentColor() const { return m_hoverContent; }
void BorderedButton::setHoverContentColor(const QColor& color) { m_hoverContent = color; refreshButtonState(); }
QColor BorderedButton::pressedContentColor() const { return m_pressedContent; }
void BorderedButton::setPressedContentColor(const QColor& color) { m_pressedContent = color; refreshButtonState(); }
QColor BorderedButton::disabledContentColor() const { return m_disabledContent; }
void BorderedButton::setDisabledContentColor(const QColor& color) { m_disabledContent = color; refreshButtonState(); }
QColor BorderedButton::checkedContentColor() const { return m_checkedContent; }
void BorderedButton::setCheckedContentColor(const QColor& color) { m_checkedContent = color; refreshButtonState(); }

int BorderedButton::cornerRadius() const { return m_cornerRadius; }
void BorderedButton::setCornerRadius(int radius) { m_cornerRadius = qMax(0, radius); update(); }
qreal BorderedButton::borderWidth() const { return m_borderWidth; }
void BorderedButton::setBorderWidth(qreal width) { m_borderWidth = qMax(qreal(0), width); update(); }
int BorderedButton::iconSpacing() const { return m_iconSpacing; }
void BorderedButton::setIconSpacing(int spacing) { m_iconSpacing = qMax(0, spacing); updateGeometry(); update(); }
BorderedButton::IconPlacement BorderedButton::iconPlacement() const { return m_iconPlacement; }
void BorderedButton::setIconPlacement(IconPlacement placement) { m_iconPlacement = placement; updateGeometry(); update(); }

QColor BorderedButton::backgroundForState(ButtonState state) const
{
    return ButtonStateColorUtils::resolve({m_normalBackground, m_hoverBackground, m_pressedBackground,
                       m_checkedBackground, m_disabledBackground}, state, darkerStep());
}

QColor BorderedButton::borderForState(ButtonState state) const
{
    return ButtonStateColorUtils::resolve({m_normalBorder, m_hoverBorder, m_pressedBorder,
                       m_checkedBorder, m_disabledBorder}, state, darkerStep());
}

QColor BorderedButton::contentForState(ButtonState state) const
{
    return ButtonStateColorUtils::resolve({m_normalContent, m_hoverContent, m_pressedContent,
                       m_checkedContent, m_disabledContent}, state, darkerStep());
}

void BorderedButton::updateAnimationTargets(bool immediate)
{
    const ButtonState state = buttonState();
    animateBackgroundColor(backgroundForState(state), immediate);
    animateBorderColor(borderForState(state), immediate);
    const ButtonState contentState = animateContentColor() ? state : ButtonState::Normal;
    animateContentColor(contentForState(contentState), immediate);
}

QSize BorderedButton::sizeHint() const
{
    const QSize content = ButtonPainter::contentSize(ButtonContentOptions{
        icon(), iconSize(), text(), font(), {}, m_iconSpacing, false,
        contentPlacement(m_iconPlacement)});
    return ButtonPainter::textSizeHint(content);
}

void BorderedButton::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    const qreal inset = m_borderWidth / 2;
    ButtonPainter::drawSurface(painter, QRectF(rect()).adjusted(inset, inset, -inset, -inset),
                      ButtonSurfaceOptions{animatedBackgroundColor(), animatedBorderColor(),
                                           m_borderWidth, m_cornerRadius, true, true, false});
    ButtonPainter::drawContent(painter, rect(), ButtonContentOptions{
        icon(), iconSize(), text(), font(), animatedContentColor(), m_iconSpacing, false,
        contentPlacement(m_iconPlacement)});
}

} // namespace animatedwidgets
