#pragma once

#include <QColor>
#include <QFont>
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

QSize buttonContentSize(const ButtonContentOptions& options);
void drawButtonSurface(QPainter& painter, const QRectF& rect, const ButtonSurfaceOptions& options);
void drawButtonContent(QPainter& painter, const QRect& bounds, const ButtonContentOptions& options);

} // namespace animatedwidgets
