#include "animatedwidgets/inputs/plaintextedit.h"
#include "animatedwidgets/utils/inputcontextmenu.h"

#include <QEvent>
#include <QPainter>
#include <QTextDocument>

namespace animatedwidgets {

PlainTextEdit::PlainTextEdit(QWidget* parent)
    : QPlainTextEdit(parent)
    , m_borderTransition(this)
    , m_backgroundTransition(this)
{
    setFrameShape(QFrame::NoFrame);
    setFocusPolicy(Qt::ClickFocus);
    setAttribute(Qt::WA_Hover, true);
    setAutoFillBackground(false);
    viewport()->setAutoFillBackground(false);
    viewport()->setAttribute(Qt::WA_OpaquePaintEvent, false);
    updateContentMargins();
    InputContextMenu::install(this);
    for (auto* transition : {&m_borderTransition, &m_backgroundTransition}) {
        transition->setDuration(m_animationDuration);
        transition->setEasingCurve(QEasingCurve::OutCubic);
        connect(transition, &ValueTransition::valueChanged, this, [this] {
            update();
            viewport()->update();
        });
    }
    updateTextPalette();
    refreshInputState(true);
}

bool PlainTextEdit::hasError() const {
    return m_hasError;
}

void PlainTextEdit::setHasError(bool error) {
    if (m_hasError == error)
        return;
    m_hasError = error;
    refreshInputState();
    emit hasErrorChanged(error);
}

QColor PlainTextEdit::borderNormal() const {
    return m_normalBorderColor;
}

void PlainTextEdit::setBorderNormal(const QColor& color) {
    m_normalBorderColor = color;
    refreshInputState();
}

QColor PlainTextEdit::borderHover() const {
    return m_hoverBorderColor;
}

void PlainTextEdit::setBorderHover(const QColor& color) {
    m_hoverBorderColor = color;
    refreshInputState();
}

QColor PlainTextEdit::borderFocus() const {
    return m_focusBorderColor;
}

void PlainTextEdit::setBorderFocus(const QColor& color) {
    m_focusBorderColor = color;
    refreshInputState();
}

QColor PlainTextEdit::borderError() const {
    return m_errorBorderColor;
}

void PlainTextEdit::setBorderError(const QColor& color) {
    m_errorBorderColor = color;
    refreshInputState();
}

QColor PlainTextEdit::borderDisabled() const {
    return m_disabledBorderColor;
}

void PlainTextEdit::setBorderDisabled(const QColor& color) {
    m_disabledBorderColor = color;
    refreshInputState();
}

QColor PlainTextEdit::backgroundNormal() const {
    return m_normalBackgroundColor;
}

void PlainTextEdit::setBackgroundNormal(const QColor& color) {
    m_normalBackgroundColor = color;
    refreshInputState();
}

QColor PlainTextEdit::backgroundHover() const {
    return m_hoverBackgroundColor;
}

void PlainTextEdit::setBackgroundHover(const QColor& color) {
    m_hoverBackgroundColor = color;
    refreshInputState();
}

QColor PlainTextEdit::backgroundFocus() const {
    return m_focusBackgroundColor;
}

void PlainTextEdit::setBackgroundFocus(const QColor& color) {
    m_focusBackgroundColor = color;
    refreshInputState();
}

QColor PlainTextEdit::backgroundError() const {
    return m_errorBackgroundColor;
}

void PlainTextEdit::setBackgroundError(const QColor& color) {
    m_errorBackgroundColor = color;
    refreshInputState();
}

QColor PlainTextEdit::backgroundDisabled() const {
    return m_disabledBackgroundColor;
}

void PlainTextEdit::setBackgroundDisabled(const QColor& color) {
    m_disabledBackgroundColor = color;
    refreshInputState();
}

QColor PlainTextEdit::textColor() const {
    return m_textColor;
}

void PlainTextEdit::setTextColor(const QColor& color) {
    m_textColor = color;
    updateTextPalette();
}

QColor PlainTextEdit::disabledTextColor() const {
    return m_disabledTextColor;
}

void PlainTextEdit::setDisabledTextColor(const QColor& color) {
    m_disabledTextColor = color;
    updateTextPalette();
}

int PlainTextEdit::borderWidth() const {
    return m_borderWidth;
}

void PlainTextEdit::setBorderWidth(int width) {
    m_borderWidth = qMax(0, width);
    updateContentMargins();
    update();
}

bool PlainTextEdit::isBorderVisible() const {
    return m_borderVisible;
}

void PlainTextEdit::setBorderVisible(bool visible) {
    if (m_borderVisible == visible)
        return;
    m_borderVisible = visible;
    updateContentMargins();
    update();
}

int PlainTextEdit::cornerRadius() const {
    return m_cornerRadius;
}

void PlainTextEdit::setCornerRadius(int radius) {
    m_cornerRadius = qMax(0, radius);
    update();
}

int PlainTextEdit::contentPadding() const {
    return m_contentPadding;
}

void PlainTextEdit::setContentPadding(int padding) {
    m_contentPadding = qMax(0, padding);
    updateContentMargins();
    updateGeometry();
}

int PlainTextEdit::animationDuration() const {
    return m_animationDuration;
}

void PlainTextEdit::setAnimationDuration(int duration) {
    m_animationDuration = qMax(0, duration);
    m_borderTransition.setDuration(m_animationDuration);
    m_backgroundTransition.setDuration(m_animationDuration);
    if (m_animationDuration == 0)
        refreshInputState(true);
}

QSize PlainTextEdit::sizeHint() const {
    QSize hint = QPlainTextEdit::sizeHint();
    hint.setHeight(qMax(hint.height(), 108));
    return hint;
}

QColor PlainTextEdit::targetBorderColor() const {
    if (!isEnabled())
        return m_disabledBorderColor;
    if (m_hasError)
        return m_errorBorderColor;
    if (hasFocus())
        return m_focusBorderColor;
    if (m_hovered)
        return m_hoverBorderColor;
    return m_normalBorderColor;
}

QColor PlainTextEdit::targetBackgroundColor() const {
    if (!isEnabled())
        return m_disabledBackgroundColor;
    if (m_hasError)
        return m_errorBackgroundColor;
    if (hasFocus())
        return m_focusBackgroundColor;
    if (m_hovered && m_hoverBackgroundColor.isValid())
        return m_hoverBackgroundColor;
    return m_normalBackgroundColor;
}

void PlainTextEdit::refreshInputState(bool immediate) {
    m_borderTransition.setTarget(targetBorderColor(), immediate);
    m_backgroundTransition.setTarget(targetBackgroundColor(), immediate);
}

void PlainTextEdit::updateTextPalette() {
    QPalette colors = palette();
    const QColor transparent(0, 0, 0, 0);
    for (auto group : {QPalette::Active, QPalette::Inactive, QPalette::Disabled})
        colors.setColor(group, QPalette::Base, transparent);
    colors.setColor(QPalette::Active, QPalette::Text, m_textColor);
    colors.setColor(QPalette::Inactive, QPalette::Text, m_textColor);
    colors.setColor(QPalette::Disabled, QPalette::Text, m_disabledTextColor);
    setPalette(colors);
    viewport()->setPalette(colors);
}

void PlainTextEdit::updateContentMargins() {
    const int margin = m_contentPadding + (m_borderVisible ? m_borderWidth : 0);
    setViewportMargins(margin, margin, margin, margin);
    document()->setDocumentMargin(0);
}

bool PlainTextEdit::event(QEvent* event) {
    if (event->type() == QEvent::Enter)
        m_hovered = true;
    if (event->type() == QEvent::Leave)
        m_hovered = false;
    const bool handled = QPlainTextEdit::event(event);
    if (event->type() == QEvent::Paint) {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        const qreal inset = m_borderVisible ? m_borderWidth / 2.0 : 0.0;
        const QRectF frame = QRectF(rect()).adjusted(inset, inset, -inset, -inset);
        painter.setBrush(m_backgroundTransition.current().value<QColor>());
        painter.setPen(m_borderVisible && m_borderWidth > 0 ? QPen(m_borderTransition.current().value<QColor>(), m_borderWidth)
                                                           : QPen(Qt::NoPen));
        painter.drawRoundedRect(frame, m_cornerRadius, m_cornerRadius);
    }
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

void PlainTextEdit::paintEvent(QPaintEvent* event) {
    QPainter painter(viewport());
    painter.fillRect(event->rect(), m_backgroundTransition.current().value<QColor>());
    painter.end();
    QPlainTextEdit::paintEvent(event);
}

} // namespace animatedwidgets
