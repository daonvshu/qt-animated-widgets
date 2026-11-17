#include "animatedwidgets/inputs/lineedit.h"
#include "animatedwidgets/utils/inputcontextmenu.h"

#include <QEvent>
#include <QPainter>

#include "animatedwidgets/utils/paintutils.h"

namespace animatedwidgets {

LineEdit::LineEdit(QWidget* parent)
    : QLineEdit(parent)
    , m_borderTransition(this)
    , m_backgroundTransition(this)
{
    setFrame(false);
    setFocusPolicy(Qt::ClickFocus);
    setAttribute(Qt::WA_Hover, true);
    LineEdit::updateContentMargins();
    InputContextMenu::install(this);
    connect(this, &QLineEdit::returnPressed, this, &QLineEdit::clearFocus);
    for (auto* transition : {&m_borderTransition, &m_backgroundTransition}) {
        transition->setDuration(m_animationDuration);
        transition->setEasingCurve(QEasingCurve::OutCubic);
        connect(transition, &ValueTransition::valueChanged, this, [this] { update(); });
    }
    updateTextPalette();
    refreshInputState(true);
}

bool LineEdit::hasError() const {
    return m_hasError;
}

void LineEdit::setHasError(bool error) {
    if (m_hasError == error)
        return;
    m_hasError = error;
    refreshInputState();
    emit hasErrorChanged(error);
}

QColor LineEdit::borderNormal() const {
    return m_normalBorderColor;
}

void LineEdit::setBorderNormal(const QColor& color) {
    m_normalBorderColor = color;
    refreshInputState();
}

QColor LineEdit::borderHover() const {
    return m_hoverBorderColor;
}

void LineEdit::setBorderHover(const QColor& color) {
    m_hoverBorderColor = color;
    refreshInputState();
}

QColor LineEdit::borderFocus() const {
    return m_focusBorderColor;
}

void LineEdit::setBorderFocus(const QColor& color) {
    m_focusBorderColor = color;
    refreshInputState();
}

QColor LineEdit::borderError() const {
    return m_errorBorderColor;
}

void LineEdit::setBorderError(const QColor& color) {
    m_errorBorderColor = color;
    refreshInputState();
}

QColor LineEdit::borderDisabled() const {
    return m_disabledBorderColor;
}

void LineEdit::setBorderDisabled(const QColor& color) {
    m_disabledBorderColor = color;
    refreshInputState();
}

QColor LineEdit::backgroundNormal() const {
    return m_normalBackgroundColor;
}

void LineEdit::setBackgroundNormal(const QColor& color) {
    m_normalBackgroundColor = color;
    refreshInputState();
}

QColor LineEdit::backgroundHover() const {
    return m_hoverBackgroundColor;
}

void LineEdit::setBackgroundHover(const QColor& color) {
    m_hoverBackgroundColor = color;
    refreshInputState();
}

QColor LineEdit::backgroundFocus() const {
    return m_focusBackgroundColor;
}

void LineEdit::setBackgroundFocus(const QColor& color) {
    m_focusBackgroundColor = color;
    refreshInputState();
}

QColor LineEdit::backgroundError() const {
    return m_errorBackgroundColor;
}

void LineEdit::setBackgroundError(const QColor& color) {
    m_errorBackgroundColor = color;
    refreshInputState();
}

QColor LineEdit::backgroundDisabled() const {
    return m_disabledBackgroundColor;
}

void LineEdit::setBackgroundDisabled(const QColor& color) {
    m_disabledBackgroundColor = color;
    refreshInputState();
}

QColor LineEdit::textColor() const {
    return m_textColor;
}

void LineEdit::setTextColor(const QColor& color) {
    m_textColor = color;
    updateTextPalette();
}

QColor LineEdit::disabledTextColor() const {
    return m_disabledTextColor;
}

void LineEdit::setDisabledTextColor(const QColor& color) {
    m_disabledTextColor = color;
    updateTextPalette();
}

int LineEdit::borderWidth() const {
    return m_borderWidth;
}

void LineEdit::setBorderWidth(int width) {
    m_borderWidth = qMax(0, width);
    update();
}

bool LineEdit::isBorderVisible() const {
    return m_borderVisible;
}

void LineEdit::setBorderVisible(bool visible) {
    if (m_borderVisible == visible)
        return;
    m_borderVisible = visible;
    update();
}

int LineEdit::cornerRadius() const {
    return m_cornerRadius;
}

void LineEdit::setCornerRadius(int radius) {
    m_cornerRadius = qMax(0, radius);
    update();
}

int LineEdit::contentPadding() const {
    return m_contentPadding;
}

void LineEdit::setContentPadding(int padding) {
    m_contentPadding = qMax(0, padding);
    updateContentMargins();
    updateGeometry();
}

void LineEdit::updateContentMargins() {
    setTextMargins(m_contentPadding, 0, m_contentPadding, 0);
}

int LineEdit::animationDuration() const {
    return m_animationDuration;
}

void LineEdit::setAnimationDuration(int duration) {
    m_animationDuration = qMax(0, duration);
    m_borderTransition.setDuration(m_animationDuration);
    m_backgroundTransition.setDuration(m_animationDuration);
    if (m_animationDuration == 0)
        refreshInputState(true);
}

QSize LineEdit::sizeHint() const {
    QSize hint = QLineEdit::sizeHint();
    hint.setHeight(qMax(hint.height(), 34));
    return hint;
}

QColor LineEdit::targetBorderColor() const {
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

QColor LineEdit::targetBackgroundColor() const {
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

void LineEdit::refreshInputState(bool immediate) {
    m_borderTransition.setTarget(targetBorderColor(), immediate || !isVisible());
    m_backgroundTransition.setTarget(targetBackgroundColor(), immediate || !isVisible());
}

void LineEdit::updateTextPalette() {
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

bool LineEdit::event(QEvent* event) {
    if (event->type() == QEvent::Enter)
        m_hovered = true;
    if (event->type() == QEvent::Leave)
        m_hovered = false;
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

void LineEdit::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QRectF frame = QRectF(rect());
    qreal strokeWidth = -1.0;
    if (m_borderVisible && m_borderWidth > 0) {
        const auto stroke = paintutils::borderStrokeForWidget(painter, QRectF(rect()), m_borderWidth);
        frame = stroke.rect;
        strokeWidth = stroke.width;
    }
    painter.setBrush(m_backgroundTransition.current().value<QColor>());
    painter.setPen(strokeWidth >= 0
                           ? QPen(m_borderTransition.current().value<QColor>(), strokeWidth)
                           : QPen(Qt::NoPen));
    painter.drawRoundedRect(frame, m_cornerRadius, m_cornerRadius);
    painter.end();
    QLineEdit::paintEvent(event);
}

} // namespace animatedwidgets
