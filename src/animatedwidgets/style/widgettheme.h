#pragma once

#include "animatedwidgets/core/animationspec.h"
#include "colorpalette.h"

namespace animatedwidgets {

struct WidgetTheme {
    ColorPalette colors;
    AnimationSpec animation = {300, QEasingCurve::OutCubic};
    int cornerRadius = 6;
    qreal borderWidth = 1.0;
    int horizontalPadding = 16;
    int verticalPadding = 8;
};

} // namespace animatedwidgets
