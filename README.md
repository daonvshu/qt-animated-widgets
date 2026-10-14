# AnimatedWidgets

Qt 5/Qt 6 and C++17 animated QWidget controls managed with CMake.

## Build

```powershell
cmake -S . -B build -DCMAKE_PREFIX_PATH=<Qt-prefix> -DBUILD_EXAMPLES=ON -DBUILD_TESTS=ON
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```

The project imports either Qt 6 or Qt 5 using `find_package(QT NAMES Qt6 Qt5)`.
The first release contains the animation core, theme value types, and eight
explicit button variants.

## Button variants

`animatedwidgets/buttons/variantbuttons.h` is a compatibility umbrella; each
control is defined in its own header/source pair such as `filledbutton.h`
and `filledbutton.cpp`. The public controls are `FilledButton`,
`OutlineButton`, `BorderedButton`, and `HoverButton`, plus four independent
`FilledIconButton`, `OutlineIconButton`, `BorderedIconButton`, and
`HoverIconButton`. Filled variants have no border; outline variants
have no fill; bordered variants draw both; hover variants are transparent at
rest and fill on hover, press, or checked state. The concrete Qt properties
expose only the colors relevant to each surface.

Each visible surface and the icon/text content can have separate normal, hover,
pressed, disabled, and checked colors. Unset hover/pressed/checked colors derive
from the normal color using `darkerStep` (pressed uses twice the step). Disabled
colors have a default or fall back to a faded normal color. Setting
`animateContentColor` enables an independent, interruptible color transition
for both text and tinted icons; disabled content always uses its disabled color.
The five state properties (`normalStyle`, `hoverStyle`, `pressedStyle`,
`disabledStyle`, and `checkedStyle`) use the `ButtonStateStyle` value type,
which groups `background`, `border`, and `content` colors. Each concrete
control applies only the surfaces it supports. Qt Designer does not provide a
native editor for custom gadget value types, so the Designer property panel
uses the corresponding state-specific `QColor` properties while C++ can use
the grouped style API.
Text buttons support `iconSpacing` and four `iconPlacement` values (before,
after, above, below text). Icon buttons ignore text and expose `shape` for
circle or rounded rectangle. All variants retain `QPushButton`'s checkable and
icon APIs.

The gallery has a Buttons page showing each variant in interactive, checked,
and disabled states, with layout, spacing, depth, content animation, and icon
shape controls.

The shared `AnimatedButtonBase` only owns state detection and interruptible
color animations. `buttonpainter` contains geometry and pixel drawing helpers.
Each concrete button selects its own state targets and only exposes properties
that its surface actually uses; no concrete class branches on another variant's
background, border, or text configuration.

## Switch

`Switch` is a two-state `QCheckBox` with a sliding thumb and animated track and
thumb colors. It supports text, keyboard toggling, right-to-left layout, and
separate checked, unchecked, and disabled colors. Track size, thumb margin,
handle radius, text spacing, and animation duration are configurable. Set
`handleIcon` to replace the circular handle; its `QIcon` On/Off and Disabled
images are used when available, while the handle color properties apply to the
default circle. A zero handle radius restores automatic sizing. Repeated toggles
continue from the current animation value. The gallery shows it on a separate
Switches page, and Qt Designer registers it in the `animated-widgets` group.

## ProgressBar

`ProgressBar` keeps `QProgressBar`'s range, value, text, orientation, and style
behavior while animating the displayed fill and formatted value. Use the
`realValue` property for fractional targets; the inherited integer `value()`
and `valueChanged` remain available for native Qt compatibility. `realValueChanged`
reports fractional targets immediately. Repeated value
updates redirect the animation from the currently displayed value. Animation is
enabled by default; `animationEnabled`, `animationDuration` (milliseconds), and
`animationEasingCurve` (`QEasingCurve::Type`) configure it. A zero duration or
disabled animation snaps the display to the target. Range changes and busy
mode also synchronize immediately. `valueDecimals` controls the displayed
decimal places of `%v` and `%p` (default 1). The gallery has a Progress page, and the
Designer plugin exposes `ProgressBar` in the `animated-widgets` group.
`trackColor`, `progressColor`, `borderColor`, `textColor`, `borderWidth`, and
`cornerRadius` generate local QSS rules for the configured parts. Colors and
radius are unset by default, leaving the native style or application QSS in
control. `borderWidth` applies when `borderColor` is set. Existing widget QSS
is kept before the generated rules; generated declarations take precedence
over earlier rules for the same part.

