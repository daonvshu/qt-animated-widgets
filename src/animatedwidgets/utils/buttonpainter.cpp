#include "animatedwidgets/utils/buttonpainter.h"

#include <QPainter>
#include <QStyle>

namespace animatedwidgets {

QSize ButtonPainter::contentSize(const ButtonContentOptions& options) {
    const bool hasIcon = !options.icon.isNull();
    const bool hasText = !options.iconOnly && !options.text.isEmpty();
    const QFontMetrics metrics(options.font);
    const int textWidth = hasText ? metrics.horizontalAdvance(options.text) : 0;
    const int textHeight = hasText ? metrics.height() : 0;
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
    return QSize(indicatorSize.width() + spacing + fontMetrics.horizontalAdvance(text) + 4, qMax(indicatorSize.height(), fontMetrics.height()) + 4);
}

QRect ButtonPainter::checkableIndicatorRect(const QRect& bounds, const QSize& indicatorSize, Qt::LayoutDirection direction) {
    const int x = direction == Qt::RightToLeft ? bounds.right() - indicatorSize.width() + 1 : bounds.left();
    return QRect(x, bounds.top() + (bounds.height() - indicatorSize.height()) / 2, indicatorSize.width(), indicatorSize.height());
}

QRect ButtonPainter::checkableTextRect(const QRect& bounds, const QSize& indicatorSize, int spacing, Qt::LayoutDirection direction) {
    const int x = direction == Qt::RightToLeft ? bounds.left() : bounds.left() + indicatorSize.width() + spacing;
    const int width = bounds.width() - indicatorSize.width() - spacing - 1;
    return QRect(x, bounds.top(), width, bounds.height());
}

void ButtonPainter::drawSurface(QPainter& painter, const QRectF& rect, const ButtonSurfaceOptions& options) {
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(options.drawBorder && options.borderWidth > 0 ? QPen(options.border, options.borderWidth) : QPen(Qt::NoPen));
    painter.setBrush(options.drawBackground ? QBrush(options.background) : QBrush(Qt::NoBrush));
    if (options.circle) {
        const qreal side = qMin(rect.width(), rect.height());
        painter.drawEllipse(QRectF(rect.center().x() - side / 2, rect.center().y() - side / 2, side, side));
    } else {
        painter.drawRoundedRect(rect, options.cornerRadius, options.cornerRadius);
    }
}

void ButtonPainter::drawContent(QPainter& painter, const QRect& bounds, const ButtonContentOptions& options) {
    const int horizontalPadding = qMax(0, options.horizontalPadding);
    const int verticalPadding = qMax(0, options.verticalPadding);
    const QRect available = bounds.adjusted(horizontalPadding, verticalPadding, -horizontalPadding, -verticalPadding);
    if (available.isEmpty())
        return;

    const bool hasIcon = !options.icon.isNull();
    const bool hasText = !options.iconOnly && !options.text.isEmpty();
    const QFontMetrics metrics(options.font);
    const int textHeight = hasText ? metrics.height() : 0;
    const int iconWidth = hasIcon ? options.iconSize.width() : 0;
    const int iconHeight = hasIcon ? options.iconSize.height() : 0;
    const int gap = hasIcon && hasText ? options.spacing : 0;
    const bool vertical = options.placement == ButtonContentOptions::Placement::AboveText || options.placement == ButtonContentOptions::Placement::BelowText;
    const bool iconFirst = options.placement == ButtonContentOptions::Placement::BeforeText || options.placement == ButtonContentOptions::Placement::AboveText;
    const int textSpace = vertical ? available.width() : qMax(0, available.width() - iconWidth - gap);
    const int textWidth = hasText ? qMin(metrics.horizontalAdvance(options.text), textSpace) : 0;
    const int contentWidth = vertical ? qMax(textWidth, iconWidth) : textWidth + iconWidth + gap;
    const int contentHeight = vertical ? textHeight + iconHeight + gap : qMax(textHeight, iconHeight);
    const Qt::Alignment alignment = QStyle::visualAlignment(options.direction, options.alignment);
    int contentX = available.left() + (available.width() - contentWidth) / 2;
    if (alignment.testFlag(Qt::AlignLeft))
        contentX = available.left();
    else if (alignment.testFlag(Qt::AlignRight))
        contentX = available.right() - contentWidth + 1;

    int contentY = available.top() + (available.height() - contentHeight) / 2;
    if (alignment.testFlag(Qt::AlignTop))
        contentY = available.top();
    else if (alignment.testFlag(Qt::AlignBottom))
        contentY = available.bottom() - contentHeight + 1;

    const QRect content(contentX, contentY, contentWidth, contentHeight);
    const QRect iconRect(
            vertical ? QRect(content.left() + (content.width() - iconWidth) / 2, content.top() + (iconFirst ? 0 : textHeight + gap), iconWidth, iconHeight)
                     : QRect(content.left() + (iconFirst ? 0 : textWidth + gap), content.top() + (content.height() - iconHeight) / 2, iconWidth, iconHeight));
    const QRect textRect(vertical ? QRect(content.left(), content.top() + (iconFirst ? iconHeight + gap : 0), content.width(), textHeight)
                                  : QRect(content.left() + (iconFirst ? iconWidth + gap : 0), content.top(), textWidth, content.height()));

    painter.save();
    painter.setClipRect(available, Qt::IntersectClip);

    if (hasIcon) {
        QPixmap pixmap = options.icon.pixmap(options.iconSize, QIcon::Normal, QIcon::Off);
        if (options.color.isValid() && !pixmap.isNull()) {
            QPainter tint(&pixmap);
            tint.setCompositionMode(QPainter::CompositionMode_SourceIn);
            tint.fillRect(pixmap.rect(), options.color);
        }
        painter.drawPixmap(iconRect, pixmap);
    }
    if (hasText) {
        painter.setPen(options.color);
        painter.setFont(options.font);
        painter.drawText(textRect, Qt::AlignCenter, metrics.elidedText(options.text, Qt::ElideRight, textRect.width()));
    }
    painter.restore();
}

} // namespace animatedwidgets
