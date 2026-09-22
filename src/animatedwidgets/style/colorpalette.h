#pragma once

#include <QColor>

namespace animatedwidgets {

struct ColorPalette {
    QColor background = QColor(QStringLiteral("#ffffff"));
    QColor foreground = QColor(QStringLiteral("#202124"));
    QColor accent = QColor(QStringLiteral("#3478f6"));
    QColor accentHover = QColor(QStringLiteral("#2864d7"));
    QColor border = QColor(QStringLiteral("#c8ccd4"));
    QColor disabled = QColor(QStringLiteral("#a8adb7"));
    QColor hoverBackground;
    QColor pressedBackground;
    QColor checkedBackground;
    QColor disabledBackground;
    QColor hoverBorder;
    QColor pressedBorder;
    QColor checkedBorder;
    QColor disabledBorder;
    QColor focusBorder;
    QColor hoverForeground;
    QColor pressedForeground;
    QColor checkedForeground;
    QColor disabledForeground;
};

} // namespace animatedwidgets
