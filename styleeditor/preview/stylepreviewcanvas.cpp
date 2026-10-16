#include "stylepreviewcanvas.h"

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
    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSizeConstraint(QLayout::SetNoConstraint);
}

void StylePreviewCanvas::addPreview(QWidget* preview) {
    m_layout->addWidget(preview, 0, Qt::AlignCenter);
}

void StylePreviewCanvas::setBackground(const QColor& color) {
    m_background = color;
    update();
}

void StylePreviewCanvas::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.fillRect(rect(), m_background.isValid() ? m_background : palette().color(QPalette::Window));
}
