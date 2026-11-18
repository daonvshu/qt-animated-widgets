#include "animatedwidgets/popups/shadowwidget.h"

#include <QEvent>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include <qmath.h>

// Qt internal function (qtbase/src/widgets/effects/qpixmapfilter.cpp).
extern void qt_blurImage(QPainter* painter, QImage& image, qreal radius, bool quality, bool alphaOnly, int transposed);

namespace animatedwidgets {

ShadowWidget::ShadowWidget(QWidget* parent)
    : QWidget(parent)
{
    setContentsMargins(shadowMargins());
}

void ShadowWidget::setShadowColor(const QColor& color) {
    if (shadowColor == color)
        return;

    shadowColor = color;
    shadowImage = QImage();
    update();
}

void ShadowWidget::showArrow(bool show) {
    m_showArrow = show;
    shadowImage = QImage();
    update();
}

void ShadowWidget::resizeContent(int width, int height) {
    resize(outerSizeForContent(QSize(width, height)));
}

void ShadowWidget::setContentFixedSize(int width, int height) {
    setFixedSize(outerSizeForContent(QSize(width, height)));
}

void ShadowWidget::setContentFixedWidth(int width) {
    setFixedWidth(outerSizeForContent(QSize(width, 0)).width());
}

void ShadowWidget::setContentFixedHeight(int height) {
    setFixedHeight(outerSizeForContent(QSize(0, height)).height());
}

void ShadowWidget::setShadowRadius(int radius) {
    const QMargins oldMargins = m_expandedShadowMargins;
    shadowRadius = qMax(0, radius);
    shadowImage = QImage();
    const QMargins newMargins = shadowMargins();
    setContentsMargins(newMargins);
    if (m_shadowSizeExpanded) {
        m_expandedShadowMargins = newMargins;
        adjustSizeForShadowMargins(oldMargins, newMargins);
    }
    update();
}

void ShadowWidget::setShadowEdges(Qt::Edges edges) {
    if (shadowEdges == edges && m_appliedShadowEdges == edges)
        return;

    const QMargins oldMargins = m_expandedShadowMargins;
    shadowEdges = edges;
    m_appliedShadowEdges = edges;
    const QMargins newMargins = shadowMargins();
    setContentsMargins(newMargins);
    if (m_shadowSizeExpanded) {
        m_expandedShadowMargins = newMargins;
        adjustSizeForShadowMargins(oldMargins, newMargins);
    }
    shadowImage = QImage();
    update();
}

void ShadowWidget::setAutoExpandShadowSize(bool enabled) {
    m_autoExpandShadowSize = enabled;
}

void ShadowWidget::setAutoAdjustShadowPosition(bool enabled) {
    m_autoAdjustShadowPosition = enabled;
}

bool ShadowWidget::event(QEvent* event) {
    if (event->type() == QEvent::Show) {
        if (shadowEdges != m_appliedShadowEdges)
            setShadowEdges(shadowEdges);
        ensureShadowSizeExpanded();
    }
    return QWidget::event(event);
}

void ShadowWidget::paintEvent(QPaintEvent*) {
    const QMargins margins = shadowMargins();
    const QRect content = rect().adjusted(margins.left(), margins.top(), -margins.right(), -margins.bottom());
    const qreal dpr = devicePixelRatioF();
    if (shadowImage.isNull() || m_cachedSize != size() || !qFuzzyCompare(m_cachedDpr, dpr)
        || m_cachedColor != shadowColor || m_cachedRadius != shadowRadius
        || m_cachedCornerRadius != shadowCornerRadius || m_cachedEdges != shadowEdges
        || m_cachedArrowEdge != arrowEdgeAlign || m_cachedArrowAlignment != arrowPosAlign
        || m_cachedArrowVisible != m_showArrow) {
        shadowImage = createShadow(shadowRadius, shadowColor, content.size(), shadowCornerRadius);
        m_cachedSize = size();
        m_cachedDpr = dpr;
        m_cachedColor = shadowColor;
        m_cachedRadius = shadowRadius;
        m_cachedCornerRadius = shadowCornerRadius;
        m_cachedEdges = shadowEdges;
        m_cachedArrowEdge = arrowEdgeAlign;
        m_cachedArrowAlignment = arrowPosAlign;
        m_cachedArrowVisible = m_showArrow;
    }

    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.drawImage(QPointF(margins.left() - shadowRadius, margins.top() - shadowRadius), shadowImage);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillPath(createArrowPath(), arrowFilledColor);
    painter.restore();

    QStyleOption option;
    option.initFrom(this);
    option.rect = content;
    style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);
}

