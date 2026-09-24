#pragma once

#include <QColor>
#include <QPixmap>
#include <QRect>

class QPainter;

namespace animatedwidgets {

void drawCheckableIcon(QPainter& painter, const QRect& rect, const QPixmap& pixmap,
                       const QColor& color, qreal opacity = 1.0);

} // namespace animatedwidgets
