#pragma once

#include <QSize>

class QWidget;

class StylePreviewUtils {
public:
    static void applyState(QWidget* preview, int state);
    static QSize resolveSize(const QSize& requested, const QSize& suggested, const QSize& hint);
};