QImage ShadowWidget::createShadow(qreal blurRadius, const QColor& color, const QSizeF& contentSize,
                                  qreal cornerRadius, const QPointF& offset) const {
    if (contentSize.isEmpty() || blurRadius <= 0 || color.alpha() == 0)
        return QImage();

    const qreal dpr = devicePixelRatioF();
    const QSizeF logicalSize = contentSize + QSizeF(2 * blurRadius, 2 * blurRadius);
    const QSize pixelSize(qMax(1, qCeil(logicalSize.width() * dpr)),
                          qMax(1, qCeil(logicalSize.height() * dpr)));
    QImage mask(pixelSize, QImage::Format_ARGB32_Premultiplied);
    mask.fill(Qt::transparent);

    QPainterPath path;
    path.addRoundedRect(QRectF(QPointF(blurRadius, blurRadius) + offset, contentSize), cornerRadius, cornerRadius);
    QPainterPath arrow = createArrowPath();
    const QMargins margins = shadowMargins();
    arrow.translate(blurRadius - margins.left(), blurRadius - margins.top());
    path.addPath(arrow);
    {
        QPainter painter(&mask);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.scale(dpr, dpr);
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::white);
        painter.drawPath(path);
    }

    QImage shadow(pixelSize, QImage::Format_ARGB32_Premultiplied);
    shadow.fill(Qt::transparent);
    {
        QPainter painter(&shadow);
        qt_blurImage(&painter, mask, blurRadius * dpr, false, true, 0);
    }
    {
        QPainter painter(&shadow);
        painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
        painter.fillRect(shadow.rect(), color);
    }
    shadow.setDevicePixelRatio(dpr);
    return shadow;
}

QPainterPath ShadowWidget::createArrowPath() const {
    QPainterPath path;
    if (!m_showArrow)
        return path;

    const QMargins margins = shadowMargins();
    const QRectF content = QRectF(rect()).adjusted(margins.left(), margins.top(), -margins.right(), -margins.bottom());
    const qreal inset = shadowCornerRadius + 12;
    const qreal halfWidth = 7;
    const qreal tipRadius = 2;
    QPointF base;

    switch (arrowEdgeAlign) {
    case Qt::TopEdge:
    case Qt::BottomEdge:
        base.setY(arrowEdgeAlign == Qt::TopEdge ? content.top() : content.bottom());
        base.setX(arrowPosAlign & Qt::AlignLeft ? content.left() + inset
                  : arrowPosAlign & Qt::AlignRight ? content.right() - inset : content.center().x());
        break;
    case Qt::LeftEdge:
    case Qt::RightEdge:
        base.setX(arrowEdgeAlign == Qt::LeftEdge ? content.left() : content.right());
        base.setY(arrowPosAlign & Qt::AlignTop ? content.top() + inset
                  : arrowPosAlign & Qt::AlignBottom ? content.bottom() - inset : content.center().y());
        break;
    }

    switch (arrowEdgeAlign) {
    case Qt::TopEdge:
        path.moveTo(base.x() - halfWidth, base.y());
        path.lineTo(base.x(), base.y() - halfWidth + tipRadius);
        path.quadTo(base.x(), base.y() - halfWidth, base.x() + tipRadius, base.y() - halfWidth + tipRadius);
        path.lineTo(base.x() + halfWidth, base.y());
        break;
    case Qt::BottomEdge:
        path.moveTo(base.x() - halfWidth, base.y());
        path.lineTo(base.x(), base.y() + halfWidth - tipRadius);
        path.quadTo(base.x(), base.y() + halfWidth, base.x() + tipRadius, base.y() + halfWidth - tipRadius);
        path.lineTo(base.x() + halfWidth, base.y());
        break;
    case Qt::LeftEdge:
        path.moveTo(base.x(), base.y() - halfWidth);
        path.lineTo(base.x() - halfWidth + tipRadius, base.y());
        path.quadTo(base.x() - halfWidth, base.y(), base.x() - halfWidth + tipRadius, base.y() + tipRadius);
        path.lineTo(base.x(), base.y() + halfWidth);
        break;
    case Qt::RightEdge:
        path.moveTo(base.x(), base.y() - halfWidth);
        path.lineTo(base.x() + halfWidth - tipRadius, base.y());
        path.quadTo(base.x() + halfWidth, base.y(), base.x() + halfWidth - tipRadius, base.y() + tipRadius);
        path.lineTo(base.x(), base.y() + halfWidth);
        break;
    }
    path.closeSubpath();
    return path;
}

