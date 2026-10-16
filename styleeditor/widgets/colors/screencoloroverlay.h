#pragma once

#include <QImage>
#include <QWidget>

class QScreen;

class ScreenColorOverlay : public QWidget {
    Q_OBJECT

public:
    explicit ScreenColorOverlay(QScreen* screen, const QImage& image, QWidget* parent = nullptr);

signals:
    void colorSelected(const QColor& color);
    void cancelled();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void closeEvent(QCloseEvent* event) override;

private:
    void selectColor(const QPointF& globalPosition);

    QImage m_image;
    QRect m_screenGeometry;
    QPointF m_position;
};
