#include "animatedwidgets/comboboxes/combobox.h"

#include <QAbstractItemView>
#include <QApplication>
#include <QEvent>
#include <QLineEdit>
#include <QPainter>

namespace animatedwidgets {

ComboBox::ComboBox(QWidget* parent)
    : QComboBox(parent)
    , m_borderTransition(this)
    , m_backgroundTransition(this)
{
    setFrame(false);
    setFocusPolicy(Qt::ClickFocus);
    setAttribute(Qt::WA_Hover, true);
    Q_INIT_RESOURCE(comboboxes);
    m_arrowIcon = QIcon(QStringLiteral(":/animatedwidgets/comboboxes/dropdown.svg"));

    for (auto* transition : {&m_borderTransition, &m_backgroundTransition}) {
        transition->setDuration(m_animationDuration);
        transition->setEasingCurve(QEasingCurve::OutCubic);
        connect(transition, &ValueTransition::valueChanged, this, [this] { update(); });
    }
    connect(qApp, &QApplication::focusChanged, this, [this](QWidget* old, QWidget* current) {
        if (old == this || current == this || (old && isAncestorOf(old)) || (current && isAncestorOf(current)))
            refreshState();
    });
    refreshState(true);
}

bool ComboBox::hasError() const {
    return m_hasError;
}

void ComboBox::setHasError(bool error) {
    if (m_hasError == error)
        return;
    m_hasError = error;
    refreshState();
    emit hasErrorChanged(error);
}

QColor ComboBox::borderNormal() const {
    return m_normalBorderColor;
}

void ComboBox::setBorderNormal(const QColor& color) {
    m_normalBorderColor = color;
    refreshState();
}

QColor ComboBox::borderHover() const {
    return m_hoverBorderColor;
}

void ComboBox::setBorderHover(const QColor& color) {
    m_hoverBorderColor = color;
    refreshState();
}

QColor ComboBox::borderFocus() const {
    return m_focusBorderColor;
}

void ComboBox::setBorderFocus(const QColor& color) {
    m_focusBorderColor = color;
    refreshState();
}

QColor ComboBox::borderError() const {
    return m_errorBorderColor;
}

void ComboBox::setBorderError(const QColor& color) {
    m_errorBorderColor = color;
    refreshState();
}

QColor ComboBox::borderDisabled() const {
    return m_disabledBorderColor;
}

void ComboBox::setBorderDisabled(const QColor& color) {
    m_disabledBorderColor = color;
    refreshState();
}

QColor ComboBox::backgroundNormal() const {
    return m_normalBackgroundColor;
}

void ComboBox::setBackgroundNormal(const QColor& color) {
    m_normalBackgroundColor = color;
    refreshState();
}

QColor ComboBox::backgroundHover() const {
    return m_hoverBackgroundColor;
}

void ComboBox::setBackgroundHover(const QColor& color) {
    m_hoverBackgroundColor = color;
    refreshState();
}

QColor ComboBox::backgroundFocus() const {
    return m_focusBackgroundColor;
}

void ComboBox::setBackgroundFocus(const QColor& color) {
    m_focusBackgroundColor = color;
    refreshState();
}

QColor ComboBox::backgroundError() const {
    return m_errorBackgroundColor;
}

void ComboBox::setBackgroundError(const QColor& color) {
    m_errorBackgroundColor = color;
    refreshState();
}

QColor ComboBox::backgroundDisabled() const {
    return m_disabledBackgroundColor;
}

void ComboBox::setBackgroundDisabled(const QColor& color) {
    m_disabledBackgroundColor = color;
    refreshState();
}

QColor ComboBox::textColor() const {
    return m_textColor;
}

void ComboBox::setTextColor(const QColor& color) {
    m_textColor = color;
    updateEditorPalette();
    update();
}

QColor ComboBox::disabledTextColor() const {
    return m_disabledTextColor;
}

void ComboBox::setDisabledTextColor(const QColor& color) {
    m_disabledTextColor = color;
    updateEditorPalette();
    update();
}

int ComboBox::borderWidth() const {
    return m_borderWidth;
}

void ComboBox::setBorderWidth(int width) {
    m_borderWidth = qMax(0, width);
    update();
}

bool ComboBox::isBorderVisible() const {
    return m_borderVisible;
}

void ComboBox::setBorderVisible(bool visible) {
    m_borderVisible = visible;
    update();
}

int ComboBox::cornerRadius() const {
    return m_cornerRadius;
}

void ComboBox::setCornerRadius(int radius) {
    m_cornerRadius = qMax(0, radius);
    update();
}

int ComboBox::contentPadding() const {
    return m_contentPadding;
}

void ComboBox::setContentPadding(int padding) {
    m_contentPadding = qMax(0, padding);
    updateEditorMargins();
    updateGeometry();
    update();
}

int ComboBox::animationDuration() const {
    return m_animationDuration;
}

void ComboBox::setAnimationDuration(int duration) {
    m_animationDuration = qMax(0, duration);
    m_borderTransition.setDuration(m_animationDuration);
    m_backgroundTransition.setDuration(m_animationDuration);
    if (m_animationDuration == 0)
        refreshState(true);
}

QIcon ComboBox::arrowIcon() const {
    return m_arrowIcon;
}

void ComboBox::setArrowIcon(const QIcon& icon) {
    m_arrowIcon = icon;
    update();
}

QSize ComboBox::arrowIconSize() const {
    return m_arrowIconSize;
}

void ComboBox::setArrowIconSize(const QSize& size) {
    m_arrowIconSize = QSize(qMax(1, size.width()), qMax(1, size.height()));
    updateEditorMargins();
    updateGeometry();
    update();
}

int ComboBox::arrowRightMargin() const {
    return m_arrowRightMargin;
}

void ComboBox::setArrowRightMargin(int margin) {
    m_arrowRightMargin = qMax(0, margin);
    updateEditorMargins();
    updateGeometry();
    update();
}

bool ComboBox::arrowVisible() const {
    return m_arrowVisible;
}

void ComboBox::setArrowVisible(bool visible) {
    m_arrowVisible = visible;
    updateEditorMargins();
    updateGeometry();
    update();
}

QSize ComboBox::sizeHint() const {
    QSize hint = QComboBox::sizeHint();
    hint.setWidth(qMax(hint.width(), 120));
    hint.setHeight(qMax(hint.height(), 34));
    return hint;
}

QColor ComboBox::targetBorderColor() const {
    if (!isEnabled())
        return m_disabledBorderColor;
    if (m_hasError)
        return m_errorBorderColor;
    QWidget* focused = QApplication::focusWidget();
    if (m_popupOpen || hasFocus() || (focused && isAncestorOf(focused)))
        return m_focusBorderColor;
    if (m_hovered)
        return m_hoverBorderColor;
    return m_normalBorderColor;
}

QColor ComboBox::targetBackgroundColor() const {
    if (!isEnabled())
        return m_disabledBackgroundColor;
    if (m_hasError)
        return m_errorBackgroundColor;
    QWidget* focused = QApplication::focusWidget();
    if (m_popupOpen || hasFocus() || (focused && isAncestorOf(focused)))
        return m_focusBackgroundColor;
    if (m_hovered && m_hoverBackgroundColor.isValid())
        return m_hoverBackgroundColor;
    return m_normalBackgroundColor;
}

void ComboBox::refreshState(bool immediate) {
    m_borderTransition.setTarget(targetBorderColor(), immediate);
    m_backgroundTransition.setTarget(targetBackgroundColor(), immediate);
}

void ComboBox::syncEditor() {
    QLineEdit* editor = lineEdit();
    if (editor == m_editor)
        return;
    m_editor = editor;
    if (!m_editor)
        return;
    m_editor->setFrame(false);
    m_editor->setAutoFillBackground(false);
    updateEditorPalette();
    updateEditorMargins();
}

void ComboBox::updateEditorPalette() {
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

void ComboBox::updateEditorMargins() {
    if (!m_editor)
        return;
    const int arrowExtra = m_arrowVisible ? qMax(0, m_arrowIconSize.width() + m_arrowRightMargin + 8 - 24) : 0;
    if (layoutDirection() == Qt::RightToLeft)
        m_editor->setTextMargins(m_contentPadding + arrowExtra, 0, m_contentPadding, 0);
    else
        m_editor->setTextMargins(m_contentPadding, 0, m_contentPadding + arrowExtra, 0);
}

bool ComboBox::event(QEvent* event) {
    if (event->type() == QEvent::Enter)
        m_hovered = true;
    if (event->type() == QEvent::Leave)
        m_hovered = false;

    const bool handled = QComboBox::event(event);
    switch (event->type()) {
        case QEvent::Enter:
        case QEvent::Leave:
        case QEvent::FocusIn:
        case QEvent::FocusOut:
        case QEvent::EnabledChange:
            refreshState();
            break;
        case QEvent::LayoutDirectionChange:
            updateEditorMargins();
            break;
        default:
            break;
    }
    return handled;
}

void ComboBox::paintEvent(QPaintEvent*) {
    syncEditor();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    const qreal inset = m_borderVisible ? m_borderWidth / 2.0 : 0.0;
    const QRectF frame = QRectF(rect()).adjusted(inset, inset, -inset, -inset);
    painter.setBrush(m_backgroundTransition.current().value<QColor>());
    painter.setPen(m_borderVisible && m_borderWidth > 0 ? QPen(m_borderTransition.current().value<QColor>(), m_borderWidth) : QPen(Qt::NoPen));
    painter.drawRoundedRect(frame, m_cornerRadius, m_cornerRadius);

    const bool rightToLeft = layoutDirection() == Qt::RightToLeft;
    if (m_arrowVisible && !m_arrowIcon.isNull()) {
        const int arrowX = rightToLeft ? m_arrowRightMargin : width() - m_arrowRightMargin - m_arrowIconSize.width();
        const QRect arrowRect(arrowX, (height() - m_arrowIconSize.height()) / 2, m_arrowIconSize.width(), m_arrowIconSize.height());
        m_arrowIcon.paint(&painter, arrowRect, Qt::AlignCenter, isEnabled() ? QIcon::Normal : QIcon::Disabled);
    }

    if (isEditable())
        return;

    const int arrowSpace = m_arrowVisible ? m_arrowIconSize.width() + m_arrowRightMargin + 8 : 0;
    QRect textRect = rect().adjusted(m_contentPadding, 0, -m_contentPadding, 0);
    if (rightToLeft)
        textRect.setLeft(textRect.left() + arrowSpace);
    else
        textRect.setRight(textRect.right() - arrowSpace);

    const QIcon itemIcon = currentIndex() >= 0 ? this->itemIcon(currentIndex()) : QIcon();
    if (!itemIcon.isNull() && textRect.width() > 0) {
        const QSize itemSize = iconSize().boundedTo(textRect.size());
        const int iconX = rightToLeft ? textRect.right() - itemSize.width() + 1 : textRect.left();
        itemIcon.paint(&painter, QRect(iconX, (height() - itemSize.height()) / 2, itemSize.width(), itemSize.height()), Qt::AlignCenter,
                       isEnabled() ? QIcon::Normal : QIcon::Disabled);
        if (rightToLeft)
            textRect.setRight(iconX - 7);
        else
            textRect.setLeft(iconX + itemSize.width() + 6);
    }

    painter.setPen(isEnabled() ? m_textColor : m_disabledTextColor);
    const QString label = fontMetrics().elidedText(currentText(), Qt::ElideRight, qMax(0, textRect.width()));
    painter.drawText(textRect, Qt::AlignVCenter | (rightToLeft ? Qt::AlignRight : Qt::AlignLeft), label);
}

void ComboBox::showPopup() {
    m_popupOpen = true;
    refreshState();
    QComboBox::showPopup();
    m_popupOpen = view() && view()->isVisible();
    refreshState();
}

void ComboBox::hidePopup() {
    QComboBox::hidePopup();
    m_popupOpen = false;
    refreshState();
}

} // namespace animatedwidgets
