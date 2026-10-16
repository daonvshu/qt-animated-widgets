#pragma once

#include <QColor>
#include <QIcon>
#include <QRect>

class QPainter;

namespace animatedwidgets {

class IconPainter final {
public:
    static void draw(QPainter& painter, const QRect& rect, const QIcon& icon, const QColor& color = {},
                     QIcon::Mode mode = QIcon::Normal, QIcon::State state = QIcon::Off);
};

} // namespace animatedwidgets
