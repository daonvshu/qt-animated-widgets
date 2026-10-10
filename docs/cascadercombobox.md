# CascaderComboBox

`CascaderComboBox` browses a hierarchical item model in adjacent columns.
Click a branch to reveal its children; selecting a leaf confirms the value
and closes the popup. Disabled nodes and descendants of disabled nodes cannot
be chosen. Column navigation uses the model's first column.

```cpp
combo->setModel(model);
combo->setColumnWidth(180);
combo->setPopupWidth(560);
combo->setMaxVisibleItems(8);
combo->setCurrentModelIndex(initialIndex);

connect(combo, &animatedwidgets::CascaderComboBox::currentModelIndexChanged,
        combo, [combo](const QModelIndex& index) {
    const QVariant value = index.data(Qt::UserRole);
    const QList<QModelIndex> path = combo->currentPath();
    // Consume the selected value and path in the application.
    Q_UNUSED(value);
    Q_UNUSED(path);
});
```

`currentModelIndex()` is an index in the original source model. `currentPath()`
returns source indexes from the browsing root's children to the selected node;
the root itself is excluded. `currentPathTexts()` returns their display labels.
`setTreeRootIndex()` limits browsing to one subtree. Source models remain owned
by the application.

`leafOnly` defaults to true. Set it to false to confirm branches as well.
Click a branch once to expand it, then click the same branch again to confirm
it. The currently selected branch can be confirmed with one click immediately
after opening the popup. Navigating away clears that shortcut, even when
returning to the same branch. Ancestors on the active browsing path remain
highlighted; a previously selected leaf loses its highlight and check icon
when browsing another branch. Columns are separated by 2 pixels.

`showFullPath` defaults to true; `pathSeparator` defaults to `" / "`.
These settings affect the painted non-editable label; native `currentText()`
continues to return the selected item's text.

The popup uses `CascaderPopupMenu` and `CascaderColumnView`. Configure its body,
list style and delegate through the inherited `popupAboutToShow` signal.
The default delegate reuses popup item colors, fonts, icons and spacing, and
draws the configured submenu arrow for branches. Columns use pixel scrolling
and inherit the popup scrollbar stylesheet. When columns exceed the popup's
width, the column container scrolls horizontally.

Up/Down navigate a column, Right enters a branch, Left returns to the parent,
Enter/Space confirms an eligible node, and Escape dismisses the popup.
The gallery includes leaf-only selection and selection at any level.
