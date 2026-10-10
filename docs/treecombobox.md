# Tree selection

`TreeComboBox` accepts a hierarchical `QAbstractItemModel`, including
`QStandardItemModel`. The application retains ownership of an externally supplied
model. Each node can carry its own icon and arbitrary role data.

```cpp
auto* model = new QStandardItemModel(combo);
auto* category = new QStandardItem(QStringLiteral("Documents"));
auto* guide = new QStandardItem(QStringLiteral("Quick guide"));
guide->setData(QStringLiteral("guide"), Qt::UserRole);
category->appendRow(guide);
model->appendRow(category);

combo->setModel(model);
combo->setCurrentModelIndex(guide->index());
connect(combo, &animatedwidgets::TreeComboBox::currentModelIndexChanged,
        receiver, [](const QModelIndex& index) {
    const QVariant value = index.data(Qt::UserRole);
    // Use the selected node's application data.
});
```

Selection defaults to leaves only. Set `leafOnly` to false to allow selecting
parents as well. Disabled nodes and descendants of disabled parents cannot be
selected. Clicking a parent in leaf-only mode expands or collapses it. The native
tree branch indicators and Left/Right keys also control expansion.

Use `setTreeRootIndex()` to restrict the popup to a subtree. The root itself is
excluded from the choices. `setCurrentModelIndex(QModelIndex())` clears selection.
Use column-zero indexes for the tree root and indexes in `modelColumn()` for
selection. For a hierarchical model, children belong to column-zero parent nodes.

Prefer `currentModelIndex()`, `currentModelIndexChanged` and `modelIndexActivated`
over the inherited row-based APIs. The inherited `currentIndex()` is relative to
the selected node's parent and cannot uniquely identify a tree node. Keep a
`QPersistentModelIndex` when retaining a selection across model edits.

The popup includes a search field by default. Set `searchable` to false to hide
it. Search preserves ancestors of matching descendants and expands results.
`maxVisibleItems` limits the number of visible tree rows used for popup height;
expanding, collapsing and filtering update the height automatically.

Customize the popup through the inherited `popupAboutToShow` signal. Its argument
is a `TreePopupMenu`, with the same popup/list style and delegate APIs as other
menus. `treeView()` exposes the `QTreeView`, `searchEditor()` exposes the
`IconLineEdit`, and `enableFiltering()` returns the existing filter model for
highlight configuration. Custom delegates receive proxy indexes; arbitrary role
data remains available through `index.data(role)`.
