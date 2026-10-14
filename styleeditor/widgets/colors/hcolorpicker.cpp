#include "hcolorpicker.h"

#include <QMouseEvent>
#include <QPainter>

HColorPicker::HColorPicker(QWidget* parent)
    : QWidget(parent)
{
    setCursor(Qt::PointingHandCursor);
}

void HColorPicker::setBaseColor(const QColor& color) {
    if (!color.isValid())
        return;

    m_color = color;
    if (color.hsvHueF() >= 0.0)
        m_hue = static_cast<float>(color.hsvHueF());
    update();
}

void HColorPicker::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    QLinearGradient hue(0, qMax(1, height() - 1), 0, 0);
    for (int step = 0; step <= 6; ++step)
        hue.setColorAt(step / 6.0, QColor::fromHsvF(static_cast<float>(step) / 6.0f, 1.0f, 1.0f));
    painter.fillRect(rect(), hue);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(Qt::white, 2));
    painter.drawEllipse(QPointF(width() / 2.0, (1.0 - m_hue) * (height() - 1)), 7, 7);
}

void HColorPicker::mousePressEvent(QMouseEvent* event) {
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

void HColorPicker::mouseMoveEvent(QMouseEvent* event) {
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

void HColorPicker::selectColor(const QPointF& position) {
    m_hue = static_cast<float>(qBound(0.0, 1.0 - position.y() / qMax(1, height() - 1), 1.0));
    m_color = QColor::fromHsvF(m_hue, static_cast<float>(m_color.hsvSaturationF()), static_cast<float>(m_color.valueF()), static_cast<float>(m_color.alphaF()));
    update();
    emit colorChanged(m_color);
}
