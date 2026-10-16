# Style editor

The optional `animatedwidgets-styleeditor` application edits the public QWidget
button properties and generates C++ setter code. It uses a card-based UI
with a live preview, grouped property rows, state preview controls and a code
copy action.

The editor uses AnimatedWidgets buttons, line edits, combo boxes and a plain
text editor, with switches for boolean properties. Fixed controls are declared
in the window and page `.ui` files. Theme colors are resource files under
`resources/styles/light` and `resources/styles/dark`, with matching CSS filenames
for buttons, inputs, labels, surfaces, views, scrollbars and the shared color
palette. `resources/styles/comm` holds reusable structure such as scrollbar
metrics and transparent native input backgrounds. Each theme also supplies
matching branch icons for the property tree. Each declaration occupies its own
line. `StyleThemeUtils` concatenates
the common files first, followed by the selected theme, and `StyleThemeManager`
loads the result on the main window. Page and dialog `.ui` styles retain local
layout, typography and shape settings without fixed theme colors.

Use **Theme manager** in the sidebar to choose Light or Dark. The selection is
remembered separately from documents. `StyleThemeScope` exposes color properties
for custom-painted trees and popup style objects that cannot consume QSS directly;
their colors are defined in each theme's `palette.css`. Newly created rows,
dialogs and editor dropdown menus receive the current theme automatically.
The preview runs in a separate process with its own QApplication and widget tree.
Editor stylesheets therefore need no per-widget isolation marker. Changing the
editor theme preserves widget properties, generated code and explicit preview
backgrounds; only a preview using the default canvas background follows the theme.

The application links AnimatedWidgets; the plugin API target remains
independent of the widget library.

Designer forms use short namespaced widget names such as `aw::FilledButton`.
Widget headers provide the `aw` namespace alias; stylesheet type selectors
retain the public meta-object namespace, such as `animatedwidgets--FilledButton`.

The application separates its implementation by responsibility:

- `StyleEditorWindow` owns the document session, file dialogs, save actions and
  unsaved-change confirmation, and coordinates sidebar navigation.
- `navigation/StyleWorkspaceRouter` installs QRouter on the workspace stack,
  registers page meta-types and reuses pages with `pushOrMove2Top`.
- `pages/StyleEditorPage` owns the editor layout, selection and property editing.
- `pages/WorkspacePage` defines the page document contract: `loadDocument`,
  `collectDocument` and `contentChanged`.
- `pages/ThemeManagerPage` selects the editor appearance without editing documents.
- The remaining workspace pages currently display their purpose and reserved status;
  they share the `WorkspaceSectionPage` presentation layout.
- `document/StyleDocumentSession` manages serializable content, file persistence
  and document metadata as a child of the main window.
- `document/StyleEditorSession` manages editable property data and cached code
  as a child of the editor page; it has no file APIs or preview QWidget instances.
- `preview/StylePreviewClient` starts and supervises the isolated worker and owns
  the local server. `StylePreviewChannel` frames socket messages, and
  `StylePreviewProtocol` serializes property metadata without executable callbacks.
- `preview/StylePreviewWorker` creates widgets, applies properties and preview
  states, resolves icons and generates code using plugin callbacks. Its canvas
  is embedded by `StylePreviewPanel` as a native foreign window.
- `dialogs/StylePopupDialog` inherits `BasePopupDlg` and supplies animated,
  window-modal dialogs. `StyleMessagePopup` handles messages and confirmations.
  Their layouts and appearance are declared in their `.ui` files. File and
  directory selection uses `QFileDialog`, including document open/save and
  resource browsing.
- `plugins/StylePluginRegistry` loads plugins and resolves widget types.
- `panels/StylePropertyInspector` builds and manages the property rows.
- Property categories are collapsible tree nodes. `StylePropertyRow` puts the
  name, a compact editor and a reset icon on one line. Color values use a swatch;
  hover shows their ARGB value, and unset colors show `Auto`.
  Plugin-defined enable conditions disable the entire row, including its reset
  button, without discarding its value. Conditions are refreshed on edits,
  document restoration, widget changes and resets.
