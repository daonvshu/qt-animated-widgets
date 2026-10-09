# Popup menus

`PopupMenu` owns the popup body, list view, delegate and optional top/bottom widgets.
`PopupMenuStyle` controls the body; `PopupMenuListStyle` controls the default delegate and list appearance.
`setModel()` uses an externally owned model. `setItemDelegate()` takes ownership of the delegate.

## Structured items

`setStrings()` remains available for simple lists. `setItems()` uses the built-in
`DefaultPopupMenuModel`, retaining single/multiple selection support:

```cpp
PopupMenuItem heading;
heading.type = PopupMenuItem::GroupTitle;
heading.text = QStringLiteral("Recommended");

PopupMenuItem option;
option.text = QStringLiteral("Development");
option.values.insert(Qt::UserRole + 100, 42);

popup->setItems({heading, option});
```

Items have text, icon, shortcut, type, enabled state and a role-to-QVariant map.
`Option` rows support selection; `GroupTitle` and `Separator` rows do not.
Selection indexes are model row indexes, including the positions occupied by headings/separators.
`setItems()` resets selection. Use `setData(index, value, role)` to update data without resetting selection.

The library reserves `Qt::UserRole + 1` (separator), `+2` (shortcut), `+3` (item type) and `+4` (highlight HTML).
Standard display, decoration and check-state roles are supplied by the default model.
Use other roles, such as `Qt::UserRole + 100`, for application data or custom delegates.
Custom models may supply these roles to use the default delegate, or provide their own delegate.

`DefaultPopupMenuListStyle::groupTitleText` and `groupTitleHeight` control headings.

## Fixed widgets

`setTopWidget()` and `setBottomWidget()` add fixed widgets outside the scrolling list.
They take ownership. Replacing a widget or passing nullptr deletes the previous widget later.
The same widget cannot occupy two slots. Adaptive height includes visible slot widgets.
Call `setAdaptiveHeight()` again after changing items, styles or slot widgets.

## Search menus

`SearchPopupMenu` composes an `IconLineEdit` with a leading SVG search icon in the top slot and a `PopupMenuFilterModel`.
Use `searchEditor()` to configure the input and `setSearchText()` to set the query.
The top slot holds the search editor; use the bottom slot for additional fixed content.

`searchSpacing` controls the gap between the search editor and the list or empty widget.
Use `setSearchSpacing()` to change it; the default is 8 pixels and negative values are clamped to 0.
Changing the spacing also recalculates height when adaptive height is enabled.

`setModel()` supplies the source model. `sourceModel()` returns it, while `model()`
returns the filtering proxy used by the list and delegate. `setItems()`/`setStrings()`
use the default source model and preserve its selection behavior.

Filtering performs a case-insensitive ordered-character match on display text by default,
using `TextSearchUtils` (for example, `hr` matches `Harbor`).
Use `filterModel()` to configure the role/column. Group headings remain only when
their following group contains a match; separators are hidden while searching.
An empty query restores all rows. Filtering preserves source-model selection.

`setHighlightEnabled(true)` enables matching-character markup using `TextSearchUtils::searchAndHighlight()`.
Highlighting is enabled by default; the default color is `#1778ff` and bold is enabled.
Use `setHighlightColor()` and `setHighlightBold()` to customize it. These settings do not change filtering.
Plain display text remains unchanged; the proxy supplies escaped HTML in `PopupMenuItem::HighlightRole`.
The default delegate renders this role. Custom delegates can read it for their own rendering.

`searchTextChanged` reports input changes. `sourceItemTriggered` supplies the original
source index and application data. The inherited `itemTriggered` supplies a proxy index.
For custom models, handle selection in application code.

Escape closes the menu. Down from the search editor focuses the first enabled option;
the list supports its normal keyboard navigation and Enter/Space activation.
Call `setAdaptiveHeight(width, maximumVisibleItems)` to enable dynamic height adjustment.
Filtering and model updates recalculate height using the remaining rows and the original visible-row limit.
An empty result replaces the list with the empty widget and retains the search editor and any bottom widget.
Updates are deferred to the next event-loop iteration so filtering can finish before measuring.
`setPopupSize()` switches back to a fixed size and disables automatic height updates.

`emptyWidget()` returns the empty-state widget, initially a centered `No results` label.
`setEmptyWidget(widget)` replaces it and takes ownership; the previous widget is deleted later.
Pass a QLabel with a pixmap for an image, or any custom QWidget for a richer empty state.
Passing nullptr removes the empty-state content. Groups/separators alone count as an empty list.
The empty widget is also shown with a fixed popup size; adaptive height includes its size hint.
