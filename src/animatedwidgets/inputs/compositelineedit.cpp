#include "animatedwidgets/inputs/compositelineedit.h"

#include "animatedwidgets/inputs/iconlineedit.h"

#include <QApplication>
#include <QEvent>
#include <QHBoxLayout>
#include <QPainter>

CompositeLineEdit::CompositeLineEdit(QWidget* parent)
    : QWidget(parent), m_borderTransition(this), m_backgroundTransition(this)
{
    setAttribute(Qt::WA_Hover, true);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(2, 1, 2, 1);
    m_layout->setSpacing(0);
    m_leadingDivider = new QWidget(this);
    m_leadingDivider->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_leadingDivider->setFixedWidth(m_borderWidth);
    m_layout->addWidget(m_leadingDivider);
    m_leadingDivider->hide();
    m_editor = new IconLineEdit(this);
    m_editor->setBorderWidth(0);
    m_editor->setNormalBackgroundColor(Qt::transparent);
    m_editor->setHoverBackgroundColor(Qt::transparent);
    m_editor->setFocusBackgroundColor(Qt::transparent);
    m_editor->setErrorBackgroundColor(Qt::transparent);
    m_editor->setDisabledBackgroundColor(Qt::transparent);
    m_editor->setAnimationDuration(0);
    m_layout->addWidget(m_editor, 1);
    m_trailingDivider = new QWidget(this);
    m_trailingDivider->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_trailingDivider->setFixedWidth(m_borderWidth);
    m_layout->addWidget(m_trailingDivider);
    m_trailingDivider->hide();

    for (auto* transition : {&m_borderTransition, &m_backgroundTransition}) {
        transition->setDuration(m_animationDuration);
        transition->setEasingCurve(QEasingCurve::OutCubic);
        connect(transition, &animatedwidgets::ValueTransition::valueChanged,
                this, [this] { update(); });
    }
    connect(&m_borderTransition, &animatedwidgets::ValueTransition::valueChanged,
            this, [this] { emit currentBorderColorChanged(currentBorderColor()); });
    connect(m_editor, &QLineEdit::textChanged, this, &CompositeLineEdit::textChanged);
    connect(m_editor, &LineEdit::hasErrorChanged, this, [this](bool error) {
        refreshState();
        emit hasErrorChanged(error);
    });
    connect(qApp, &QApplication::focusChanged, this, [this](QWidget* old, QWidget* current) {
        if ((old && isAncestorOf(old)) || (current && isAncestorOf(current))) refreshState();
    });
    refreshState(true);
}

IconLineEdit* CompositeLineEdit::editor() const { return m_editor; }
QWidget* CompositeLineEdit::leadingWidget() const { return m_leadingWidget; }
QWidget* CompositeLineEdit::trailingWidget() const { return m_trailingWidget; }

void CompositeLineEdit::setLeadingWidget(QWidget* widget)
{
    if (widget == m_leadingWidget) return;
    if (widget && (widget == this || widget == m_editor || widget == m_trailingWidget
                   || widget->isAncestorOf(this))) return;
    if (widget) {
        if (widget->parentWidget() && widget->parentWidget()->layout())
            widget->parentWidget()->layout()->removeWidget(widget);
        widget->setParent(this);
    }
    if (m_leadingWidget) {
        m_layout->removeWidget(m_leadingWidget);
        m_leadingWidget->hide();
        m_leadingWidget->deleteLater();
    }
    m_leadingWidget = widget;
    if (widget) {
        m_layout->insertWidget(0, widget);
        widget->show();
    }
    updateGeometry();
    refreshState();
}

void CompositeLineEdit::setTrailingWidget(QWidget* widget)
{
    if (widget == m_trailingWidget) return;
    if (widget && (widget == this || widget == m_editor || widget == m_leadingWidget
                   || widget->isAncestorOf(this))) return;
    if (widget) {
        if (widget->parentWidget() && widget->parentWidget()->layout())
            widget->parentWidget()->layout()->removeWidget(widget);
        widget->setParent(this);
    }
    if (m_trailingWidget) {
        m_layout->removeWidget(m_trailingWidget);
        m_trailingWidget->hide();
        m_trailingWidget->deleteLater();
    }
    m_trailingWidget = widget;
    if (widget) {
        m_layout->addWidget(widget);
        widget->show();
    }
    updateGeometry();
    refreshState();
}