- `dialogs/ColorPickerDialog` inherits `BasePopupDlg` through `StylePopupDialog`.
  Its Designer form contains saturation/value and hue pickers, sixteen preset
  colors, a HEX/RGB/HSV input-format combo box and an alpha slider (0-255) with
  an editable value and dropdown arrow on the right. The arrow opens a
  `PopupMenu` to choose decimal (0-255), integer percentage (0-100) or two-digit
  hexadecimal display with a `0x` prefix. Manual edits update the slider and color;
  percentage inputs accept an optional `%`, and hexadecimal inputs accept an optional
  `0x` prefix. Changing the format preserves the alpha value. The combo box
  switches the numeric input page; edits synchronize the other color representations
  and pickers. Apply updates the property after
  the close animation; Cancel leaves it unchanged, and Auto restores color
  derivation. The picker widgets live under `widgets/colors/`.
  The eyedropper beside the value inputs captures each screen and opens temporary
  overlays with a pixel magnifier and HEX preview. Click to sample RGB while
  retaining the current alpha, or press Escape or right-click to cancel. Picking
  updates the dialog; Apply still confirms the property change. Captures remain
  in memory and are released when the overlays close. Coordinate conversion uses
  each screen's geometry and capture dimensions to support display scaling.
  Unset colors are stored as JSON `null`, separately from fully transparent
  colors, and the built-in plugin exports them as `QColor()`.
- `panels/StyleWidgetTreeModel` groups document styles by plugin category;
  `StyleWidgetTreeAdapter` binds recycled `StyleWidgetTreeRow` widgets and their
  add/edit buttons to stable document item IDs.
- `utils/` contains static helpers for JSON persistence, property editors,
  preview states and C++ code generation.

The editor is a standalone executable. Its implementation classes, including
its built-in plugin implementation, use the global namespace and are not
exported as an application library. Public plugin API types retain the
`animatedwidgets::styleeditor` namespace.

The optional editor requires the QRouter and VirtualItemViews CMake packages.
Add the matching Qt version's installed package prefixes to `CMAKE_PREFIX_PATH`.
The editor imports them using `find_package(QRouter REQUIRED CONFIG)` and
`find_package(VirtualItemViews REQUIRED CONFIG)`, linking `qrouter` and
`VirtualItemViews::VirtualItemViews`. Widget libraries and the plugin API remain
independent of these dependencies.

The main window keeps its sidebar and document toolbar while QRouter switches
the lower workspace. The editor page remains alive when another section is
selected, preserving document contents, property values and the current preview.
Page layouts and local shape styles are declared in their `.ui` files; shared
theme colors are loaded from resources. Future pages derive from `WorkspacePage`,
provide a `Q_INVOKABLE` constructor taking `const QVariant&` and `QWidget*`, and
are registered in `StyleWorkspaceRouter` before navigation.

After an edit, a page emits `contentChanged`. The main window immediately asks
that page to collect its content into the window-owned document and updates the
modified state. This automatic synchronization works for temporary documents
as well; writing a JSON file remains an explicit Save or Save as action.
Before writing, the window asks the current page to collect its latest content.
Pages replace only the content they own, preserving other pages' data. Reserved
pages collect nothing, so saving from them retains existing widget styles.
New and Open reload all existing pages; pages created later load the current
document when first visited.

Dialog results are delivered asynchronously after the popup close animation.
New, Open and Close continue only after the unsaved-change choice and, when
needed, a successful save. File selection supports typed paths, directory
navigation, JSON filtering and overwrite confirmation without native dialogs
or nested blocking event loops.

The editor works with a document. A document can remain temporary while it is
being tuned, or it can be saved as a JSON file with New, Open, Save and Save as.
The left panel uses a VirtualItemViews tree. Its root nodes show all main types
from the loaded plugins' `category` fields, including categories without styles.
Click a root's plus button to open `StyleWidgetItemPopup`, choose a subtype in
that category, and name the new style. Each child is an independent widget
instance with its own preview and property values; its edit button reopens the
same dialog to change the subtype or name. The dialog inherits `BasePopupDlg`
through `StylePopupDialog`, and uses a Designer form for its layout and theme.

