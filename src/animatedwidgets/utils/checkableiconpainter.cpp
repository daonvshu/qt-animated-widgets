#include "animatedwidgets/utils/checkableiconpainter.h"
#include "animatedwidgets/utils/iconpainter.h"

#include <QPainter>

namespace animatedwidgets {

void CheckableIconPainter::draw(QPainter& painter, const QRectF& rect, const QPixmap& pixmap, const QColor& color, qreal opacity) {
    if (pixmap.isNull() || rect.isEmpty() || opacity <= 0.0)
        return;

    QPixmap tinted = pixmap;
    const QSizeF logical = QSizeF(tinted.size()) / tinted.devicePixelRatio();
    if (color.isValid()) {
        QPainter tintPainter(&tinted);
        tintPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
        tintPainter.fillRect(QRectF(QPointF(0, 0), logical), color);
    }

    QRectF target(QPointF(0, 0), logical);
    target.moveCenter(rect.center());

    painter.save();
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.setOpacity(opacity);
    painter.drawPixmap(target, tinted, QRectF(tinted.rect()));
    painter.restore();
}

void CheckableIconPainter::draw(QPainter& painter, const QRectF& rect, const QIcon& icon, const QColor& color, qreal opacity) {
    if (icon.isNull() || rect.isEmpty() || opacity <= 0.0)
        return;

    painter.save();
    painter.setOpacity(opacity);
    IconPainter::draw(painter, rect, icon, color);
    painter.restore();
}

} // namespace animatedwidgets
