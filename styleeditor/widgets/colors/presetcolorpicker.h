#pragma once

#include <QColor>
#include <QList>
#include <QWidget>

class PresetColorPicker : public QWidget {
    Q_OBJECT

public:
    explicit PresetColorPicker(QWidget* parent = nullptr);
    void setBaseColor(const QColor& color);

signals:
    void colorChanged(const QColor& color);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    QRectF previewRect() const;
    QRectF presetRect(int index) const;

    QColor m_color = QColor(Qt::red);
    const QList<QColor> m_presets{
        QColor(0xaa, 0xff, 0x00), QColor(0x00, 0xff, 0x00), QColor(0xff, 0xff, 0x00), QColor(0xff, 0x00, 0x00),
        QColor(0x55, 0x55, 0xff), QColor(0xff, 0x00, 0xff), QColor(0xff, 0x55, 0x00), QColor(0x00, 0xff, 0xff),
        QColor(0xff, 0x00, 0x7f), QColor(0x00, 0x00, 0xff), QColor(0xaa, 0x00, 0xff), QColor(0xff, 0x55, 0xff),
        QColor(0xff, 0xaa, 0x7f), QColor(0x55, 0xff, 0xff), QColor(0x00, 0xaa, 0x00), QColor(0xff, 0x55, 0x7f)};
};
