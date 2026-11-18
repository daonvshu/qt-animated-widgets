#pragma once

#include <QPainter>
#include <QRectF>

#include <cmath>

namespace animatedwidgets {

class PaintUtils final {
public:
    struct BorderStroke {
        QRectF rect;
        QPen pen;
    };

    // Keep the stroke inside the fully visible device pixels of the logical bounds.
    static BorderStroke borderStroke(const QPainter& painter, const QRectF& bounds, const QColor& color, qreal width) {
        if (width <= 0 || bounds.isEmpty())
            return {bounds, QPen(Qt::NoPen)};

        const qreal dpr = painter.device() ? painter.device()->devicePixelRatioF() : 1.0;
        const int pixelWidth = qMax(1, qRound(width * dpr));
        const qreal halfWidth = pixelWidth / 2.0;
        const QPointF origin = painter.deviceTransform().map(QPointF());
        const qreal left = std::ceil(origin.x() + bounds.left() * dpr) + halfWidth;
        const qreal top = std::ceil(origin.y() + bounds.top() * dpr) + halfWidth;
        const qreal right = std::floor(origin.x() + bounds.right() * dpr) - halfWidth;
        const qreal bottom = std::floor(origin.y() + bounds.bottom() * dpr) - halfWidth;
        if (right <= left || bottom <= top)
            return {bounds, QPen(Qt::NoPen)};

        const QRectF rect((left - origin.x()) / dpr, (top - origin.y()) / dpr, (right - left) / dpr, (bottom - top) / dpr);
        return {rect, QPen(color, pixelWidth == 1 ? 0.0 : pixelWidth / dpr)};
    }
};

} // namespace animatedwidgets
