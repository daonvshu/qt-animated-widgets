#include "comboboxstyleproperties.h"

#include "../utils/stylepluginproperties.h"
#include "animatedwidgets/comboboxes/combobox.h"
#include "animatedwidgets/comboboxes/compositecombobox.h"

using animatedwidgets::styleeditor::StylePropertyBuilder;
using animatedwidgets::styleeditor::StylePropertyGroup;
using P = StylePluginProperties;

void ComboBoxStyleProperties::comboBox(StylePropertyBuilder& builder) {
    builder.addGroups({
        interaction(),
        P::inputBackground(),
        P::inputBorder(),
        P::inputShape(),
        P::textColors(),
        arrow(),
        popup(),
        P::animation(),
        editing()
    });
}

void ComboBoxStyleProperties::searchComboBox(StylePropertyBuilder& builder) {
    builder.addGroups({
        interaction(),
        P::inputBackground(),
        P::inputBorder(),
        P::inputShape(),
        P::textColors(),
        arrow(),
        popup(),
        P::animation(),
        search()
    });
}

void ComboBoxStyleProperties::creatableComboBox(StylePropertyBuilder& builder) {
    builder.addGroups(
            {interaction(), P::inputBackground(), P::inputBorder(), P::inputShape(), P::textColors(), arrow(), popup(), P::animation(), search(), creation()});
}

void ComboBoxStyleProperties::treeComboBox(StylePropertyBuilder& builder) {
    builder.addGroups({
        interaction(),
        P::inputBackground(),
        P::inputBorder(),
        P::inputShape(),
        P::textColors(),
        arrow(),
        popup(),
        P::animation(),
        hierarchy(),
        treeSearch()
    });
}

void ComboBoxStyleProperties::cascaderComboBox(StylePropertyBuilder& builder) {
    builder.addGroups({
        interaction(),
        P::inputBackground(),
        P::inputBorder(),
        P::inputShape(),
        P::textColors(),
        arrow(),
        popup(),
        P::animation(),
        hierarchy(),
        cascader()
    });
}

void ComboBoxStyleProperties::multiSelectComboBox(StylePropertyBuilder& builder) {
    builder.addGroups({
        interaction(),
        P::inputBackground(),
        P::inputBorder(),
        P::inputShape(),
        P::animation(),
        multiContent(),
        multiArrow(),
        tags(),
        tagCloseButton(),
        multiPopup()
    });
}

void ComboBoxStyleProperties::compositeComboBox(StylePropertyBuilder& builder) {
    builder.addGroups(
            {interaction(), P::inputBackground(), P::inputBorder(), P::inputShape(), P::animation(), P::compositeLayout(), editing(), compositeContent()});

    auto* widget = qobject_cast<CompositeComboBox*>(builder.object());
    auto* defaults = qobject_cast<CompositeComboBox*>(builder.defaults());
    if (widget && defaults) {
        P::editorProperties(builder, widget->editor(), defaults->editor(), {P::textColors(), arrow(), popup()});
    }
}

StylePropertyGroup ComboBoxStyleProperties::interaction() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Combo box");
    group.properties = {
        P::field(QStringLiteral("enabled"), QStringLiteral("Enabled")),
        P::field(QStringLiteral("hasError"), QStringLiteral("Error state"))
    };
    return group;
}

StylePropertyGroup ComboBoxStyleProperties::editing() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Editing");
    group.properties = {
        P::field(QStringLiteral("editable"), QStringLiteral("Editable")),
        P::when(P::field(QStringLiteral("popupOnEdit"), QStringLiteral("Open popup on editing")), QStringLiteral("editable"))
    };
    return group;
}

StylePropertyGroup ComboBoxStyleProperties::arrow() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Arrow");
    group.properties = {
        P::field(QStringLiteral("arrowVisible"), QStringLiteral("Visible")),
        P::when(P::icon(QStringLiteral("arrowIcon"), QStringLiteral("Icon")), QStringLiteral("arrowVisible")),
        P::when(P::field(QStringLiteral("arrowIconSize"), QStringLiteral("Icon size")), QStringLiteral("arrowVisible")),
        P::when(P::field(QStringLiteral("arrowRightMargin"), QStringLiteral("Right margin")), QStringLiteral("arrowVisible")),
        P::field(QStringLiteral("iconHint"), QStringLiteral("Tint icons"))
    };
    return group;
}

StylePropertyGroup ComboBoxStyleProperties::popup() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Popup");
    group.properties = {
        P::field(QStringLiteral("popupWidth"), QStringLiteral("Width (0 = automatic)")),
        P::field(QStringLiteral("maxVisibleItems"), QStringLiteral("Maximum visible items")),
        P::horizontalAlignment(QStringLiteral("popupAlign"), QStringLiteral("Alignment")),
        P::field(QStringLiteral("popupScrollBarStyleSheet"), QStringLiteral("Scrollbar stylesheet"))
    };
    return group;
}

