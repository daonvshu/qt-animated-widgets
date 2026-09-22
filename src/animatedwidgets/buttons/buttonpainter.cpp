#include "animatedwidgets/buttons/buttonpainter.h"

#include <QPainter>

namespace animatedwidgets {

QSize buttonContentSize(const ButtonContentOptions& options)
{
    const bool hasIcon = !options.icon.isNull();
    const bool hasText = !options.iconOnly && !options.text.isEmpty();
    const QFontMetrics metrics(options.font);
    const int textWidth = hasText ? metrics.horizontalAdvance(options.text) : 0;
    const int textHeight = hasText ? metrics.height() : 0;
    const int iconWidth = hasIcon ? options.iconSize.width() : 0;
    const int iconHeight = hasIcon ? options.iconSize.height() : 0;
    const bool vertical = options.placement == ButtonContentOptions::Placement::AboveText
                       || options.placement == ButtonContentOptions::Placement::BelowText;
    const int gap = hasIcon && hasText ? options.spacing : 0;
    return vertical
        ? QSize(qMax(textWidth, iconWidth), textHeight + iconHeight + gap)
        : QSize(textWidth + iconWidth + gap, qMax(textHeight, iconHeight));
}

void drawButtonSurface(QPainter& painter, const QRectF& rect, const ButtonSurfaceOptions& options)
{
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(options.drawBorder && options.borderWidth > 0
                       ? QPen(options.border, options.borderWidth)
                       : QPen(Qt::NoPen));
    painter.setBrush(options.drawBackground ? QBrush(options.background) : QBrush(Qt::NoBrush));
    if (options.circle) {
        const qreal side = qMin(rect.width(), rect.height());
        painter.drawEllipse(QRectF(rect.center().x() - side / 2,
                                   rect.center().y() - side / 2, side, side));
    } else {
        painter.drawRoundedRect(rect, options.cornerRadius, options.cornerRadius);
    }
}

void drawButtonContent(QPainter& painter, const QRect& bounds, const ButtonContentOptions& options)
{
    const bool hasIcon = !options.icon.isNull();
    const bool hasText = !options.iconOnly && !options.text.isEmpty();
    const QFontMetrics metrics(options.font);
    const int textWidth = hasText ? metrics.horizontalAdvance(options.text) : 0;
    const int textHeight = hasText ? metrics.height() : 0;
    const int iconWidth = hasIcon ? options.iconSize.width() : 0;
    const int iconHeight = hasIcon ? options.iconSize.height() : 0;
    const int gap = hasIcon && hasText ? options.spacing : 0;
    const bool vertical = options.placement == ButtonContentOptions::Placement::AboveText
                       || options.placement == ButtonContentOptions::Placement::BelowText;
    const bool iconFirst = options.placement == ButtonContentOptions::Placement::BeforeText
                        || options.placement == ButtonContentOptions::Placement::AboveText;
    const int contentWidth = vertical ? qMax(textWidth, iconWidth) : textWidth + iconWidth + gap;
    const int contentHeight = vertical ? textHeight + iconHeight + gap : qMax(textHeight, iconHeight);
    const QRect content(bounds.center().x() - contentWidth / 2,
                        bounds.center().y() - contentHeight / 2, contentWidth, contentHeight);
    const QRect iconRect(vertical
                             ? QRect(content.center().x() - iconWidth / 2,
                                     content.top() + (iconFirst ? 0 : textHeight + gap),
                                     iconWidth, iconHeight)
                             : QRect(content.left() + (iconFirst ? 0 : textWidth + gap),
                                     content.center().y() - iconHeight / 2,
                                     iconWidth, iconHeight));
    const QRect textRect(vertical
                             ? QRect(content.left(),
                                     content.top() + (iconFirst ? iconHeight + gap : 0),
                                     content.width(), textHeight)
                             : QRect(content.left() + (iconFirst ? iconWidth + gap : 0),
                                     content.top(), textWidth, content.height()));

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
        painter.drawText(textRect, Qt::AlignCenter,
                         metrics.elidedText(options.text, Qt::ElideRight, textRect.width()));
    }
}

} // namespace animatedwidgets
