#pragma once

#include <QColor>
#include <QImage>
#include <QPointF>
#include <QRect>

class ScreenColorUtils {
public:
    static QPoint imagePosition(const QSize& imageSize, const QRect& screenGeometry, const QPointF& globalPosition);
    static QColor colorAt(const QImage& image, const QRect& screenGeometry, const QPointF& globalPosition);
};