StylePropertyGroup ComboBoxStyleProperties::search() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Search");
    group.properties = {
        P::field(QStringLiteral("highlightEnabled"), QStringLiteral("Highlight matches")),
        P::when(P::field(QStringLiteral("highlightColor"), QStringLiteral("Highlight color")), QStringLiteral("highlightEnabled")),
        P::when(P::field(QStringLiteral("highlightBold"), QStringLiteral("Highlight bold")), QStringLiteral("highlightEnabled"))
    };
    return group;
}

StylePropertyGroup ComboBoxStyleProperties::creation() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Creation");
    group.properties = {
        P::field(QStringLiteral("creationEnabled"), QStringLiteral("Allow creation")),
        P::when(P::field(QStringLiteral("createText"), QStringLiteral("Action text")), QStringLiteral("creationEnabled")),
        P::when(P::field(QStringLiteral("minimumCreateLength"), QStringLiteral("Minimum length")), QStringLiteral("creationEnabled")),
        P::when(P::enumeration(QStringLiteral("creationCaseSensitivity"), QStringLiteral("Case sensitivity"), QStringLiteral("Qt")), QStringLiteral("creationEnabled"))
    };
    return group;
}

StylePropertyGroup ComboBoxStyleProperties::hierarchy() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Hierarchy");
    group.properties = {
        P::field(QStringLiteral("leafOnly"), QStringLiteral("Leaf selection only"))
    };
    return group;
}

StylePropertyGroup ComboBoxStyleProperties::treeSearch() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Search");
    group.properties = {
        P::field(QStringLiteral("searchable"), QStringLiteral("Searchable"))
    };
    return group;
}

StylePropertyGroup ComboBoxStyleProperties::cascader() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Columns");
    group.properties = {
        P::field(QStringLiteral("columnWidth"), QStringLiteral("Column width")),
        P::field(QStringLiteral("showFullPath"), QStringLiteral("Show full path")),
        P::when(P::field(QStringLiteral("pathSeparator"), QStringLiteral("Path separator")), QStringLiteral("showFullPath"))
    };
    return group;
}

StylePropertyGroup ComboBoxStyleProperties::multiContent() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Selection");
    group.properties = {
        P::field(QStringLiteral("placeholderText"), QStringLiteral("Placeholder")),
        P::field(QStringLiteral("searchable"), QStringLiteral("Searchable")),
        P::field(QStringLiteral("clearButtonVisible"), QStringLiteral("Clear button visible")),
        P::field(QStringLiteral("tagSpacing"), QStringLiteral("Tag spacing"))
    };
    return group;
}

StylePropertyGroup ComboBoxStyleProperties::multiArrow() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Arrow");
    group.properties = {
        P::field(QStringLiteral("arrowVisible"), QStringLiteral("Visible")),
        P::when(P::icon(QStringLiteral("arrowIcon"), QStringLiteral("Icon")), QStringLiteral("arrowVisible")),
        P::when(P::field(QStringLiteral("arrowIconSize"), QStringLiteral("Icon size")), QStringLiteral("arrowVisible")),
        P::field(QStringLiteral("iconHint"), QStringLiteral("Tint icons"))
    };
    return group;
}

StylePropertyGroup ComboBoxStyleProperties::tags() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Tags");
    group.properties = {
        P::field(QStringLiteral("tagBackground"), QStringLiteral("Background")),
        P::field(QStringLiteral("tagBorder"), QStringLiteral("Border")),
        P::field(QStringLiteral("tagContent"), QStringLiteral("Content")),
        P::field(QStringLiteral("tagCornerRadius"), QStringLiteral("Corner radius")),
        P::field(QStringLiteral("tagBorderWidth"), QStringLiteral("Border width")),
        P::field(QStringLiteral("tagContentPadding"), QStringLiteral("Content padding")),
        P::field(QStringLiteral("tagIconSize"), QStringLiteral("Icon size")),
        P::field(QStringLiteral("tagIconSpacing"), QStringLiteral("Icon spacing"))
    };
    return group;
}

StylePropertyGroup ComboBoxStyleProperties::tagCloseButton() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Tag close button");
    group.properties = {
        P::field(QStringLiteral("tagCloseButtonVisible"), QStringLiteral("Visible")),
        P::when(P::icon(QStringLiteral("tagCloseIcon"), QStringLiteral("Icon")), QStringLiteral("tagCloseButtonVisible")),
        P::when(P::field(QStringLiteral("tagCloseIconSize"), QStringLiteral("Icon size")), QStringLiteral("tagCloseButtonVisible")),
        P::when(P::field(QStringLiteral("tagCloseButtonSize"), QStringLiteral("Button size")), QStringLiteral("tagCloseButtonVisible"))
    };
    return group;
}

StylePropertyGroup ComboBoxStyleProperties::multiPopup() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Popup");
    group.properties = {
        P::field(QStringLiteral("popupWidth"), QStringLiteral("Width (0 = automatic)")),
        P::field(QStringLiteral("maxVisibleItems"), QStringLiteral("Maximum visible items")),
        P::field(QStringLiteral("popupScrollBarStyleSheet"), QStringLiteral("Scrollbar stylesheet"))
    };
    return group;
}

StylePropertyGroup ComboBoxStyleProperties::compositeContent() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Content");
    group.properties = {
        P::field(QStringLiteral("arrowVisible"), QStringLiteral("Arrow visible"))
    };
    return group;
}
