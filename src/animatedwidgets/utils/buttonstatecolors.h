#pragma once

#include "animatedwidgets/buttons/buttonstate.h"

#include <QColor>

namespace animatedwidgets {

struct ButtonStateColors {
    QColor normal;
    QColor hover;
    QColor pressed;
    QColor checked;
    QColor disabled;
};

class ButtonStateColorUtils final {
public:
    static QColor darker(const QColor& color, int darkerStep, int multiplier = 1);
    static QColor resolve(const ButtonStateColors& colors, ButtonState state, int darkerStep);
    // Controls whose fill only exists in active states pass their active fill as the normal color.
    static QColor resolveHoverOnly(const ButtonStateColors& colors, ButtonState state, int darkerStep);
    static QColor defaultActiveFill();
};

} // namespace animatedwidgets
