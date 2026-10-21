#pragma once

#include <QEasingCurve>

namespace animatedwidgets {

struct AnimationSpec {
    int duration = 300;
    QEasingCurve easing = QEasingCurve::OutCubic;
};

} // namespace animatedwidgets