## Slider

`Slider` retains `QSlider`'s integer range, orientation, ticks, and input
behavior. Its logical `value()` and `valueChanged` update immediately, while
programmatic, keyboard, and wheel changes animate the displayed handle from
its current position. Pointer presses and drags use the native position
immediately so the handle stays under the pointer. `animationEnabled` defaults
to true; `animationDuration` (milliseconds) and `animationEasingCurve`
(`QEasingCurve::Type`) control the position transition. A zero duration or
disabled animation snaps to the target. The gallery has a Sliders page, and
Designer registers `Slider` in the `animated-widgets` group.

## Inputs

`LineEdit` keeps native `QLineEdit` editing, selection, validator, and context
menu behavior. Its border and background colors transition on hover, focus,
error, and disabled state changes. `hasError` is controlled explicitly by the
caller and does not clear when text changes. Border and background colors,
text colors, border width, corner radius, content padding, and animation
duration are configurable.

`IconLineEdit` positions independent leading and trailing icon buttons using
`contentPadding` from the input edges and `iconSpacing` between each icon and
the text. `leadingIconSize` and `trailingIconSize` set their sizes independently;
both sides retain visibility properties and click signals. `PasswordLineEdit` adds a reveal
button, configurable eye icons, and optional timed auto-hide; it hides the
password again on focus loss. These classes have separate headers under
`animatedwidgets/inputs/`. The gallery has an Inputs page, and all three
classes are registered in Qt Designer.

`CompositeLineEdit` places an `IconLineEdit` between optional leading and
trailing widgets and paints one shared animated border for the whole group.
Use `setLeadingWidget` / `setTrailingWidget` for existing widgets, or
`setLeadingWidgetFactory` / `setTrailingWidgetFactory` to create them with the
container as parent. Installed widgets belong to the container; replacing one
deletes the old widget later. Focus on any descendant activates the shared
focus border, while `hasError` and disabled state override it. The `editor()`
accessor exposes the native editing and icon APIs. Designer previews the empty
container; arbitrary factory content is supplied by application code.

## Qt Designer plugin

`BUILD_DESIGNER_PLUGIN` is ON by default. The `animatedwidgetsdesigner` module
registers the controls with short namespaced names such as `aw::FilledButton`
and distinct icons in the `animated-widgets` group. Designer adapters have
matching meta-object names to avoid collisions with other widget plugins.
The public classes remain in `animatedwidgets`; widget headers expose the
`aw` namespace alias for generated C++ code. Existing unqualified C++ aliases
remain available. Use the `aw::` names in existing forms' widget declarations
and custom widget definitions when migrating from unqualified Designer names.
Stylesheet type selectors continue to use `animatedwidgets--FilledButton`
and the corresponding names of the other public classes.
More widget adapters can be added under
`plugins/designer/` and registered
in `plugins/designer/plugins.cpp`.

The plugin is installed to `<prefix>/lib/qtplugins/designer` (or the platform's
CMake install libdir). Point Qt Designer at `<prefix>/lib/qtplugins` through
`QT_PLUGIN_PATH`, or place the plugin in the matching Qt installation's
`plugins/designer` directory. Use a plugin built with the same Qt version and
compiler as Designer. Applications loading generated `.ui` files must still
link `AnimatedWidgets::Widgets` and have the installed headers available.