void CompositeLineEdit::setLeadingWidgetFactory(const WidgetFactory& factory)
{
    setLeadingWidget(factory ? factory(this) : nullptr);
}

void CompositeLineEdit::setTrailingWidgetFactory(const WidgetFactory& factory)
{
    setTrailingWidget(factory ? factory(this) : nullptr);
}

QString CompositeLineEdit::text() const { return m_editor->text(); }
void CompositeLineEdit::setText(const QString& text) { m_editor->setText(text); }
QString CompositeLineEdit::placeholderText() const { return m_editor->placeholderText(); }
void CompositeLineEdit::setPlaceholderText(const QString& text) { m_editor->setPlaceholderText(text); }
bool CompositeLineEdit::isReadOnly() const { return m_editor->isReadOnly(); }
void CompositeLineEdit::setReadOnly(bool readOnly) { m_editor->setReadOnly(readOnly); }
bool CompositeLineEdit::hasError() const { return m_editor->hasError(); }
void CompositeLineEdit::setHasError(bool error) { m_editor->setHasError(error); }

QColor CompositeLineEdit::normalBorderColor() const { return m_normalBorderColor; }
void CompositeLineEdit::setNormalBorderColor(const QColor& color) { m_normalBorderColor = color; refreshState(); }
QColor CompositeLineEdit::hoverBorderColor() const { return m_hoverBorderColor; }
void CompositeLineEdit::setHoverBorderColor(const QColor& color) { m_hoverBorderColor = color; refreshState(); }
QColor CompositeLineEdit::focusBorderColor() const { return m_focusBorderColor; }
void CompositeLineEdit::setFocusBorderColor(const QColor& color) { m_focusBorderColor = color; refreshState(); }
QColor CompositeLineEdit::errorBorderColor() const { return m_errorBorderColor; }
void CompositeLineEdit::setErrorBorderColor(const QColor& color) { m_errorBorderColor = color; refreshState(); }
QColor CompositeLineEdit::disabledBorderColor() const { return m_disabledBorderColor; }
void CompositeLineEdit::setDisabledBorderColor(const QColor& color) { m_disabledBorderColor = color; refreshState(); }
QColor CompositeLineEdit::currentBorderColor() const
{
    return m_borderTransition.current().value<QColor>();
}
QColor CompositeLineEdit::backgroundColor() const { return m_backgroundColor; }
void CompositeLineEdit::setBackgroundColor(const QColor& color) { m_backgroundColor = color; refreshState(); }
QColor CompositeLineEdit::hoverBackgroundColor() const { return m_hoverBackgroundColor; }
void CompositeLineEdit::setHoverBackgroundColor(const QColor& color)
{
    m_hoverBackgroundColor = color;
    refreshState();
}
QColor CompositeLineEdit::errorBackgroundColor() const { return m_errorBackgroundColor; }
void CompositeLineEdit::setErrorBackgroundColor(const QColor& color) { m_errorBackgroundColor = color; refreshState(); }
QColor CompositeLineEdit::disabledBackgroundColor() const { return m_disabledBackgroundColor; }
void CompositeLineEdit::setDisabledBackgroundColor(const QColor& color) { m_disabledBackgroundColor = color; refreshState(); }
int CompositeLineEdit::borderWidth() const { return m_borderWidth; }
void CompositeLineEdit::setBorderWidth(int width)
{
    m_borderWidth = qMax(0, width);
    m_layout->setContentsMargins(m_borderWidth + 1, m_borderWidth,
                                 m_borderWidth + 1, m_borderWidth);
    updateInputBorders();
    updateGeometry();
    update();
}
bool CompositeLineEdit::isBorderVisible() const { return m_borderVisible; }
void CompositeLineEdit::setBorderVisible(bool visible)
{
    if (m_borderVisible == visible) return;
    m_borderVisible = visible;
    updateInputBorders();
}
int CompositeLineEdit::cornerRadius() const { return m_cornerRadius; }
void CompositeLineEdit::setCornerRadius(int radius) { m_cornerRadius = qMax(0, radius); update(); }
int CompositeLineEdit::contentPadding() const { return m_editor->contentPadding(); }
void CompositeLineEdit::setContentPadding(int padding)
{
    m_editor->setContentPadding(padding);
    updateGeometry();
}
bool CompositeLineEdit::drawInputLeftBorder() const { return m_drawInputLeftBorder; }
void CompositeLineEdit::setDrawInputLeftBorder(bool draw)
{
    if (m_drawInputLeftBorder == draw) return;
    m_drawInputLeftBorder = draw;
    updateInputBorders();
}
bool CompositeLineEdit::drawInputRightBorder() const { return m_drawInputRightBorder; }
void CompositeLineEdit::setDrawInputRightBorder(bool draw)
{
    if (m_drawInputRightBorder == draw) return;
    m_drawInputRightBorder = draw;
    updateInputBorders();
}
int CompositeLineEdit::spacing() const { return m_layout->spacing(); }
void CompositeLineEdit::setSpacing(int spacing) { m_layout->setSpacing(qMax(0, spacing)); }
int CompositeLineEdit::animationDuration() const { return m_animationDuration; }
void CompositeLineEdit::setAnimationDuration(int duration)
{
    m_animationDuration = qMax(0, duration);
    m_borderTransition.setDuration(m_animationDuration);
    m_backgroundTransition.setDuration(m_animationDuration);
    if (m_animationDuration == 0) refreshState(true);
}

