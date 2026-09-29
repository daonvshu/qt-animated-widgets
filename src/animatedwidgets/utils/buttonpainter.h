#pragma once

#include <QColor>
#include <QFont>
#include <QFontMetrics>
#include <QIcon>
#include <QRect>
#include <QSize>
#include <QString>

class QPainter;

namespace animatedwidgets {

struct ButtonContentOptions {
    QIcon icon;
    QSize iconSize;
    QString text;
    QFont font;
    QColor color;
    int spacing = 6;
    bool iconOnly = false;
    enum class Placement { BeforeText, AfterText, AboveText, BelowText } placement = Placement::BeforeText;
};

struct ButtonSurfaceOptions {
    QColor background;
    QColor border;
    qreal borderWidth = 0.0;
    int cornerRadius = 0;
    bool drawBackground = false;
    bool drawBorder = false;
    bool circle = false;
};

class ButtonPainter final {
public:
    static QSize contentSize(const ButtonContentOptions& options);
    static void drawSurface(QPainter& painter, const QRectF& rect, const ButtonSurfaceOptions& options);
    static void drawContent(QPainter& painter, const QRect& bounds, const ButtonContentOptions& options);

    static QSize textSizeHint(const QSize& contentSize);
    static QSize iconSizeHint(const QSize& iconSize);

    static QSize checkableSizeHint(const QSize& indicatorSize, int spacing,
                                   const QFontMetrics& fontMetrics, const QString& text);
    static QRect checkableIndicatorRect(const QRect& bounds, const QSize& indicatorSize,
                                        Qt::LayoutDirection direction);
    static QRect checkableTextRect(const QRect& bounds, const QSize& indicatorSize, int spacing,
                                   Qt::LayoutDirection direction);
};

} // namespace animatedwidgets
