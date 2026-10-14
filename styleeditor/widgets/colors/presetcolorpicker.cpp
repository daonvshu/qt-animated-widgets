#include "presetcolorpicker.h"

#include <QMouseEvent>
#include <QPainter>

PresetColorPicker::PresetColorPicker(QWidget* parent)
    : QWidget(parent)
{
    setCursor(Qt::PointingHandCursor);
    setMinimumSize(272, 72);
}

void PresetColorPicker::setBaseColor(const QColor& color) {
    if (!color.isValid())
        return;
    m_color = color;
    update();
}

QRectF PresetColorPicker::previewRect() const {
    return QRectF(16, (height() - 40) / 2.0, 40, 40);
}

QRectF PresetColorPicker::presetRect(int index) const {
    const qreal spacing = (width() - 80 - 16) / 7.0;
    return QRectF(64 + (index % 8) * spacing, height() / 2.0 - 20 + (index / 8) * 24, 16, 16);
}

void PresetColorPicker::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    const QRectF preview = previewRect();
    for (int row = 0; row < 5; ++row) {
        for (int column = 0; column < 5; ++column)
            painter.fillRect(QRectF(preview.x() + column * 8, preview.y() + row * 8, 8, 8),
                             (row + column) % 2 == 0 ? QColor(Qt::white) : QColor(QStringLiteral("#e5e6eb")));
    }
    painter.fillRect(preview, m_color);
    painter.setPen(QColor(QStringLiteral("#e5e6eb")));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(preview);

    for (int index = 0; index < m_presets.size(); ++index) {
        painter.setBrush(m_presets.at(index));
        painter.drawRect(presetRect(index));
    }
}

void PresetColorPicker::mousePressEvent(QMouseEvent* event) {
    if (event->button() != Qt::LeftButton) {
        QWidget::mousePressEvent(event);
        return;
    }
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    const QPointF position = event->position();
#else
    const QPointF position = event->localPos();
#endif
    for (int index = 0; index < m_presets.size(); ++index) {
        if (!presetRect(index).contains(position))
            continue;
        QColor color = m_presets.at(index);
        color.setAlpha(m_color.alpha());
        setBaseColor(color);
        event->accept();
        emit colorChanged(color);
        return;
    }
    QWidget::mousePressEvent(event);
}