QSize CompositeLineEdit::sizeHint() const
{
    QSize hint = m_layout->sizeHint();
    hint.setHeight(qMax(36, hint.height()));
    return hint;
}

QColor CompositeLineEdit::targetBorderColor() const
{
    if (!isEnabled()) return m_disabledBorderColor;
    if (hasError()) return m_errorBorderColor;
    QWidget* focused = QApplication::focusWidget();
    if (focused && isAncestorOf(focused)) return m_focusBorderColor;
    if (m_hovered) return m_hoverBorderColor;
    return m_normalBorderColor;
}

QColor CompositeLineEdit::targetBackgroundColor() const
{
    if (!isEnabled()) return m_disabledBackgroundColor;
    if (hasError()) return m_errorBackgroundColor;
    QWidget* focused = QApplication::focusWidget();
    if (focused && isAncestorOf(focused)) return m_backgroundColor;
    if (m_hovered && m_hoverBackgroundColor.isValid()) return m_hoverBackgroundColor;
    return m_backgroundColor;
}

void CompositeLineEdit::refreshState(bool immediate)
{
    m_borderTransition.setTarget(targetBorderColor(), immediate);
    m_backgroundTransition.setTarget(targetBackgroundColor(), immediate);
}

void CompositeLineEdit::updateInputBorders()
{
    m_leadingDivider->setFixedWidth(m_borderWidth);
    m_trailingDivider->setFixedWidth(m_borderWidth);
    const bool leftToRight = layoutDirection() == Qt::LeftToRight;
    m_leadingDivider->setVisible(m_borderVisible && m_borderWidth > 0 &&
        (leftToRight ? m_drawInputLeftBorder : m_drawInputRightBorder));
    m_trailingDivider->setVisible(m_borderVisible && m_borderWidth > 0 &&
        (leftToRight ? m_drawInputRightBorder : m_drawInputLeftBorder));
    updateGeometry();
    update();
}

bool CompositeLineEdit::event(QEvent* event)
{
    if (event->type() == QEvent::Enter) m_hovered = true;
    if (event->type() == QEvent::Leave) m_hovered = false;
    const bool handled = QWidget::event(event);
    switch (event->type()) {
    case QEvent::Enter:
    case QEvent::Leave:
    case QEvent::EnabledChange:
        refreshState();
        break;
    case QEvent::LayoutDirectionChange:
        updateInputBorders();
        break;
    default:
        break;
    }
    return handled;
}

void CompositeLineEdit::paintEvent(QPaintEvent*)
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
    if (!m_borderVisible || m_borderWidth == 0) return;
    painter.setBrush(Qt::NoBrush);
    for (const QWidget* divider : {m_leadingDivider, m_trailingDivider}) {
        if (divider->isHidden()) continue;
        const qreal x = divider->geometry().left() + divider->width() / 2.0;
        painter.drawLine(QPointF(x, m_borderWidth),
                         QPointF(x, height() - m_borderWidth));
    }
}
