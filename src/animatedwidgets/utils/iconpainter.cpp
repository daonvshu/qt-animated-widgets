#include "animatedwidgets/utils/iconpainter.h"

#include <QPainter>

namespace animatedwidgets {

void IconPainter::draw(QPainter& painter, const QRectF& rect, const QIcon& icon, const QColor& color, QIcon::Mode mode, QIcon::State state) {
    if (icon.isNull() || rect.isEmpty())
        return;

    // Rasterize at the target device resolution so SVG sources are rendered at their final size
    const qreal ratio = painter.device() ? painter.device()->devicePixelRatioF() : 1.0;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QPixmap pixmap = icon.pixmap(rect.size().toSize(), ratio, mode, state);
#else
    QPixmap pixmap = icon.pixmap((rect.size() * ratio).toSize(), mode, state);
    pixmap.setDevicePixelRatio(ratio);
#endif
    if (pixmap.isNull())
        return;

    const QSizeF logicalSize = QSizeF(pixmap.size()) / pixmap.devicePixelRatio();
    if (color.isValid()) {
        QPainter tint(&pixmap);
        tint.setCompositionMode(QPainter::CompositionMode_SourceIn);
        tint.fillRect(QRectF(QPointF(0, 0), logicalSize), color);
    }

    QRectF target(QPointF(0, 0), logicalSize);
    target.moveCenter(rect.center());
    painter.save();
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.drawPixmap(target.topLeft(), pixmap);
    painter.restore();
}

} // namespace animatedwidgets
