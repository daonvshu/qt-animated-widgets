#include "animatedwidgets/inputs/compositeinputbase.h"

#include <QApplication>
#include <QEvent>
#include <QHBoxLayout>
#include <QPainter>

#include "animatedwidgets/utils/paintutils.h"
#include <QVBoxLayout>

namespace animatedwidgets {

CompositeInputBase::CompositeInputBase(QWidget* parent)
    : QWidget(parent)
    , m_borderTransition(this)
    , m_backgroundTransition(this)
{
    setAttribute(Qt::WA_Hover, true);
    m_layout = new QVBoxLayout(this);
    m_layout->setSpacing(0);

    const auto makeHost = [this] {
        auto* host = new QWidget(this);
        auto* layout = new QHBoxLayout(host);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        return host;
    };

    const auto makeDivider = [this] {
        auto* divider = new QWidget(this);
        divider->setAttribute(Qt::WA_TransparentForMouseEvents);
        divider->hide();
        return divider;
    };

    m_topHost = makeHost();
    m_topHost->hide();
    m_topDivider = makeDivider();
    m_layout->addWidget(m_topHost);
    m_layout->addWidget(m_topDivider);

    m_middleHost = new QWidget(this);
    m_middleLayout = new QHBoxLayout(m_middleHost);
    m_middleLayout->setContentsMargins(0, 0, 0, 0);
    m_middleLayout->setSpacing(0);

    m_leadingHost = makeHost();
    m_leadingHost->hide();
    m_leadingDivider = makeDivider();
    m_editorHost = makeHost();
    m_trailingDivider = makeDivider();
    m_trailingHost = makeHost();
    m_trailingHost->hide();

    m_middleLayout->addWidget(m_leadingHost);
    m_middleLayout->addWidget(m_leadingDivider);
    m_middleLayout->addWidget(m_editorHost, 1);
    m_middleLayout->addWidget(m_trailingDivider);
    m_middleLayout->addWidget(m_trailingHost);
    m_layout->addWidget(m_middleHost, 1);

    m_bottomDivider = makeDivider();
    m_bottomHost = makeHost();
    m_bottomHost->hide();
    m_layout->addWidget(m_bottomDivider);
    m_layout->addWidget(m_bottomHost);

    updateFrameMargins();
    updateDividers();

    for (auto* transition : {&m_borderTransition, &m_backgroundTransition}) {
        transition->setDuration(m_animationDuration);
        transition->setEasingCurve(QEasingCurve::OutCubic);
        connect(transition, &ValueTransition::valueChanged, this, [this] { update(); });
    }

    connect(&m_borderTransition, &ValueTransition::valueChanged, this,
            [this] { emit currentBorderColorChanged(currentBorderColor()); });
    connect(qApp, &QApplication::focusChanged, this, [this](QWidget* old, QWidget* current) {
        if ((old && isAncestorOf(old)) || (current && isAncestorOf(current)))
            refreshState();
    });
    refreshState(true);
}

QWidget* CompositeInputBase::topWidget() const {
    return m_topWidget;
}

QWidget* CompositeInputBase::bottomWidget() const {
    return m_bottomWidget;
}

QWidget* CompositeInputBase::leadingWidget() const {
    return m_leadingWidget;
}

QWidget* CompositeInputBase::trailingWidget() const {
    return m_trailingWidget;
}

void CompositeInputBase::setTopWidget(QWidget* widget) {
    setSlotWidget(Slot::Top, widget);
}

void CompositeInputBase::setBottomWidget(QWidget* widget) {
    setSlotWidget(Slot::Bottom, widget);
}

void CompositeInputBase::setLeadingWidget(QWidget* widget) {
    setSlotWidget(Slot::Leading, widget);
}

void CompositeInputBase::setTrailingWidget(QWidget* widget) {
    setSlotWidget(Slot::Trailing, widget);
}

QWidget* CompositeInputBase::slotWidget(Slot slot) const {
    switch (slot) {
        case Slot::Top: return m_topWidget;
        case Slot::Bottom: return m_bottomWidget;
        case Slot::Leading: return m_leadingWidget;
        case Slot::Trailing: return m_trailingWidget;
    }
    return nullptr;
}

QWidget* CompositeInputBase::slotHost(Slot slot) const {
    switch (slot) {
        case Slot::Top: return m_topHost;
        case Slot::Bottom: return m_bottomHost;
        case Slot::Leading: return m_leadingHost;
        case Slot::Trailing: return m_trailingHost;
    }
    return nullptr;
}

QBoxLayout* CompositeInputBase::slotLayout(Slot slot) const {
    return static_cast<QBoxLayout*>(slotHost(slot)->layout());
}

void CompositeInputBase::setSlotWidget(Slot slot, QWidget* widget) {
    QWidget* previous = slotWidget(slot);
    if (widget == previous)
        return;
    if (widget && (widget == this || widget == m_editorWidget || widget->isAncestorOf(this) || widget == m_topHost || widget == m_bottomHost ||
                   widget == m_leadingHost || widget == m_trailingHost || widget == m_editorHost || widget == m_middleHost ||
                   (slot != Slot::Top && widget == m_topWidget) || (slot != Slot::Bottom && widget == m_bottomWidget) ||
                   (slot != Slot::Leading && widget == m_leadingWidget) || (slot != Slot::Trailing && widget == m_trailingWidget)))
        return;
    QWidget* host = slotHost(slot);
    QBoxLayout* layout = slotLayout(slot);

    if (widget) {
        if (widget->parentWidget() && widget->parentWidget()->layout())
            widget->parentWidget()->layout()->removeWidget(widget);
        widget->setParent(host);
    }

    if (previous) {
        layout->removeWidget(previous);
        previous->hide();
        previous->deleteLater();
    }

    switch (slot) {
        case Slot::Top: m_topWidget = widget; break;
        case Slot::Bottom: m_bottomWidget = widget; break;
        case Slot::Leading: m_leadingWidget = widget; break;
        case Slot::Trailing: m_trailingWidget = widget; break;
    }

    if (widget) {
        layout->addWidget(widget);
        widget->show();
    }

    host->setVisible(widget != nullptr);
    updateDividers();
    updateGeometry();
    refreshState();
}

void CompositeInputBase::setEditorWidget(QWidget* editor) {
    if (!editor || m_editorWidget)
        return;
    m_editorWidget = editor;
    editor->setParent(m_editorHost);
    m_editorHost->layout()->addWidget(editor);
    updateGeometry();
}

void CompositeInputBase::setSlotMargins(const QMargins& top, const QMargins& bottom) {
    m_topHost->layout()->setContentsMargins(top);
    m_bottomHost->layout()->setContentsMargins(bottom);
    updateGeometry();
}

void CompositeInputBase::setHorizontalFrameInset(int extra) {
    m_horizontalFrameInset = qMax(0, extra);
    updateFrameMargins();
}

bool CompositeInputBase::hasError() const {
    return m_hasError;
}

void CompositeInputBase::setHasError(bool error) {
    if (m_hasError == error)
        return;
    m_hasError = error;
    refreshState();
    emit hasErrorChanged(error);
}

QColor CompositeInputBase::borderNormal() const {
    return m_normalBorderColor;
}

void CompositeInputBase::setBorderNormal(const QColor& color) {
    m_normalBorderColor = color;
    refreshState();
}

QColor CompositeInputBase::borderHover() const {
    return m_hoverBorderColor;
}

void CompositeInputBase::setBorderHover(const QColor& color) {
    m_hoverBorderColor = color;
    refreshState();
}

QColor CompositeInputBase::borderFocus() const {
    return m_focusBorderColor;
}

void CompositeInputBase::setBorderFocus(const QColor& color) {
    m_focusBorderColor = color;
    refreshState();
}

QColor CompositeInputBase::borderError() const {
    return m_errorBorderColor;
}

void CompositeInputBase::setBorderError(const QColor& color) {
    m_errorBorderColor = color;
    refreshState();
}

QColor CompositeInputBase::borderDisabled() const {
    return m_disabledBorderColor;
}

void CompositeInputBase::setBorderDisabled(const QColor& color) {
    m_disabledBorderColor = color;
    refreshState();
}

QColor CompositeInputBase::currentBorderColor() const {
    return m_borderTransition.current().value<QColor>();
}

QColor CompositeInputBase::backgroundNormal() const {
    return m_normalBackgroundColor;
}

void CompositeInputBase::setBackgroundNormal(const QColor& color) {
    m_normalBackgroundColor = color;
    refreshState();
}

QColor CompositeInputBase::backgroundHover() const {
    return m_hoverBackgroundColor;
}

void CompositeInputBase::setBackgroundHover(const QColor& color) {
    m_hoverBackgroundColor = color;
    refreshState();
}

QColor CompositeInputBase::backgroundFocus() const {
    return m_focusBackgroundColor;
}

void CompositeInputBase::setBackgroundFocus(const QColor& color) {
    m_focusBackgroundColor = color;
    refreshState();
}

QColor CompositeInputBase::backgroundError() const {
    return m_errorBackgroundColor;
}

void CompositeInputBase::setBackgroundError(const QColor& color) {
    m_errorBackgroundColor = color;
    refreshState();
}

QColor CompositeInputBase::backgroundDisabled() const {
    return m_disabledBackgroundColor;
}

void CompositeInputBase::setBackgroundDisabled(const QColor& color) {
    m_disabledBackgroundColor = color;
    refreshState();
}

int CompositeInputBase::borderWidth() const {
    return m_borderWidth;
}

void CompositeInputBase::setBorderWidth(int width) {
    m_borderWidth = qMax(0, width);
    updateFrameMargins();
    updateDividers();
    update();
}

bool CompositeInputBase::isBorderVisible() const {
    return m_borderVisible;
}

void CompositeInputBase::setBorderVisible(bool visible) {
    if (m_borderVisible == visible)
        return;
    m_borderVisible = visible;
    updateFrameMargins();
    updateDividers();
    update();
}

int CompositeInputBase::cornerRadius() const {
    return m_cornerRadius;
}

void CompositeInputBase::setCornerRadius(int radius) {
    m_cornerRadius = qMax(0, radius);
    update();
}

bool CompositeInputBase::drawInputLeftBorder() const {
    return m_drawInputLeftBorder;
}

void CompositeInputBase::setDrawInputLeftBorder(bool draw) {
    m_drawInputLeftBorder = draw;
    updateDividers();
}

bool CompositeInputBase::drawInputRightBorder() const {
    return m_drawInputRightBorder;
}

void CompositeInputBase::setDrawInputRightBorder(bool draw) {
    m_drawInputRightBorder = draw;
    updateDividers();
}

bool CompositeInputBase::drawInputTopBorder() const {
    return m_drawInputTopBorder;
}

void CompositeInputBase::setDrawInputTopBorder(bool draw) {
    m_drawInputTopBorder = draw;
    updateDividers();
}

bool CompositeInputBase::drawInputBottomBorder() const {
    return m_drawInputBottomBorder;
}

void CompositeInputBase::setDrawInputBottomBorder(bool draw) {
    m_drawInputBottomBorder = draw;
    updateDividers();
}

int CompositeInputBase::spacing() const {
    return m_layout->spacing();
}

void CompositeInputBase::setSpacing(int spacing) {
    const int value = qMax(0, spacing);
    m_layout->setSpacing(value);
    m_middleLayout->setSpacing(value);
}

int CompositeInputBase::animationDuration() const {
    return m_animationDuration;
}

void CompositeInputBase::setAnimationDuration(int duration) {
    m_animationDuration = qMax(0, duration);
    m_borderTransition.setDuration(m_animationDuration);
    m_backgroundTransition.setDuration(m_animationDuration);
    if (m_animationDuration == 0)
        refreshState(true);
}

QSize CompositeInputBase::sizeHint() const {
    return m_layout->sizeHint();
}

QColor CompositeInputBase::targetBorderColor() const {
    if (!isEnabled())
        return m_disabledBorderColor;
    if (m_hasError)
        return m_errorBorderColor;
    QWidget* focused = QApplication::focusWidget();
    if (focused && isAncestorOf(focused))
        return m_focusBorderColor;
    if (m_hovered)
        return m_hoverBorderColor;
    return m_normalBorderColor;
}

QColor CompositeInputBase::targetBackgroundColor() const {
    if (!isEnabled())
        return m_disabledBackgroundColor;
    if (m_hasError)
        return m_errorBackgroundColor;
    QWidget* focused = QApplication::focusWidget();
    if (focused && isAncestorOf(focused))
        return m_focusBackgroundColor;
    if (m_hovered && m_hoverBackgroundColor.isValid())
        return m_hoverBackgroundColor;
    return m_normalBackgroundColor;
}

void CompositeInputBase::refreshState(bool immediate) {
    m_borderTransition.setTarget(targetBorderColor(), immediate || !isVisible());
    m_backgroundTransition.setTarget(targetBackgroundColor(), immediate || !isVisible());
}

void CompositeInputBase::updateFrameMargins() {
    const int frame = m_borderVisible ? m_borderWidth : 0;
    m_layout->setContentsMargins(frame + m_horizontalFrameInset, frame, frame + m_horizontalFrameInset, frame);
    updateGeometry();
}

void CompositeInputBase::updateDividers() {
    const bool visible = m_borderVisible && m_borderWidth > 0;
    const bool leftToRight = layoutDirection() == Qt::LeftToRight;
    m_leadingDivider->setFixedWidth(m_borderWidth);
    m_trailingDivider->setFixedWidth(m_borderWidth);
    m_topDivider->setFixedHeight(m_borderWidth);
    m_bottomDivider->setFixedHeight(m_borderWidth);
    m_leadingDivider->setVisible(visible && m_leadingWidget && (leftToRight ? m_drawInputLeftBorder : m_drawInputRightBorder));
    m_trailingDivider->setVisible(visible && m_trailingWidget && (leftToRight ? m_drawInputRightBorder : m_drawInputLeftBorder));
    m_topDivider->setVisible(visible && m_topWidget && m_drawInputTopBorder);
    m_bottomDivider->setVisible(visible && m_bottomWidget && m_drawInputBottomBorder);
    updateGeometry();
    update();
}

bool CompositeInputBase::event(QEvent* event) {
    if (event->type() == QEvent::Enter)
        m_hovered = true;
    if (event->type() == QEvent::Leave)
        m_hovered = false;
    const bool handled = QWidget::event(event);
    switch (event->type()) {
        case QEvent::Enter:
        case QEvent::Leave:
        case QEvent::EnabledChange:
            refreshState();
            break;
        case QEvent::LayoutDirectionChange:
            updateDividers();
            break;
        default:
            break;
    }
    return handled;
}

void CompositeInputBase::paintEvent(QPaintEvent*) {
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
    painter.setPen(strokeWidth >= 0 ? QPen(currentBorderColor(), strokeWidth) : QPen(Qt::NoPen));
    painter.drawRoundedRect(frame, m_cornerRadius, m_cornerRadius);
    if (!m_borderVisible || m_borderWidth == 0)
        return;

    const qreal dpr = devicePixelRatioF();
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(currentBorderColor(), strokeWidth));
    for (const QWidget* divider : {m_leadingDivider, m_trailingDivider}) {
        if (divider->isHidden())
            continue;
        const QPoint origin = divider->mapTo(this, QPoint(0, 0));
        const qreal x = paintutils::alignVerticalLine(origin.x() + divider->width() / 2.0, dpr);
        const qreal top = m_middleHost->mapTo(this, QPoint(0, 0)).y();
        painter.drawLine(QPointF(x, top), QPointF(x, top + m_middleHost->height()));
    }
    for (const QWidget* divider : {m_topDivider, m_bottomDivider}) {
        if (divider->isHidden())
            continue;
        const QPoint origin = divider->mapTo(this, QPoint(0, 0));
        const qreal y = paintutils::alignHorizontalLine(origin.y() + divider->height() / 2.0, dpr);
        painter.drawLine(QPointF(origin.x(), y), QPointF(origin.x() + divider->width(), y));
    }
}

} // namespace animatedwidgets
