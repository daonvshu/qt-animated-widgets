#include "screencolorutils.h"

#include <qmath.h>

QPoint ScreenColorUtils::imagePosition(const QSize& imageSize, const QRect& screenGeometry, const QPointF& globalPosition) {
    if (imageSize.isEmpty() || screenGeometry.isEmpty() || !qIsFinite(globalPosition.x()) || !qIsFinite(globalPosition.y()))
        return QPoint(-1, -1);

    const QPointF local = globalPosition - screenGeometry.topLeft();
    if (local.x() < 0 || local.y() < 0 || local.x() >= screenGeometry.width() || local.y() >= screenGeometry.height())
        return QPoint(-1, -1);

    const int x = qFloor(local.x() * imageSize.width() / screenGeometry.width());
    const int y = qFloor(local.y() * imageSize.height() / screenGeometry.height());
    return QPoint(qBound(0, x, imageSize.width() - 1), qBound(0, y, imageSize.height() - 1));
}

QColor ScreenColorUtils::colorAt(const QImage& image, const QRect& screenGeometry, const QPointF& globalPosition) {
    const QPoint pixel = imagePosition(image.size(), screenGeometry, globalPosition);
    return image.rect().contains(pixel) ? image.pixelColor(pixel) : QColor();
}
