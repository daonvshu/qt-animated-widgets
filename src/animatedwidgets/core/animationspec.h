#pragma once

#include <QEasingCurve>

namespace animatedwidgets {

struct AnimationSpec {
    int duration = 180;
    QEasingCurve easing = QEasingCurve::OutCubic;
};

} // namespace animatedwidgets
