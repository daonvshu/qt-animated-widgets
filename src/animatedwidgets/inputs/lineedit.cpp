#include "animatedwidgets/inputs/lineedit.h"

#include <QEvent>
#include <QPainter>

LineEdit::LineEdit(QWidget* parent)
    : QLineEdit(parent), m_borderTransition(this), m_backgroundTransition(this)
{
    setFrame(false);
    setAttribute(Qt::WA_Hover, true);
    updateContentMargins();
    for (auto* transition : {&m_borderTransition, &m_backgroundTransition}) {
        transition->setDuration(m_animationDuration);
        transition->setEasingCurve(QEasingCurve::OutCubic);
        connect(transition, &animatedwidgets::ValueTransition::valueChanged,
                this, [this] { update(); });
    }
    updateTextPalette();
    refreshInputState(true);
}

bool LineEdit::hasError() const { return m_hasError; }
void LineEdit::setHasError(bool error)
{
    if (m_hasError == error) return;
    m_hasError = error;
    refreshInputState();
    emit hasErrorChanged(error);
}

QColor LineEdit::normalBorderColor() const { return m_normalBorderColor; }
void LineEdit::setNormalBorderColor(const QColor& color)
{
    m_normalBorderColor = color;
    refreshInputState();
}
QColor LineEdit::hoverBorderColor() const { return m_hoverBorderColor; }
void LineEdit::setHoverBorderColor(const QColor& color)
{
    m_hoverBorderColor = color;
    refreshInputState();
}
QColor LineEdit::focusBorderColor() const { return m_focusBorderColor; }
void LineEdit::setFocusBorderColor(const QColor& color)
{
    m_focusBorderColor = color;
    refreshInputState();
}
QColor LineEdit::errorBorderColor() const { return m_errorBorderColor; }
void LineEdit::setErrorBorderColor(const QColor& color)
{
    m_errorBorderColor = color;
    refreshInputState();
}
QColor LineEdit::disabledBorderColor() const { return m_disabledBorderColor; }
void LineEdit::setDisabledBorderColor(const QColor& color)
{
    m_disabledBorderColor = color;
    refreshInputState();
}
QColor LineEdit::normalBackgroundColor() const { return m_normalBackgroundColor; }
void LineEdit::setNormalBackgroundColor(const QColor& color)
{
    m_normalBackgroundColor = color;
    refreshInputState();
}
QColor LineEdit::hoverBackgroundColor() const { return m_hoverBackgroundColor; }
void LineEdit::setHoverBackgroundColor(const QColor& color)
{
    m_hoverBackgroundColor = color;
    refreshInputState();
}
QColor LineEdit::focusBackgroundColor() const { return m_focusBackgroundColor; }
void LineEdit::setFocusBackgroundColor(const QColor& color)
{
    m_focusBackgroundColor = color;
    refreshInputState();
}
QColor LineEdit::errorBackgroundColor() const { return m_errorBackgroundColor; }
void LineEdit::setErrorBackgroundColor(const QColor& color)
{
    m_errorBackgroundColor = color;
    refreshInputState();
}
QColor LineEdit::disabledBackgroundColor() const { return m_disabledBackgroundColor; }
void LineEdit::setDisabledBackgroundColor(const QColor& color)
{
    m_disabledBackgroundColor = color;
    refreshInputState();
}
QColor LineEdit::textColor() const { return m_textColor; }
void LineEdit::setTextColor(const QColor& color)
{
    m_textColor = color;
    updateTextPalette();
}
QColor LineEdit::disabledTextColor() const { return m_disabledTextColor; }
void LineEdit::setDisabledTextColor(const QColor& color)
{
    m_disabledTextColor = color;
    updateTextPalette();
}
int LineEdit::borderWidth() const { return m_borderWidth; }
void LineEdit::setBorderWidth(int width)
{
    m_borderWidth = qMax(0, width);
    update();
}
bool LineEdit::isBorderVisible() const { return m_borderVisible; }
void LineEdit::setBorderVisible(bool visible)
{
    if (m_borderVisible == visible) return;
    m_borderVisible = visible;
    update();
}
int LineEdit::cornerRadius() const { return m_cornerRadius; }
void LineEdit::setCornerRadius(int radius)
{
    m_cornerRadius = qMax(0, radius);
    update();
}
int LineEdit::contentPadding() const { return m_contentPadding; }
void LineEdit::setContentPadding(int padding)
{
    m_contentPadding = qMax(0, padding);
    updateContentMargins();
    updateGeometry();
}

