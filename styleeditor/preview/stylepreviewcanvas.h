#pragma once

#include <QColor>
#include <QWidget>

class QVBoxLayout;

class StylePreviewCanvas : public QWidget {
public:
    StylePreviewCanvas();

    void addPreview(QWidget* preview);
    void setBackground(const QColor& color);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    void createLayout();

    QVBoxLayout* m_layout = nullptr;
    QColor m_background;
};
