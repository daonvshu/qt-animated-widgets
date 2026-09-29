#include "animatedwidgets/utils/checkableiconpainter.h"

#include <QPainter>

namespace animatedwidgets {

void CheckableIconPainter::draw(QPainter& painter, const QRect& rect, const QPixmap& pixmap, const QColor& color, qreal opacity) {
    if (pixmap.isNull() || rect.isEmpty() || opacity <= 0.0)
        return;

    QPixmap tinted = pixmap;
    if (color.isValid()) {
        QPainter tintPainter(&tinted);
        tintPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
        tintPainter.fillRect(tinted.rect(), color);
    }

    painter.save();
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.setOpacity(opacity);
    painter.drawPixmap(rect, tinted);
    painter.restore();
}

} // namespace animatedwidgets