QMargins ShadowWidget::shadowMargins() const {
    return QMargins(shadowEdges & Qt::LeftEdge ? shadowRadius : 0,
                    shadowEdges & Qt::TopEdge ? shadowRadius : 0,
                    shadowEdges & Qt::RightEdge ? shadowRadius : 0,
                    shadowEdges & Qt::BottomEdge ? shadowRadius : 0);
}

QSize ShadowWidget::outerSizeForContent(const QSize& contentSize) const {
    if (!m_shadowSizeExpanded)
        return contentSize;
    return contentSize + QSize(m_expandedShadowMargins.left() + m_expandedShadowMargins.right(),
                               m_expandedShadowMargins.top() + m_expandedShadowMargins.bottom());
}

void ShadowWidget::adjustSizeForShadowMargins(const QMargins& oldMargins, const QMargins& newMargins) {
    const QSize delta(newMargins.left() + newMargins.right() - oldMargins.left() - oldMargins.right(),
                      newMargins.top() + newMargins.bottom() - oldMargins.top() - oldMargins.bottom());
    const QPoint positionDelta(newMargins.left() - oldMargins.left(), newMargins.top() - oldMargins.top());
    if (delta.isNull() && positionDelta.isNull())
        return;

    const QSize oldSize = size();
    const QPoint oldPosition = pos();
    const QSize oldMinimum = minimumSize();
    const QSize oldMaximum = maximumSize();
    const QSize newMinimum(qBound(0, oldMinimum.width() + delta.width(), QWIDGETSIZE_MAX),
                           qBound(0, oldMinimum.height() + delta.height(), QWIDGETSIZE_MAX));
    const QSize newMaximum(oldMaximum.width() == QWIDGETSIZE_MAX ? QWIDGETSIZE_MAX
                               : qBound(newMinimum.width(), oldMaximum.width() + delta.width(), QWIDGETSIZE_MAX),
                           oldMaximum.height() == QWIDGETSIZE_MAX ? QWIDGETSIZE_MAX
                               : qBound(newMinimum.height(), oldMaximum.height() + delta.height(), QWIDGETSIZE_MAX));
    setMinimumSize(newMinimum);
    setMaximumSize(newMaximum);
    resize(qMax(newMinimum.width(), oldSize.width() + delta.width()),
           qMax(newMinimum.height(), oldSize.height() + delta.height()));
    if (m_autoAdjustShadowPosition)
        move(oldPosition - positionDelta);
}

void ShadowWidget::ensureShadowSizeExpanded() {
    if (m_shadowSizeExpanded || !m_autoExpandShadowSize)
        return;
    m_expandedShadowMargins = shadowMargins();
    m_shadowSizeExpanded = true;
    adjustSizeForShadowMargins(QMargins(), m_expandedShadowMargins);
}

} // namespace animatedwidgets
