# MultiSelectComboBox

`MultiSelectComboBox` is a single-line, multi-selection tag input. It inherits
`CompositeInputBase` for animated borders, backgrounds, error/disabled states and
optional widgets on all four sides. It does not inherit `QComboBox`.

```cpp
#include "animatedwidgets/comboboxes/multiselectcombobox.h"
#include "animatedwidgets/popups/defaultpopupmenumodel.h"

auto* combo = new animatedwidgets::MultiSelectComboBox(parent);
combo->setStrings({"Design", "Development", "Testing", "Delivery"});
combo->setSelectedIndexes({0, 1});
combo->setPlaceholderText("Filter teams");
combo->setMaxVisibleItems(6);

connect(combo, &animatedwidgets::MultiSelectComboBox::selectedIndexesChanged,
        receiver, [combo](const QList<int>& indexes) {
    for (int itemIndex : indexes) {
        const QModelIndex item = combo->model()->indexForItem(itemIndex);
        const QString text = item.data(Qt::DisplayRole).toString();
        // Read custom roles from this same source-model index.
    }
});
```

The persistent `DefaultPopupMenuModel` stores selection. Logical indexes exclude
separators and are sorted in source-item order. Filtering changes visible rows,
not selection. Use `setItems()` to provide icons, enabled states and custom roles.
Replacing the items resets selection; assigning identical strings preserves it.
The built-in model is configured for multi-selection; keep that mode unchanged.

`setSearchable(false)` hides the editor and leaves tags plus the dropdown button.
When searchable, clicking the input or typing opens the list without moving
keyboard focus away from the editor. Selecting an option toggles it and leaves the
list open. Outside click, focus leaving the control, or Escape closes the list and
clears the query. Up/Down moves the highlighted option, Enter toggles it, and
Backspace with an empty query removes the last displayed tag. Input-method preedit
is left to the editor. Tag close buttons remove individual selections; the clear
button removes all selections. Disabled options cannot be toggled in the list.

Tags stay on one line. The tag viewport scrolls horizontally when necessary;
mouse-wheel scrolling is available and the editor is brought into view after
layout or selection changes. `contentPadding` sets the outer horizontal inset;
`tagSpacing` sets the gap between tags and the editor.

## Tag styles

`TagWidget` is independently usable and exposes text, icon, background, border,
content/disabled-content colors, radius, border width, padding, icon size/spacing,
and close-button visibility, icon and size. The default close icon is an SVG in the
existing input resources.

Use `setTagStyle()` for a shared style or a factory for per-item styles:

```cpp
combo->setTagStyleFactory([](const QModelIndex& item) {
    animatedwidgets::TagStyle style;
    style.content = item.data(Qt::UserRole + 100).value<QColor>();
    style.background = style.content;
    style.background.setAlpha(24);
    return style;
});
```

The factory receives a source-model index, even while the list is filtered. It
returns the complete style for that tag. An empty factory restores the shared
style. Model data changes update tags; call `refreshTags()` when external data
captured by the factory changes. The `tag...` properties expose shared style
settings in Designer. The complete `TagStyle` also includes `disabledContent`.

## Popup and buttons

Configure the popup through `popupAboutToShow(PopupMenu*)`, using the same popup
style, list style, delegate, and scrollbar APIs as other menus. Each opening
creates a new popup; selection and query filtering live in the control's model.
Filtering recalculates the visible popup height and displays `No results` for an
empty list. `popupWidth == 0` follows the control width; `maxVisibleItems == 0`
allows all rows. `filterModel()` exposes keyword-highlight settings.

`editor()`, `arrowButton()` and `clearButton()` expose the embedded controls for
text settings, button icons, sizes and state colors. Keep the editor transparent
and borderless to retain the shared outer frame. The clear/arrow visibility
switches are exposed as properties. `TagWidget` and `MultiSelectComboBox` are also
registered in the Designer `animated-widgets` group under their short names.

The gallery's ComboBoxes page includes non-searchable tags, searchable colored
tags and a disabled multi-selection example.
