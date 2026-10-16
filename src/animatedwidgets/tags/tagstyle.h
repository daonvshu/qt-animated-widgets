#pragma once

#include <QColor>
#include <QIcon>
#include <QSize>

namespace animatedwidgets {

struct TagStyle {
    QColor background = QColor(QStringLiteral("#f2f3f5"));
    QColor border = Qt::transparent;
    QColor content = QColor(QStringLiteral("#404752"));
    QColor disabledContent = QColor(QStringLiteral("#9298a2"));
    int borderWidth = 0;
    int cornerRadius = 3;
    int contentPadding = 6;
    QSize iconSize = QSize(14, 14);
    int iconSpacing = 4;
    QIcon closeIcon; // A null icon uses the built-in close SVG.
    QSize closeIconSize = QSize(12, 12);
    int closeButtonSize = 18;
    bool closeButtonVisible = true;
    bool iconHint = false;
};

} // namespace animatedwidgets
