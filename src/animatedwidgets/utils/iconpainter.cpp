#include "animatedwidgets/utils/iconpainter.h"

#include <QPainter>

namespace animatedwidgets {

void IconPainter::draw(QPainter& painter, const QRectF& rect, const QIcon& icon, const QColor& color, QIcon::Mode mode, QIcon::State state) {
    if (icon.isNull() || rect.isEmpty())
        return;

    const qreal ratio = painter.device() ? painter.device()->devicePixelRatioF() : 1.0;
    const QSize pixelSize = (rect.size() * ratio).toSize();
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QPixmap pixmap = icon.pixmap(rect.size().toSize(), ratio, mode, state);
#else
    QPixmap pixmap = icon.pixmap(pixelSize, mode, state);
    pixmap.setDevicePixelRatio(ratio);
#endif
    if (pixmap.isNull())
        return;

    QSizeF targetSize = QSizeF(pixmap.size()) / pixmap.devicePixelRatio();
    if (pixmap.size() == pixelSize)
        targetSize = rect.size();
    else if (targetSize.width() > rect.width() || targetSize.height() > rect.height())
        targetSize.scale(rect.size(), Qt::KeepAspectRatio);

    // Use extra samples for small scalable icons without enlarging the destination.
    constexpr int smallIconLimit = 16;
    if (pixmap.width() <= smallIconLimit && pixmap.height() <= smallIconLimit) {
        const QSize sampleSize = pixmap.size() * 2;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        const QPixmap sampled = icon.pixmap(sampleSize, 1.0, mode, state);
#else
        const QPixmap sampled = icon.pixmap(sampleSize, mode, state);
#endif
        if (sampled.size() == sampleSize)
            pixmap = sampled;
    }

    if (color.isValid()) {
        QPainter tint(&pixmap);
        tint.setCompositionMode(QPainter::CompositionMode_SourceIn);
        tint.fillRect(QRectF(QPointF(0, 0), QSizeF(pixmap.size()) / pixmap.devicePixelRatio()), color);
    }

    QRectF target(QPointF(0, 0), targetSize);
    target.moveCenter(rect.center());

    painter.save();
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.drawPixmap(target, pixmap, QRectF(pixmap.rect()));
    painter.restore();
}

} // namespace animatedwidgets