Renaming retains all properties. Changing subtype keeps the item's ID and
copies properties with matching names and property types; enum values must
also exist in the new subtype. Other properties use the new subtype's defaults.
Cancelling leaves the document unchanged. Search matches categories, style
names and subtypes. Missing plugin types appear under `Unavailable widgets`
and retain their saved data until explicitly replaced with an available type.

Saved documents use a small, versioned JSON format:

```json
{
  "version": 1,
  "name": "Button Theme",
  "items": [
    {
      "id": "filledbutton-example",
      "name": "Primary Button",
      "widgetType": "filledbutton",
      "properties": {
        "cornerRadius": 8,
        "backgroundNormal": "#ff3478f6"
      }
    }
  ]
}
```

Property callbacks and code-generation functions are supplied by the plugin;
only their current values are persisted in JSON. If a document references a
widget type that is not provided by a loaded plugin, its data is retained without
an editable preview. Unknown property values are also preserved on save.

The editor loads style editor plugins from:

```text
<editor directory>/plugins/styleeditor/
```

The built-in button plugin is emitted there as
`animatedwidgets-styleeditor-buttons`. Build `animatedwidgets-styleeditor` and
`animatedwidgetsstyleeditorbuttons` together, then launch the editor from its
output directory.

External plugins only need the lightweight
`AnimatedWidgets::StyleEditorApi` target. They can describe their own QWidget
types without linking the complete AnimatedWidgets library. A plugin that uses
AnimatedWidgets controls for its preview can additionally link
`AnimatedWidgets::AnimatedWidgets`.

The public API is under `animatedwidgets/styleeditor/api`:

- `StyleEditorPlugin` identifies a plugin and returns widget descriptors.
- `StyleEditorWidgetType` creates a widget, describes its editable properties and
  optionally returns `suggestedSize` for the editor.
- `StyleProperty` declares the editor type, default value, apply callback and C++
  code expression, plus optional `enabledWhen` conditions.
- `StylePropertyCondition` matches a controlling property's name and expected
  value. Multiple conditions must all match; no conditions means always enabled.
- `StyleWidgetTypeBuilder` registers a QWidget type, its property definitions and
  an optional suggested size. Creation returns an unmodified widget; default
  values come from another fresh instance of the same type.
- `StyleWidgetTypeOptions` supplies named registration fields: ID, display name,
  category, optional C++ type name, suggested size and property definition callback.
- `StylePropertyBuilder` groups properties and binds ordinary `Q_PROPERTY`
  declarations. Text, color, bool, int, float, double, QSize and QKeySequence types are inferred;
  enums, flag combinations and icon resource paths have explicit methods.
- `StyleEnumOption` maps a stable option name to its numeric value and C++
  expression. Use explicit options for combinations such as alignment flags.
- `StyleCodeExpressions`, under `styleeditor/utils`, formats escaped text,
  colors, floating-point values and icon paths for custom bindings.

The builder API is header-only and remains part of the lightweight
`AnimatedWidgets::StyleEditorApi` target. Existing plugins can still return
`StyleEditorWidgetType` and `StyleProperty` directly. Property descriptors now carry
enable conditions and dedicated icon, size and shortcut editors, so the plugin IID is version 4.0. Rebuild external plugins
against this API. Existing document property keys are unchanged.

Define property groups in named functions rather than mixing registration,
preview layout and serialization in one callback:

