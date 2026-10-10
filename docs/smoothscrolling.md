# Optional smooth scrolling

CMake searches for `SmoothScrollbar` in config mode through the normal
`CMAKE_PREFIX_PATH` and `SmoothScrollbar_DIR` search paths. When the exported
`SmoothScrollbar::SmoothScrollbar` target is available, AnimatedWidgets links it
and enables smooth scrolling automatically. Without the package, native scrolling
remains available; the dependency is not downloaded automatically.

Use a package built for the same Qt version, architecture and build configuration
as AnimatedWidgets. CMake reports whether the integration is enabled. Installed
AnimatedWidgets packages built with this integration also resolve SmoothScrollbar
through `find_dependency()`.

The following scroll areas install an area-owned `sscroll::SmoothScrollController`:

- PopupMenu views, including replacement views and nested menus. This covers the
  ComboBox, SearchComboBox, MultiSelectComboBox and SearchPopupMenu result lists.
- PlainTextEdit, including the editor inside CompositePlainTextEdit.
- MultiSelectComboBox's horizontal tag viewport.

Installation is idempotent through `SmoothScrolling::install()`. The controller
handles its own teardown; widgets do not need to uninstall it before destruction.
The controller uses a 300 ms OutCubic wheel transition and the library's default input policies:
native touchpad pixel scrolling, horizontal scrolling when only that axis is
available, Ctrl+wheel preservation, and propagation at scroll boundaries. Existing
scrollbar styles are unchanged. Item views continue to scroll per pixel.

Applications can configure the controller without exposing the optional library
in widget headers, when they link SmoothScrollbar themselves:

```cpp
#include <smoothscroll/smoothscrollcontroller.h>

auto* controller = area->findChild<sscroll::SmoothScrollController*>(
    QString(), Qt::FindDirectChildrenOnly);
if (controller) {
    auto settings = controller->settings();
    settings.animationDuration = 200;
    controller->setSettings(settings);
}
```

For a popup, use its `popupAboutToShow` signal and `popup->view()` as the area.
For multiline input, the input itself is the area. Disabling or changing settings
on one controller affects only that scroll area.
