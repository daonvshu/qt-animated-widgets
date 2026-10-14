#include "vscolorpicker.h"

#include <QMouseEvent>
#include <QPainter>

VSColorPicker::VSColorPicker(QWidget* parent)
    : QWidget(parent)
{
    setCursor(Qt::CrossCursor);
}

void VSColorPicker::setBaseColor(const QColor& color) {
    if (!color.isValid())
        return;

    m_color = color;
    if (color.hsvHueF() >= 0.0)
        m_hue = static_cast<float>(color.hsvHueF());
    update();
}

void VSColorPicker::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.fillRect(rect(), QColor::fromHsvF(m_hue, 1.0f, 1.0f));

    QLinearGradient saturation(0, 0, qMax(1, width() - 1), 0);
    saturation.setColorAt(0.0, Qt::white);
    saturation.setColorAt(1.0, QColor(255, 255, 255, 0));
    painter.fillRect(rect(), saturation);

    QLinearGradient value(0, 0, 0, qMax(1, height() - 1));
    value.setColorAt(0.0, QColor(0, 0, 0, 0));
    value.setColorAt(1.0, Qt::black);
    painter.fillRect(rect(), value);

    painter.setRenderHint(QPainter::Antialiasing);
    const QPointF position(m_color.hsvSaturationF() * (width() - 1), (1.0 - m_color.valueF()) * (height() - 1));
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(QColor::fromHsvF(m_hue, static_cast<float>(1.0 - m_color.hsvSaturationF()), static_cast<float>(1.0 - m_color.valueF())), 2));
    painter.drawEllipse(position, 7, 7);
}

void VSColorPicker::mousePressEvent(QMouseEvent* event) {
    if (event->button() != Qt::LeftButton) {
        QWidget::mousePressEvent(event);
        return;
    }
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    selectColor(event->position());
#else
    selectColor(event->localPos());
#endif
    event->accept();
}

void VSColorPicker::mouseMoveEvent(QMouseEvent* event) {
    if (!(event->buttons() & Qt::LeftButton)) {
        QWidget::mouseMoveEvent(event);
        return;
    }
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    selectColor(event->position());
#else
    selectColor(event->localPos());
#endif
    event->accept();
}

void VSColorPicker::selectColor(const QPointF& position) {
    const qreal saturation = qBound(0.0, position.x() / qMax(1, width() - 1), 1.0);
    const qreal value = qBound(0.0, 1.0 - position.y() / qMax(1, height() - 1), 1.0);
    m_color = QColor::fromHsvF(m_hue, static_cast<float>(saturation), static_cast<float>(value), static_cast<float>(m_color.alphaF()));
    update();
    emit colorChanged(m_color);
}
