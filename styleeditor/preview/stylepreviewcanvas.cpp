#include "stylepreviewcanvas.h"

#include <QPaintEvent>
#include <QPainter>
#include <QVBoxLayout>

StylePreviewCanvas::StylePreviewCanvas()
    : QWidget(nullptr, Qt::Window | Qt::FramelessWindowHint)
{
    createLayout();
}

void StylePreviewCanvas::createLayout() {
    setWindowTitle(QStringLiteral("Widget preview"));
    setAttribute(Qt::WA_ShowWithoutActivating);
    setAttribute(Qt::WA_OpaquePaintEvent);
    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSizeConstraint(QLayout::SetNoConstraint);
}

void StylePreviewCanvas::addPreview(QWidget* preview) {
    m_layout->addWidget(preview, 0, Qt::AlignCenter);
}

void StylePreviewCanvas::setBackground(const QColor& color, const QColor& themeColor) {
    if (m_background == color && m_themeBackground == themeColor)
        return;

    m_background = color;
    m_themeBackground = themeColor;
    update();
}

void StylePreviewCanvas::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    const QRect dirty = event->rect();
    if (m_background.isValid() && m_background.alpha() == 255) {
        painter.fillRect(QRectF(dirty), m_background);
        return;
    }

    QColor base = m_themeBackground.isValid() ? m_themeBackground : palette().color(QPalette::Window);
    base.setAlpha(255);
    painter.fillRect(QRectF(dirty), base);

    constexpr int tileSize = 12;
    const QColor alternate = base.lightness() < 128 ? QColor(255, 255, 255, 24) : QColor(0, 0, 0, 20);
    const int firstColumn = dirty.left() / tileSize;
    const int lastColumn = dirty.right() / tileSize;
    const int lastRow = dirty.bottom() / tileSize;
    for (int row = dirty.top() / tileSize; row <= lastRow; ++row) {
        for (int column = firstColumn + (row + firstColumn) % 2; column <= lastColumn; column += 2)
            painter.fillRect(QRectF(column * tileSize, row * tileSize, tileSize, tileSize), alternate);
    }

    if (m_background.isValid())
        painter.fillRect(QRectF(dirty), m_background);
}
