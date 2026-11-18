#include "animatedwidgets/utils/buttonpainter.h"
#include "animatedwidgets/utils/iconpainter.h"

#include <QFontMetricsF>
#include <QPainter>
#include <QStyle>

#include "animatedwidgets/utils/paintutils.h"
#include <QtMath>

namespace animatedwidgets {

QSize ButtonPainter::contentSize(const ButtonContentOptions& options) {
    const bool hasIcon = !options.icon.isNull();
    const bool hasText = !options.iconOnly && !options.text.isEmpty();
    const QFontMetricsF metrics(options.font);
    const int textWidth = hasText ? qCeil(metrics.horizontalAdvance(options.text)) : 0;
    const int textHeight = hasText ? qCeil(metrics.height()) : 0;
    const int iconWidth = hasIcon ? options.iconSize.width() : 0;
    const int iconHeight = hasIcon ? options.iconSize.height() : 0;
    const bool vertical = options.placement == ButtonContentOptions::Placement::AboveText || options.placement == ButtonContentOptions::Placement::BelowText;
    const int gap = hasIcon && hasText ? options.spacing : 0;
    return vertical ? QSize(qMax(textWidth, iconWidth), textHeight + iconHeight + gap) : QSize(textWidth + iconWidth + gap, qMax(textHeight, iconHeight));
}

QSize ButtonPainter::textSizeHint(const QSize& contentSize, int horizontalPadding, int verticalPadding) {
    const int horizontalPaddingSize = 2 * qMax(0, horizontalPadding);
    const int verticalPaddingSize = 2 * qMax(0, verticalPadding);
    return QSize(qMax(80, contentSize.width() + qMax(28, horizontalPaddingSize)),
                 qMax(36, contentSize.height() + qMax(16, verticalPaddingSize)));
}

QSize ButtonPainter::iconSizeHint(const QSize& iconSize) {
    const int side = qMax(iconSize.width(), iconSize.height()) + 24;
    return QSize(side, side);
}

QSize ButtonPainter::checkableSizeHint(const QSize& indicatorSize, int spacing, const QFontMetrics& fontMetrics, const QString& text) {
    const QFontMetricsF metrics(fontMetrics);
    const bool hasText = !text.isEmpty();
    const int gap = hasText ? qMax(0, spacing) : 0;
    const int textWidth = hasText ? qCeil(metrics.horizontalAdvance(text)) : 0;
    return QSize(indicatorSize.width() + gap + textWidth + 4,
                 qMax(indicatorSize.height(), qCeil(metrics.height())) + 4);
}

QRectF ButtonPainter::checkableIndicatorRect(const QRectF& bounds, const QSize& indicatorSize, Qt::LayoutDirection direction) {
    const qreal x = direction == Qt::RightToLeft ? bounds.right() - indicatorSize.width() : bounds.left();
    return QRectF(x, bounds.center().y() - indicatorSize.height() / 2.0, indicatorSize.width(), indicatorSize.height());
}

QRectF ButtonPainter::checkableTextRect(const QRectF& bounds, const QSize& indicatorSize, int spacing, Qt::LayoutDirection direction) {
    const qreal x = direction == Qt::RightToLeft ? bounds.left() : bounds.left() + indicatorSize.width() + spacing;
    const qreal width = bounds.width() - indicatorSize.width() - spacing - 1;
    return QRectF(x, bounds.top(), width, bounds.height());
}

void ButtonPainter::drawSurface(QPainter& painter, const QRectF& rect, const ButtonSurfaceOptions& options) {
    painter.setRenderHint(QPainter::Antialiasing, true);
    const auto border = PaintUtils::borderStroke(painter, rect, options.border, options.drawBorder ? options.borderWidth : 0.0);
    painter.setPen(border.pen);
    painter.setBrush(options.drawBackground ? QBrush(options.background) : QBrush(Qt::NoBrush));
    if (options.circle) {
        const qreal side = qMin(border.rect.width(), border.rect.height());
        painter.drawEllipse(QRectF(border.rect.center().x() - side / 2, border.rect.center().y() - side / 2, side, side));
    } else {
        painter.drawRoundedRect(border.rect, options.cornerRadius, options.cornerRadius);
    }
}

void ButtonPainter::drawContent(QPainter& painter, const QRectF& bounds, const ButtonContentOptions& options) {
    const qreal horizontalPadding = qMax(0, options.horizontalPadding);
    const qreal verticalPadding = qMax(0, options.verticalPadding);
    const QRectF available = bounds.adjusted(horizontalPadding, verticalPadding, -horizontalPadding, -verticalPadding);
    if (available.isEmpty())
        return;

    const bool hasIcon = !options.icon.isNull();
    const bool hasText = !options.iconOnly && !options.text.isEmpty();
    const QFontMetricsF metrics(options.font);
    const qreal textHeight = hasText ? metrics.height() : 0.0;
    const qreal iconWidth = hasIcon ? options.iconSize.width() : 0.0;
    const qreal iconHeight = hasIcon ? options.iconSize.height() : 0.0;
    const qreal gap = hasIcon && hasText ? options.spacing : 0.0;
    const bool vertical = options.placement == ButtonContentOptions::Placement::AboveText || options.placement == ButtonContentOptions::Placement::BelowText;
    const bool iconFirst = options.placement == ButtonContentOptions::Placement::BeforeText || options.placement == ButtonContentOptions::Placement::AboveText;
    const qreal textSpace = vertical ? available.width() : qMax(qreal(0), available.width() - iconWidth - gap);
    const QString displayText = hasText ? metrics.elidedText(options.text, Qt::ElideRight, textSpace) : QString();
    const qreal textWidth = hasText ? qMin(metrics.horizontalAdvance(displayText), textSpace) : 0.0;
    const qreal contentWidth = vertical ? qMax(textWidth, iconWidth) : textWidth + iconWidth + gap;
    const qreal contentHeight = vertical ? textHeight + iconHeight + gap : qMax(textHeight, iconHeight);
    const Qt::Alignment alignment = QStyle::visualAlignment(options.direction, options.alignment);
    qreal contentX = available.left() + (available.width() - contentWidth) / 2.0;
    if (alignment.testFlag(Qt::AlignLeft))
        contentX = available.left();
    else if (alignment.testFlag(Qt::AlignRight))
        contentX = available.right() - contentWidth;

    qreal contentY = available.top() + (available.height() - contentHeight) / 2.0;
    if (alignment.testFlag(Qt::AlignTop))
        contentY = available.top();
    else if (alignment.testFlag(Qt::AlignBottom))
        contentY = available.bottom() - contentHeight;

    const QRectF content(contentX, contentY, contentWidth, contentHeight);
    const QRectF iconRect(
            vertical ? QRectF(content.left() + (content.width() - iconWidth) / 2.0, content.top() + (iconFirst ? 0.0 : textHeight + gap), iconWidth, iconHeight)
                     : QRectF(content.left() + (iconFirst ? 0.0 : textWidth + gap), content.top() + (content.height() - iconHeight) / 2.0, iconWidth, iconHeight));
    const QRectF textRect(vertical ? QRectF(content.left(), content.top() + (iconFirst ? iconHeight + gap : 0.0), content.width(), textHeight)
                                   : QRectF(content.left() + (iconFirst ? iconWidth + gap : 0.0), content.top(), textWidth, contentHeight));

    painter.save();
    painter.setClipRect(available, Qt::IntersectClip);

    if (hasIcon) {
        IconPainter::draw(painter, iconRect, options.icon, options.iconHint ? options.color : QColor(),
                          options.enabled ? QIcon::Normal : QIcon::Disabled);
    }
    if (hasText) {
        painter.setPen(options.color);
        painter.setFont(options.font);
        painter.drawText(textRect, Qt::AlignCenter, displayText);
    }
    painter.restore();
}

} // namespace animatedwidgets
