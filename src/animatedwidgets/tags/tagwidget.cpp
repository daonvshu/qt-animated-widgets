#include "animatedwidgets/tags/tagwidget.h"

#include "animatedwidgets/buttons/hovericonbutton.h"
#include "animatedwidgets/utils/buttonpainter.h"
#include "animatedwidgets/utils/widgetresources.h"

#include <QEvent>
#include <QPainter>
#include <QResizeEvent>

namespace animatedwidgets {

TagWidget::TagWidget(QWidget* parent)
    : QWidget(parent)
{
    WidgetResources::initializeInputs();
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_closeButton = new HoverIconButton(this);
    m_closeButton->setFocusPolicy(Qt::NoFocus);
    m_closeButton->setCursor(Qt::PointingHandCursor);
    m_closeButton->setAnimateContentColor(true);
    connect(m_closeButton, &QPushButton::clicked, this, &TagWidget::closeRequested);
    applyStyle();
}

QString TagWidget::text() const {
    return m_text;
}

void TagWidget::setText(const QString& text) {
    m_text = text;
    setAccessibleName(text);
    m_closeButton->setAccessibleName(tr("Remove %1").arg(text));
    updateGeometry();
    update();
}

QIcon TagWidget::icon() const {
    return m_icon;
}

void TagWidget::setIcon(const QIcon& icon) {
    m_icon = icon;
    updateGeometry();
    update();
}

bool TagWidget::iconHint() const {
    return m_style.iconHint;
}

void TagWidget::setIconHint(bool enabled) {
    if (m_style.iconHint == enabled)
        return;
    m_style.iconHint = enabled;
    applyStyle();
}

TagStyle TagWidget::tagStyle() const {
    return m_style;
}

void TagWidget::setTagStyle(const TagStyle& style) {
    m_style = style;
    m_style.borderWidth = qMax(0, style.borderWidth);
    m_style.cornerRadius = qMax(0, style.cornerRadius);
    m_style.contentPadding = qMax(0, style.contentPadding);
    m_style.iconSize = QSize(qMax(1, style.iconSize.width()), qMax(1, style.iconSize.height()));
    m_style.iconSpacing = qMax(0, style.iconSpacing);
    m_style.closeIconSize = QSize(qMax(1, style.closeIconSize.width()), qMax(1, style.closeIconSize.height()));
    m_style.closeButtonSize = qMax(1, style.closeButtonSize);
    applyStyle();
}

QColor TagWidget::background() const {
    return m_style.background;
}

void TagWidget::setBackground(const QColor& color) {
    m_style.background = color;
    applyStyle();
}

QColor TagWidget::border() const {
    return m_style.border;
}

void TagWidget::setBorder(const QColor& color) {
    m_style.border = color;
    applyStyle();
}

QColor TagWidget::content() const {
    return m_style.content;
}

void TagWidget::setContent(const QColor& color) {
    m_style.content = color;
    applyStyle();
}

QColor TagWidget::disabledContent() const {
    return m_style.disabledContent;
}

void TagWidget::setDisabledContent(const QColor& color) {
    m_style.disabledContent = color;
    applyStyle();
}

int TagWidget::borderWidth() const {
    return m_style.borderWidth;
}

void TagWidget::setBorderWidth(int width) {
    m_style.borderWidth = qMax(0, width);
    applyStyle();
}

int TagWidget::cornerRadius() const {
    return m_style.cornerRadius;
}

void TagWidget::setCornerRadius(int radius) {
    m_style.cornerRadius = qMax(0, radius);
    applyStyle();
}

int TagWidget::contentPadding() const {
    return m_style.contentPadding;
}

void TagWidget::setContentPadding(int padding) {
    m_style.contentPadding = qMax(0, padding);
    applyStyle();
}

QSize TagWidget::iconSize() const {
    return m_style.iconSize;
}

void TagWidget::setIconSize(const QSize& size) {
    m_style.iconSize = QSize(qMax(1, size.width()), qMax(1, size.height()));
    applyStyle();
}

int TagWidget::iconSpacing() const {
    return m_style.iconSpacing;
}

void TagWidget::setIconSpacing(int spacing) {
    m_style.iconSpacing = qMax(0, spacing);
    applyStyle();
}

QIcon TagWidget::closeIcon() const {
    return m_style.closeIcon;
}

void TagWidget::setCloseIcon(const QIcon& icon) {
    m_style.closeIcon = icon;
    applyStyle();
}

QSize TagWidget::closeIconSize() const {
    return m_style.closeIconSize;
}

void TagWidget::setCloseIconSize(const QSize& size) {
    m_style.closeIconSize = QSize(qMax(1, size.width()), qMax(1, size.height()));
    applyStyle();
}

int TagWidget::closeButtonSize() const {
    return m_style.closeButtonSize;
}

void TagWidget::setCloseButtonSize(int size) {
    m_style.closeButtonSize = qMax(1, size);
    applyStyle();
}

bool TagWidget::closeButtonVisible() const {
    return m_style.closeButtonVisible;
}

void TagWidget::setCloseButtonVisible(bool visible) {
    m_style.closeButtonVisible = visible;
    applyStyle();
}

QSize TagWidget::sizeHint() const {
    const ButtonContentOptions options{m_icon, m_style.iconSize, m_text, font(), m_style.content, m_style.iconSpacing};
    QSize size = ButtonPainter::contentSize(options);
    if (m_style.closeButtonVisible) {
        size.rwidth() += m_style.iconSpacing + m_style.closeButtonSize;
        size.setHeight(qMax(size.height(), m_style.closeButtonSize));
    }
    return size + QSize(2 * (m_style.contentPadding + m_style.borderWidth), 2 * (3 + m_style.borderWidth));
}

QSize TagWidget::minimumSizeHint() const {
    return sizeHint();
}

void TagWidget::applyStyle() {
    m_closeButton->setIcon(m_style.closeIcon.isNull() ? QIcon(QStringLiteral(":/animatedwidgets/inputs/clear.svg")) : m_style.closeIcon);
    m_closeButton->setIconSize(m_style.closeIconSize);
    m_closeButton->setIconHint(m_style.iconHint);
    m_closeButton->setVisible(m_style.closeButtonVisible);
    m_closeButton->setContentNormal(m_style.content);
    m_closeButton->setContentHover(m_style.content);
    m_closeButton->setContentDisabled(m_style.disabledContent);
    m_closeButton->setBackgroundHover(QColor(0, 0, 0, 16));
    updateCloseGeometry();
    updateGeometry();
    update();
}

void TagWidget::updateCloseGeometry() {
    const int inset = m_style.contentPadding + m_style.borderWidth;
    const int x = layoutDirection() == Qt::LeftToRight ? width() - inset - m_style.closeButtonSize : inset;
    m_closeButton->setGeometry(x, (height() - m_style.closeButtonSize) / 2, m_style.closeButtonSize, m_style.closeButtonSize);
}

bool TagWidget::event(QEvent* event) {
    const bool handled = QWidget::event(event);
    if (event->type() == QEvent::FontChange || event->type() == QEvent::LayoutDirectionChange) {
        updateCloseGeometry();
        updateGeometry();
        update();
    }
    return handled;
}

void TagWidget::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    ButtonPainter::drawSurface(painter, QRectF(rect()),
                               {m_style.background, m_style.border, qreal(m_style.borderWidth), m_style.cornerRadius, true, m_style.borderWidth > 0, false});

    const int padding = m_style.contentPadding + m_style.borderWidth;
    QRect bounds = rect().adjusted(padding, 3, -padding, -3);
    if (m_style.closeButtonVisible) {
        const int reserved = m_style.closeButtonSize + m_style.iconSpacing;
        if (layoutDirection() == Qt::LeftToRight)
            bounds.adjust(0, 0, -reserved, 0);
        else
            bounds.adjust(reserved, 0, 0, 0);
    }
    ButtonContentOptions options{m_icon, m_style.iconSize, m_text, font(), isEnabled() ? m_style.content : m_style.disabledContent, m_style.iconSpacing};
    options.iconHint = m_style.iconHint;
    options.enabled = isEnabled();
    if (layoutDirection() == Qt::RightToLeft)
        options.placement = ButtonContentOptions::Placement::AfterText;
    ButtonPainter::drawContent(painter, bounds, options);
}

void TagWidget::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    updateCloseGeometry();
}

} // namespace animatedwidgets
