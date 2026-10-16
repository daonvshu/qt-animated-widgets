#pragma once

#include <QColor>
#include <QImage>
#include <QMargins>
#include <QPainterPath>
#include <QWidget>

namespace animatedwidgets {

class ShadowWidget : public QWidget {
public:
    explicit ShadowWidget(QWidget* parent = nullptr);

    void setShadowColor(const QColor& color);
    void setShadowRadius(int radius);

    void showArrow(bool show = true);
    void resizeContent(int width, int height);
    void setContentFixedSize(int width, int height);
    void setContentFixedWidth(int width);
    void setContentFixedHeight(int height);

protected:
    void setShadowEdges(Qt::Edges edges);
    void setAutoExpandShadowSize(bool enabled);
    void setAutoAdjustShadowPosition(bool enabled);
    void ensureShadowSizeExpanded();

    bool event(QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

    int shadowRadius = 12;
    QColor shadowColor = QColor(QStringLiteral("#a9acb6"));
    int shadowCornerRadius = 8;
    QImage shadowImage;
    Qt::Alignment arrowPosAlign = Qt::AlignHCenter;
    Qt::Edge arrowEdgeAlign = Qt::TopEdge;
    Qt::Edges shadowEdges = Qt::LeftEdge | Qt::RightEdge | Qt::TopEdge | Qt::BottomEdge;
    QColor arrowFilledColor = Qt::white;

    QImage createShadow(qreal blurRadius, const QColor& color, const QSizeF& contentSize, qreal cornerRadius,
                        const QPointF& offset = QPointF()) const;
    QPainterPath createArrowPath() const;
    QMargins shadowMargins() const;

private:
    using QWidget::setFixedHeight;
    using QWidget::setFixedSize;
    using QWidget::setFixedWidth;

    QSize outerSizeForContent(const QSize& contentSize) const;
    void adjustSizeForShadowMargins(const QMargins& oldMargins, const QMargins& newMargins);

    bool m_showArrow = false;
    bool m_autoExpandShadowSize = true;
    bool m_autoAdjustShadowPosition = true;
    bool m_shadowSizeExpanded = false;
    QMargins m_expandedShadowMargins;
    Qt::Edges m_appliedShadowEdges = shadowEdges;
    QSize m_cachedSize;
    qreal m_cachedDpr = 0;
    QColor m_cachedColor;
    int m_cachedRadius = -1;
    int m_cachedCornerRadius = -1;
    Qt::Edges m_cachedEdges;
    Qt::Edge m_cachedArrowEdge = Qt::TopEdge;
    Qt::Alignment m_cachedArrowAlignment;
    bool m_cachedArrowVisible = false;
};

} // namespace animatedwidgets
