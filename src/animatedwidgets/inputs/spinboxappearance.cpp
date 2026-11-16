#include "animatedwidgets/inputs/spinboxappearance.h"
#include "animatedwidgets/utils/inputcontextmenu.h"

#include <QAbstractSpinBox>
#include <QEvent>
#include <QLineEdit>
#include <QPainter>

#include "animatedwidgets/utils/paintutils.h"

namespace animatedwidgets {

SpinBoxAppearance::SpinBoxAppearance(QAbstractSpinBox* owner)
    : m_owner(owner)
    , m_borderTransition(owner)
    , m_backgroundTransition(owner)
{
    for (auto* transition : {&m_borderTransition, &m_backgroundTransition}) {
        transition->setDuration(m_animationDuration);
        transition->setEasingCurve(QEasingCurve::OutCubic);
        QObject::connect(transition, &ValueTransition::valueChanged, owner, [owner] { owner->update(); });
    }
    refresh(true);
}

void SpinBoxAppearance::setEditor(QLineEdit* editor) {
    m_editor = editor;
    InputContextMenu::install(editor, m_owner);
    m_editor->setFrame(false);
    m_editor->setAutoFillBackground(false);
    updateEditorPalette();
    updateEditorMargins();
}

void SpinBoxAppearance::handleEvent(QEvent* event) {
    if (event->type() == QEvent::Enter)
        m_hovered = true;
    if (event->type() == QEvent::Leave)
        m_hovered = false;
    switch (event->type()) {
        case QEvent::Enter:
        case QEvent::Leave:
        case QEvent::FocusIn:
        case QEvent::FocusOut:
        case QEvent::EnabledChange:
            refresh();
            break;
        default:
            break;
    }
}

void SpinBoxAppearance::paint(QPainter& painter) const {
    painter.setRenderHint(QPainter::Antialiasing);
    QRectF frame = QRectF(m_owner->rect());
    qreal strokeWidth = -1.0;
    if (m_borderVisible && m_borderWidth > 0) {
        const auto stroke = paintutils::borderStrokeForWidget(painter, QRectF(m_owner->rect()), m_borderWidth);
        frame = stroke.rect;
        strokeWidth = stroke.width;
    }
    painter.setBrush(m_backgroundTransition.current().value<QColor>());
    painter.setPen(strokeWidth >= 0
                           ? QPen(m_borderTransition.current().value<QColor>(), strokeWidth)
                           : QPen(Qt::NoPen));
    painter.drawRoundedRect(frame, m_cornerRadius, m_cornerRadius);
}

bool SpinBoxAppearance::hasError() const {
    return m_hasError;
}

void SpinBoxAppearance::setHasError(bool error) {
    if (m_hasError == error)
        return;
    m_hasError = error;
    refresh();
}

QColor SpinBoxAppearance::borderNormal() const {
    return m_normalBorderColor;
}

void SpinBoxAppearance::setBorderNormal(const QColor& color) {
    m_normalBorderColor = color;
    refresh();
}

QColor SpinBoxAppearance::borderHover() const {
    return m_hoverBorderColor;
}

void SpinBoxAppearance::setBorderHover(const QColor& color) {
    m_hoverBorderColor = color;
    refresh();
}

QColor SpinBoxAppearance::borderFocus() const {
    return m_focusBorderColor;
}

void SpinBoxAppearance::setBorderFocus(const QColor& color) {
    m_focusBorderColor = color;
    refresh();
}

QColor SpinBoxAppearance::borderError() const {
    return m_errorBorderColor;
}

void SpinBoxAppearance::setBorderError(const QColor& color) {
    m_errorBorderColor = color;
    refresh();
}

QColor SpinBoxAppearance::borderDisabled() const {
    return m_disabledBorderColor;
}

void SpinBoxAppearance::setBorderDisabled(const QColor& color) {
    m_disabledBorderColor = color;
    refresh();
}

QColor SpinBoxAppearance::backgroundNormal() const {
    return m_normalBackgroundColor;
}

void SpinBoxAppearance::setBackgroundNormal(const QColor& color) {
    m_normalBackgroundColor = color;
    refresh();
}

QColor SpinBoxAppearance::backgroundHover() const {
    return m_hoverBackgroundColor;
}

void SpinBoxAppearance::setBackgroundHover(const QColor& color) {
    m_hoverBackgroundColor = color;
    refresh();
}

QColor SpinBoxAppearance::backgroundFocus() const {
    return m_focusBackgroundColor;
}

void SpinBoxAppearance::setBackgroundFocus(const QColor& color) {
    m_focusBackgroundColor = color;
    refresh();
}

QColor SpinBoxAppearance::backgroundError() const {
    return m_errorBackgroundColor;
}

void SpinBoxAppearance::setBackgroundError(const QColor& color) {
    m_errorBackgroundColor = color;
    refresh();
}

QColor SpinBoxAppearance::backgroundDisabled() const {
    return m_disabledBackgroundColor;
}

void SpinBoxAppearance::setBackgroundDisabled(const QColor& color) {
    m_disabledBackgroundColor = color;
    refresh();
}

QColor SpinBoxAppearance::textColor() const {
    return m_textColor;
}

void SpinBoxAppearance::setTextColor(const QColor& color) {
    m_textColor = color;
    updateEditorPalette();
}

QColor SpinBoxAppearance::disabledTextColor() const {
    return m_disabledTextColor;
}

void SpinBoxAppearance::setDisabledTextColor(const QColor& color) {
    m_disabledTextColor = color;
    updateEditorPalette();
}

int SpinBoxAppearance::borderWidth() const {
    return m_borderWidth;
}

void SpinBoxAppearance::setBorderWidth(int width) {
    m_borderWidth = qMax(0, width);
    m_owner->update();
}

bool SpinBoxAppearance::isBorderVisible() const {
    return m_borderVisible;
}

void SpinBoxAppearance::setBorderVisible(bool visible) {
    m_borderVisible = visible;
    m_owner->update();
}

int SpinBoxAppearance::cornerRadius() const {
    return m_cornerRadius;
}

void SpinBoxAppearance::setCornerRadius(int radius) {
    m_cornerRadius = qMax(0, radius);
    m_owner->update();
}

int SpinBoxAppearance::contentPadding() const {
    return m_contentPadding;
}

void SpinBoxAppearance::setContentPadding(int padding) {
    m_contentPadding = qMax(0, padding);
    updateEditorMargins();
    m_owner->updateGeometry();
}

void SpinBoxAppearance::setTrailingControlWidth(int width) {
    m_trailingControlWidth = qMax(0, width);
    updateEditorMargins();
    m_owner->updateGeometry();
}

int SpinBoxAppearance::animationDuration() const {
    return m_animationDuration;
}

void SpinBoxAppearance::setAnimationDuration(int duration) {
    m_animationDuration = qMax(0, duration);
    m_borderTransition.setDuration(m_animationDuration);
    m_backgroundTransition.setDuration(m_animationDuration);
    if (m_animationDuration == 0)
        refresh(true);
}

QColor SpinBoxAppearance::targetBorderColor() const {
    if (!m_owner->isEnabled())
        return m_disabledBorderColor;
    if (m_hasError)
        return m_errorBorderColor;
    if (m_owner->hasFocus())
        return m_focusBorderColor;
    if (m_hovered)
        return m_hoverBorderColor;
    return m_normalBorderColor;
}

QColor SpinBoxAppearance::targetBackgroundColor() const {
    if (!m_owner->isEnabled())
        return m_disabledBackgroundColor;
    if (m_hasError)
        return m_errorBackgroundColor;
    if (m_owner->hasFocus())
        return m_focusBackgroundColor;
    if (m_hovered && m_hoverBackgroundColor.isValid())
        return m_hoverBackgroundColor;
    return m_normalBackgroundColor;
}

void SpinBoxAppearance::refresh(bool immediate) {
    m_borderTransition.setTarget(targetBorderColor(), immediate || !m_owner || !m_owner->isVisible());
    m_backgroundTransition.setTarget(targetBackgroundColor(), immediate || !m_owner || !m_owner->isVisible());
}

void SpinBoxAppearance::updateEditorPalette() {
    if (!m_editor)
        return;
    QPalette colors = m_editor->palette();
    const QColor transparent(0, 0, 0, 0);
    for (auto group : {QPalette::Active, QPalette::Inactive, QPalette::Disabled})
        colors.setColor(group, QPalette::Base, transparent);
    colors.setColor(QPalette::Active, QPalette::Text, m_textColor);
    colors.setColor(QPalette::Inactive, QPalette::Text, m_textColor);
    colors.setColor(QPalette::Disabled, QPalette::Text, m_disabledTextColor);
    m_editor->setPalette(colors);
}

void SpinBoxAppearance::updateEditorMargins() {
    if (m_editor)
        m_editor->setTextMargins(m_contentPadding, 0, m_trailingControlWidth + m_contentPadding, 0);
}

} // namespace animatedwidgets
