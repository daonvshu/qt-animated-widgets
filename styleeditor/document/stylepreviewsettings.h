#pragma once

#include <QColor>
#include <QSize>

struct StylePreviewSettings {
    QSize size;
    QColor background;
    bool widthAuto = false;
    bool heightAuto = false;
};