```cpp
#include "animatedwidgets/styleeditor/api/stylewidgettypebuilder.h"

namespace se = animatedwidgets::styleeditor;

class CustomerStyleProperties {
public:
    static void define(se::StylePropertyBuilder& builder) {
        content(builder);
        shape(builder);
    }

private:
    static void content(se::StylePropertyBuilder& builder) {
        builder.setGroup(QStringLiteral("Content"));
        builder.add(QStringLiteral("text"), QStringLiteral("Text"));
    }

    static void shape(se::StylePropertyBuilder& builder) {
        builder.setGroup(QStringLiteral("Shape"));
        builder.add(QStringLiteral("cornerRadius"), QStringLiteral("Radius"));
    }
};

// Inside the plugin's widgetTypes() implementation:
se::StyleWidgetTypeOptions options;
options.id = QStringLiteral("customerwidget");
options.name = QStringLiteral("Customer Widget");
options.category = QStringLiteral("Custom");
options.suggestedSize = QSize(150, 42);
options.defineProperties = &CustomerStyleProperties::define;

return {se::StyleWidgetTypeBuilder::create<CustomerWidget>(options)};
```

`CustomerWidget` must provide the declared readable/writable properties and a
constructor accepting a QWidget parent. The registered C++ type name is read
from its meta-object; the optional `options.typeName` field overrides it for
classes without their own `Q_OBJECT` declaration. Plugins create widgets without
setting preview text, minimum/fixed sizes or background colors.
Generated setters are omitted when the current value equals its known default.

`panels/StylePreviewPanel` owns the preview layout and settings. Its Designer
form provides width and height inputs (1-4096 pixels), a background color swatch
and independent Auto switches for width and height, plus Reset preview. An Auto
dimension uses the preview widget's size hint and size policy so a widget's own
layout can determine that dimension; a fixed dimension uses the entered pixel
value. Widgets remain centered inside a scrollable canvas at the chosen dimensions.
Without a custom size, the editor uses the plugin's suggested size, then the
widget's size hint. Background defaults to the main window's theme
color; Auto in the color picker restores that default. Custom sizes and
background colors are stored per document item in the optional `preview` JSON
object. Auto dimensions are stored as `widthAuto` and `heightAuto`; their fixed
pixel values are omitted while Auto is enabled. These editor settings are excluded
from generated widget setter code.

The first preview starts a persistent worker using the editor executable's
internal `--style-preview` mode. The editor owns a user-scoped QLocalServer;
the worker connects with QLocalSocket and a per-launch handshake token. Messages
are versioned JSON with a length prefix and a 16 MiB limit. Increasing request
revisions prevent stale responses from replacing a newer selection or edit.
The worker loads the same plugin directory, while the editor uses plugin type
registrations without constructing preview widgets. Plugin factories, property
callbacks and code expressions execute in the worker. Property metadata, including
defaults and enable conditions, travels back as data.

Only changed properties are applied to an existing preview, preserving running
animations when the canvas background or size changes. Switching document items
creates a new preview widget. The embedded canvas uses QWindow::fromWinId and
QWidget::createWindowContainer; native embedding requires platform support and
is intended for the Windows editor. The attach message supplies the native host
handle and viewport dimensions so the worker establishes its Qt window parent
and initial geometry before showing the canvas. Container resize events send
viewport-only messages without reapplying widget properties. The container stays
hidden until the first preview is ready. Startup or connection failures show a
Reload preview action. Worker crashes preserve document data; reloading rebuilds
the selected preview from the latest snapshot. The editor detaches the native
window before shutdown, and the worker exits when its local connection closes.
Qt Network is required only by the editor and its IPC tests.

Use `enumeration(name, label, scope)` for a `Q_ENUM` property. The scope is its
fully qualified C++ enum type. For flag combinations or a curated option list,
pass a `QList<StyleEnumOption>` instead; each option supplies its stored name,
numeric value and complete code expression. These names are persisted in JSON,
so keep them stable across plugin versions.

Use `alignment(name, label)` for a Qt alignment property. The editor presents
separate horizontal (Left, Center, Right) and vertical (Top, Center, Bottom)
choices on two stacked rows and combines them internally. JSON keeps the original property key and
combined value; generated code uses a single setter such as
`setContentAlignment(Qt::AlignLeft | Qt::AlignTop)`. Existing saved alignment
values remain compatible, and the centered default still omits the setter.

