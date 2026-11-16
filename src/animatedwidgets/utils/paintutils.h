#pragma once

#include <QPainter>
#include <QRectF>
#include <QTransform>
#include <QtMath>

namespace animatedwidgets {
namespace paintutils {

struct BorderStroke {
    QRectF rect;
    // 0 表示 cosmetic 画笔（固定 1 设备像素），负数表示不绘制描边
    qreal width = -1.0;
};

// 描边宽度对齐到整数设备像素
inline qreal alignedPhysicalWidth(qreal borderWidth, qreal devicePixelRatio) {
    const qreal dpr = devicePixelRatio > 0 ? devicePixelRatio : 1.0;
    return qMax(1.0, qreal(qRound(borderWidth * dpr)));
}

inline qreal devicePixelRatioOf(const QPainter& painter) {
    return painter.device() ? painter.device()->devicePixelRatioF() : 1.0;
}

// 在控件裁剪区域内对齐描边：
// 1. 把像素中心对齐到裁剪区内的整数/半整数设备像素；
// 2. 缩放后控件起点可能落在小数设备像素上，边缘像素会被裁掉一部分，
//    这里把描边整体收进到完整可见的像素范围内，避免左右边缘发虚或截断。
inline BorderStroke alignStrokeInClip(const QPainter& painter, const QRectF& pathRect,
                                      const QRectF& clipRect, qreal borderWidth) {
    const qreal dpr = devicePixelRatioOf(painter);
    const qreal physicalWidth = alignedPhysicalWidth(borderWidth, dpr);
    const qreal penWidth = physicalWidth <= 1.0 ? 0.0 : physicalWidth / dpr;
    const int widthPixels = qMax(1, int(qRound(physicalWidth)));
    const bool oddWidth = widthPixels % 2 != 0;

    const QTransform transform = painter.deviceTransform();
    const QPointF origin = transform.map(QPointF(0.0, 0.0));
    const qreal clipLeft = origin.x() + clipRect.left() * dpr;
    const qreal clipTop = origin.y() + clipRect.top() * dpr;
    const qreal clipRight = origin.x() + clipRect.right() * dpr;
    const qreal clipBottom = origin.y() + clipRect.bottom() * dpr;

    const auto alignAxis = [&](qreal localValue, qreal base, qreal clipStart, qreal clipEnd,
                               bool leading) -> qreal {
        const qreal current = base + localValue * dpr;
        if (oddWidth) {
            const qreal minCenter = std::ceil(clipStart) + 0.5;
            const qreal maxCenter = std::floor(clipEnd) - 0.5;
            qreal center = leading ? std::max(current, minCenter) : std::min(current, maxCenter);
            center = std::round(center - 0.5) + 0.5;
            center = qBound(minCenter, center, maxCenter);
            return (center - base) / dpr;
        }
        const qreal half = physicalWidth / 2.0;
        const qreal minCenter = std::ceil(clipStart) + half;
        const qreal maxCenter = std::floor(clipEnd) - half;
        qreal center = leading ? std::max(current, minCenter) : std::min(current, maxCenter);
        center = std::round(center - half) + half;
        center = qBound(minCenter, center, maxCenter);
        return (center - base) / dpr;
    };

    const qreal left = alignAxis(pathRect.left(), origin.x(), clipLeft, clipRight, true);
    const qreal right = alignAxis(pathRect.right(), origin.x(), clipLeft, clipRight, false);
    const qreal top = alignAxis(pathRect.top(), origin.y(), clipTop, clipBottom, true);
    const qreal bottom = alignAxis(pathRect.bottom(), origin.y(), clipTop, clipBottom, false);

    const QRectF rect(left, top, qMax(qreal(0), right - left), qMax(qreal(0), bottom - top));
    return { rect, penWidth };
}

// 从控件外框计算内缩后的描边路径
inline BorderStroke borderStrokeForWidget(const QPainter& painter, const QRectF& widgetRect,
                                         qreal borderWidth) {
    const qreal dpr = devicePixelRatioOf(painter);
    const qreal physicalWidth = alignedPhysicalWidth(borderWidth, dpr);
    const qreal inset = physicalWidth / (2.0 * dpr);
    return alignStrokeInClip(painter, widgetRect.adjusted(inset, inset, -inset, -inset),
                             widgetRect, borderWidth);
}

// 竖直分隔线落在半像素中心
inline qreal alignVerticalLine(qreal x, qreal devicePixelRatio) {
    const qreal dpr = devicePixelRatio > 0 ? devicePixelRatio : 1.0;
    return (std::floor(x * dpr) + 0.5) / dpr;
}

// 水平分隔线落在半像素中心
inline qreal alignHorizontalLine(qreal y, qreal devicePixelRatio) {
    const qreal dpr = devicePixelRatio > 0 ? devicePixelRatio : 1.0;
    return (std::floor(y * dpr) + 0.5) / dpr;
}

} // namespace paintutils
} // namespace animatedwidgets
