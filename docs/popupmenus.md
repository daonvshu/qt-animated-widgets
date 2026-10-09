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
The default delegate draws each option's `PopupMenuItem::icon` (or `Qt::DecorationRole`)
when it is set. Configure `DefaultPopupMenuListStyle::itemIconSize` and
`itemIconSpacing` for ordinary item icons; `iconSpacing` remains the gap after checked
and unchecked state icons. These settings also apply to ComboBox and SearchComboBox popups.
If any option in the displayed model's sibling list has an icon, all option rows reserve
the same icon width and spacing so their text aligns. Lists without icons reserve no icon column.
Group titles and separators keep their own layout. Icon changes and filtering recalculate the column.

```cpp
PopupMenuItem item;
item.text = QStringLiteral("Harbor");
item.icon = QIcon(QStringLiteral(":/icons/harbor.svg"));
popup->setItems({item});

auto* style = new DefaultPopupMenuListStyle;
style->itemIconSize = QSize(18, 18);
style->itemIconSpacing = 8;
popup->setListStyle(style);
```

## ComboBox groups

`ComboBox::addGroupTitle(text)` and `insertGroupTitle(index, text)` insert nonselectable
headings into the default `QStandardItemModel`. They return false without changing the
model if the installed model is not a `QStandardItemModel`. `isGroupTitle(index)` identifies headings.
The methods respect `rootModelIndex()` and `modelColumn()`.

```cpp
combo->addGroupTitle(QStringLiteral("Planning"));
combo->addItem(QStringLiteral("Design"), QStringLiteral("design"));
combo->addItem(QStringLiteral("Development"), QStringLiteral("development"));
combo->addGroupTitle(QStringLiteral("Release"));
combo->addItem(QStringLiteral("Testing"), QStringLiteral("testing"));
combo->setCurrentIndex(1);
```

Headings use the popup list style's `groupTitleText` and `groupTitleHeight` settings.
Configure these through `popupAboutToShow`, as with other popup styles.
The popup does not select headings or draw selection indicators for them.
The source heading flags are `Qt::NoItemFlags`, so native ComboBox keyboard navigation skips them.
Inserting a heading preserves an existing selection; inserting into an unselected combo keeps index -1.
Programmatic `setCurrentIndex()` remains the inherited Qt API; pass an option row, not a heading.
Indexes and `count()` include heading and separator rows; `currentData()` can identify business values.
`CompositeComboBox` exposes these methods through `editor()`.

For a custom model, return `"groupTitle"` in `Qt::AccessibleDescriptionRole` for heading rows
and `Qt::NoItemFlags` from `flags()` for those rows. The popup translates that marker to
`PopupMenuItem::GroupTitle`. Native `"separator"` markers also remain supported.
Source `Qt::UserRole` values, including the popup's reserved roles, are not used to identify
ComboBox headings, preserving existing application data.

## Editable ComboBox

`ComboBox::popupOnEdit` defaults to true. In editable mode, clicking the input or editing text opens the
popup while keeping focus and IME input in the editor. Ordinary ComboBox lists are not
filtered. `setCurrentText()` and other programmatic changes do not open the popup.
Use `setPopupOnEdit(false)` to disable automatic opening; arrow-button opening remains available.
`CompositeComboBox` forwards the same property.

While enabled, an existing completer uses inline completion so a second completion popup
does not compete with the control's popup. Disabling the option restores the completer's
previous completion mode. `SearchComboBox` has no native completer and inherits the option
for automatic opening on typing; its click-to-open behavior remains enabled.

In an editable ComboBox popup, Up/Down navigate options and Enter confirms a keyboard-navigated
option. Enter without popup navigation retains Qt's editable-text/insert-policy behavior.
Escape, outside clicks and loss of editor focus close the popup without discarding typed text.
The gallery's `Popup on edit` checkbox demonstrates this option.

## SearchComboBox

`SearchComboBox` inherits `ComboBox` and searches using its own editable input.
The popup contains only results and the empty-state widget, without a second input.
It defaults to `NoInsert` and disables Qt's completer. Keep it editable for searching.

```cpp
auto* combo = new SearchComboBox(parent);
combo->addGroupTitle(QStringLiteral("Assets"));
combo->addItem(QStringLiteral("Orchard"), 123);
combo->addItem(QStringLiteral("Harbor"), 456);
combo->setCurrentIndex(1);
combo->setHighlightColor(QColor(QStringLiteral("#1778ff")));
combo->setMaxVisibleItems(6);
```

User edits filter the popup with `PopupMenuFilterModel`; programmatic text changes do not start searching.
The proxy chain is source model -> `ComboPopupModel` -> `PopupMenuFilterModel`.
The original model, business roles and source-row indexes remain unchanged.
`currentIndexChanged` and `activated` report source rows, including group/separator positions.
Inherited `currentTextChanged` also reports temporary editor text while typing.

Highlighting defaults to enabled, `#1778ff`, and bold. Configure `highlightEnabled`,
`highlightColor` and `highlightBold`, as with `SearchPopupMenu`.
Custom delegates receive the filtering proxy index and can read `PopupMenuItem::HighlightRole`.
Use `popupAboutToShow` for list/body styles and delegates.
Popup height follows the filtered results up to `maxVisibleItems()`.

Clicking the editable input opens the results popup without requiring a text change.
The results use a nonactivating tool window rather than a `Qt::Popup` input grab;
typing and input methods stay with the ComboBox editor. Outside clicks and owner-window
deactivation close the results window without consuming the outside click.
The editor keeps focus. Up/Down navigate enabled result options; Enter selects the current
result, or the first enabled result if none is highlighted. Escape/outside dismissal cancels
the query and restores the currently selected item's text. Confirming a result also clears
the query. Temporary text is not inserted into the model.

`setEmptyWidget(widget)` takes ownership of a custom empty-result widget and reuses it between
popup openings. The default is a centered `No results` label; nullptr restores this default.
`emptyWidget()` returns the custom widget, or nullptr when using the built-in label.

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
