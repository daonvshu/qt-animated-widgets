#include "animatedwidgets/utils/iconpainter.h"

#include <QPainter>

namespace animatedwidgets {

void IconPainter::draw(QPainter& painter, const QRect& rect, const QIcon& icon, const QColor& color, QIcon::Mode mode, QIcon::State state) {
    if (icon.isNull() || rect.isEmpty())
        return;
    if (!color.isValid()) {
        icon.paint(&painter, rect, Qt::AlignCenter, mode, state);
        return;
    }

    const qreal ratio = painter.device()->devicePixelRatioF();
    QPixmap pixmap = icon.pixmap(rect.size() * ratio, mode, state);
    if (pixmap.isNull())
        return;
    pixmap.setDevicePixelRatio(ratio);
    {
        QPainter tint(&pixmap);
        tint.setCompositionMode(QPainter::CompositionMode_SourceIn);
        tint.fillRect(pixmap.rect(), color);
    }

    QRect target(QPoint(), pixmap.size() / ratio);
    target.moveCenter(rect.center());
    painter.drawPixmap(target, pixmap);
}

} // namespace animatedwidgets