void LineEdit::updateContentMargins()
{
    setTextMargins(m_contentPadding, 0, m_contentPadding, 0);
}
int LineEdit::animationDuration() const { return m_animationDuration; }
void LineEdit::setAnimationDuration(int duration)
{
    m_animationDuration = qMax(0, duration);
    m_borderTransition.setDuration(m_animationDuration);
    m_backgroundTransition.setDuration(m_animationDuration);
    if (m_animationDuration == 0) refreshInputState(true);
}

QSize LineEdit::sizeHint() const
{
    QSize hint = QLineEdit::sizeHint();
    hint.setHeight(qMax(hint.height(), 34));
    return hint;
}

QColor LineEdit::targetBorderColor() const
{
    if (!isEnabled()) return m_disabledBorderColor;
    if (m_hasError) return m_errorBorderColor;
    if (hasFocus()) return m_focusBorderColor;
    if (m_hovered) return m_hoverBorderColor;
    return m_normalBorderColor;
}

QColor LineEdit::targetBackgroundColor() const
{
    if (!isEnabled()) return m_disabledBackgroundColor;
    if (m_hasError) return m_errorBackgroundColor;
    if (hasFocus()) return m_focusBackgroundColor;
    if (m_hovered && m_hoverBackgroundColor.isValid()) return m_hoverBackgroundColor;
    return m_normalBackgroundColor;
}

void LineEdit::refreshInputState(bool immediate)
{
    m_borderTransition.setTarget(targetBorderColor(), immediate);
    m_backgroundTransition.setTarget(targetBackgroundColor(), immediate);
}

void LineEdit::updateTextPalette()
{
    QPalette colors = palette();
    const QColor transparent(0, 0, 0, 0);
    colors.setColor(QPalette::Active, QPalette::Base, transparent);
    colors.setColor(QPalette::Inactive, QPalette::Base, transparent);
    colors.setColor(QPalette::Disabled, QPalette::Base, transparent);
    colors.setColor(QPalette::Active, QPalette::Text, m_textColor);
    colors.setColor(QPalette::Inactive, QPalette::Text, m_textColor);
    colors.setColor(QPalette::Disabled, QPalette::Text, m_disabledTextColor);
    setPalette(colors);
}

bool LineEdit::event(QEvent* event)
{
    if (event->type() == QEvent::Enter) m_hovered = true;
    if (event->type() == QEvent::Leave) m_hovered = false;
    const bool handled = QLineEdit::event(event);
    switch (event->type()) {
    case QEvent::Enter:
    case QEvent::Leave:
    case QEvent::FocusIn:
    case QEvent::FocusOut:
    case QEvent::EnabledChange:
        refreshInputState();
        break;
    default:
        break;
    }
    return handled;
}

void LineEdit::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    const qreal inset = m_borderVisible ? m_borderWidth / 2.0 : 0.0;
    const QRectF frame = QRectF(rect()).adjusted(inset, inset, -inset, -inset);
    painter.setBrush(m_backgroundTransition.current().value<QColor>());
    painter.setPen(m_borderVisible && m_borderWidth > 0
        ? QPen(m_borderTransition.current().value<QColor>(), m_borderWidth)
        : QPen(Qt::NoPen));
    painter.drawRoundedRect(frame, m_cornerRadius, m_cornerRadius);
    painter.end();
    QLineEdit::paintEvent(event);
}
