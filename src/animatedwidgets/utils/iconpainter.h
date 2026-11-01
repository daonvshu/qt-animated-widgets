#pragma once

#include <QColor>
#include <QIcon>
#include <QRectF>

class QPainter;

namespace animatedwidgets {

class IconPainter final {
public:
    static void draw(QPainter& painter, const QRectF& rect, const QIcon& icon, const QColor& color = {},
                     QIcon::Mode mode = QIcon::Normal, QIcon::State state = QIcon::Off);
};

} // namespace animatedwidgets
