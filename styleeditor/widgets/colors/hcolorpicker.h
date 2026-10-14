#pragma once

#include <QColor>
#include <QWidget>

class HColorPicker : public QWidget {
    Q_OBJECT

public:
    explicit HColorPicker(QWidget* parent = nullptr);
    void setBaseColor(const QColor& color);

signals:
    void colorChanged(const QColor& color);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    void selectColor(const QPointF& position);

    QColor m_color = QColor(Qt::red);
    float m_hue = 0.0f;
};
