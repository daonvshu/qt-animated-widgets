#include "animatedwidgets/utils/buttonstatecolors.h"

#include "animatedwidgets/style/colorpalette.h"

namespace animatedwidgets {

QColor ButtonStateColorUtils::darker(const QColor& color, int darkerStep, int multiplier)
{
    return color.darker(100 + qMax(0, multiplier) * qMax(0, darkerStep));
}

QColor ButtonStateColorUtils::resolve(const ButtonStateColors& colors, ButtonState state, int darkerStep)
{
    switch (state) {
    case ButtonState::Hover:
        if (colors.hover.isValid()) return colors.hover;
        return darker(colors.normal, darkerStep);
    case ButtonState::Pressed:
        if (colors.pressed.isValid()) return colors.pressed;
        if (colors.hover.isValid()) return darker(colors.hover, darkerStep);
        return darker(colors.normal, darkerStep, 2);
    case ButtonState::Checked:
        if (colors.checked.isValid()) return colors.checked;
        if (colors.pressed.isValid()) return colors.pressed;
        if (colors.hover.isValid()) return darker(colors.hover, darkerStep);
        return darker(colors.normal, darkerStep, 2);
    case ButtonState::Disabled:
        if (colors.disabled.isValid()) return colors.disabled;
        return colors.normal;
    case ButtonState::Normal:
        break;
    }
    return colors.normal;
}

QColor ButtonStateColorUtils::resolveHoverOnly(const ButtonStateColors& colors, ButtonState state, int darkerStep)
{
    const QColor& activeFill = colors.normal;
    switch (state) {
    case ButtonState::Normal:
        break;
    case ButtonState::Hover:
        if (colors.hover.isValid()) return colors.hover;
        return activeFill;
    case ButtonState::Pressed:
        if (colors.pressed.isValid()) return colors.pressed;
        if (colors.hover.isValid()) return darker(colors.hover, darkerStep);
        return darker(activeFill, darkerStep, 2);
    case ButtonState::Checked:
        if (colors.checked.isValid()) return colors.checked;
        if (colors.pressed.isValid()) return colors.pressed;
        if (colors.hover.isValid()) return darker(colors.hover, darkerStep);
        return darker(activeFill, darkerStep, 2);
    case ButtonState::Disabled:
        if (colors.disabled.isValid()) return colors.disabled;
        return QColor(Qt::transparent);
    }
    return QColor(Qt::transparent);
}

QColor ButtonStateColorUtils::defaultActiveFill()
{
    static const QColor color = ColorPalette().accent;
    return color;
}

} // namespace animatedwidgets
