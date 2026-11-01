#pragma once

#include <QColor>
#include <QIcon>
#include <QPixmap>
#include <QRectF>

class QPainter;

namespace animatedwidgets {

class CheckableIconPainter final {
public:
    static void draw(QPainter& painter, const QRectF& rect, const QPixmap& pixmap, const QColor& color, qreal opacity = 1.0);
    static void draw(QPainter& painter, const QRectF& rect, const QIcon& icon, const QColor& color, qreal opacity = 1.0);
};

} // namespace animatedwidgets
