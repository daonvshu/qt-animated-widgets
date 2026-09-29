#pragma once

#include <QColor>
#include <QPixmap>
#include <QRect>

class QPainter;

namespace animatedwidgets {

class CheckableIconPainter final {
public:
    static void draw(QPainter& painter, const QRect& rect, const QPixmap& pixmap,
                     const QColor& color, qreal opacity = 1.0);
};

} // namespace animatedwidgets