`icon(name, label, resource, defaultResource)` edits a `QIcon` property through
a resource picker. Use the QRC dropdown to select a `.qrc` file from any directory.
Its first entry opens `QFileDialog`; subsequent
entries switch between previously selected QRC files without adding duplicates.
History entries show full paths to distinguish files with the same name.
The searchable virtual tree groups icons by resource prefixes and alias directories.
Rows display icon previews and names; selection displays the complete reference.
Clear icon restores an empty icon. No manual icon-path entry is required.
QRC images use `:/prefix/alias`, including QRC aliases. The editor resolves images
from all remembered QRC files for preview, with the active QRC taking precedence
when resource references overlap. Switching QRC files preserves other icon previews.
Include the QRC files in the consuming application's build so generated `:/`
references are available at runtime.

Resource settings are shared by document items and saved in an optional
`resources` object. The first QRC's directory becomes the automatic path base,
stored relative to the JSON file in `projectDirectory` for compatibility with
existing documents. The active source and `qrcFiles` history are relative to that
base, including parent-directory references; files on other drives retain absolute
paths. No project selection is required. Temporary documents also support resource
selection. Moving the directory tree together with its JSON and resources
preserves these relative references. Changing the resource source refreshes
preview resolution for all icon properties.

Supply the current and default paths when either is nonempty;
QIcon does not retain a recoverable source path. Both paths default to empty.
QSize properties use separate width and height inputs and generate `QSize(w, h)`.
Shortcuts use portable Qt key-sequence text and generate a `QKeySequence` setter.
Button plugins expose text, icon, icon size, enabled, checkable/checked, shortcut,
auto-repeat settings, auto-default/default and tooltip properties. Checked is
editable only when checkable is enabled; repeat timing requires auto-repeat.
The Content group's Tint icon switch controls `iconHint`: it defaults to false,
preserving original icon colors regardless of the content color. Enable it to
apply content colors to the icon; Animate content controls state transitions
independently. Icon-only variants disable content color and animation controls
until Tint icon is enabled. JSON stores the switch and generated C++ emits
`setIconHint(true)` only when it differs from the default.
For nonstandard properties, construct a `StyleProperty` with custom callbacks
and pass it to `builder.add(property)`. Callback safety for such custom bindings
is the plugin's responsibility; automatic bindings guard the target with QPointer.

Builder methods return false for missing, read-only, unsupported or duplicate
properties and invalid enum options. `errors()` exposes definition diagnostics;
the type builder reports them when building the property list.

Declare enable conditions after registering both properties:

```cpp
builder.enabledWhen(QStringLiteral("contentHover"), QStringLiteral("animateContentColor"));
builder.enabledWhen(QStringLiteral("contentPressed"), QStringLiteral("animateContentColor"), true);
```

The expected value defaults to `true`; other values can be booleans, numbers
or enum option names. Repeated calls with different controlling properties add
conditions that must all match. Missing properties, self-dependencies, invalid
expected values and duplicate controllers are rejected with a builder error.
Plugins returning descriptors directly can fill `StyleProperty::enabledWhen`.
Conditions use current property values and do not recursively follow dependencies;
an unknown controller disables the target. Condition metadata belongs to the plugin,
while saved JSON and generated setters retain disabled properties' configured values.

The built-in button plugin is divided into `ButtonStylePlugin` for registration
and suggested sizes, and `ButtonStyleProperties` for explicit per-type property
groups. Each button type lists the
groups it supports, including the four state backgrounds supported by hover
buttons; no background-normal or border properties are declared for those types.
Hover, pressed, disabled and checked content colors require `Animate content`.
The normal content color remains editable because buttons use it even when
content animation is disabled.

An external plugin follows the normal Qt plugin pattern:

```cpp
class CustomerPlugin : public QObject, public animatedwidgets::styleeditor::StyleEditorPlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID ANIMATEDWIDGETS_STYLE_EDITOR_PLUGIN_IID)
    Q_INTERFACES(animatedwidgets::styleeditor::StyleEditorPlugin)
};
```

Keep plugin output compatible with the Qt major version used by the editor. The
editor keeps loaded plugin instances alive for the lifetime of the window and
ignores files that do not implement the API interface.
