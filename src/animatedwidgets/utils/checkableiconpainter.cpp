#include "animatedwidgets/utils/checkableiconpainter.h"

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
    painter.drawPixmap(target.topLeft(), tinted);
    painter.restore();
}

void CheckableIconPainter::draw(QPainter& painter, const QRectF& rect, const QIcon& icon, const QColor& color, qreal opacity) {
    if (icon.isNull() || rect.isEmpty() || opacity <= 0.0)
        return;

    // Rasterize at the target device resolution so SVG sources are rendered at their final size
    const qreal ratio = painter.device() ? painter.device()->devicePixelRatioF() : 1.0;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    const QPixmap pixmap = icon.pixmap(rect.size().toSize(), ratio);
#else
    QPixmap pixmap = icon.pixmap((rect.size() * ratio).toSize());
    pixmap.setDevicePixelRatio(ratio);
#endif
    draw(painter, rect, pixmap, color, opacity);
}

} // namespace animatedwidgets
