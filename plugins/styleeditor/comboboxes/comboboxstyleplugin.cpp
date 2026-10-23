#include "comboboxstyleplugin.h"

#include "comboboxstyleproperties.h"
#include "../utils/styleplugintypeutils.h"
#include "animatedwidgets/comboboxes/combobox.h"
#include "animatedwidgets/comboboxes/searchcombobox.h"
#include "animatedwidgets/comboboxes/creatablecombobox.h"
#include "animatedwidgets/comboboxes/treecombobox.h"
#include "animatedwidgets/comboboxes/cascadercombobox.h"
#include "animatedwidgets/comboboxes/multiselectcombobox.h"
#include "animatedwidgets/comboboxes/compositecombobox.h"
#include "../utils/stylepreviewdata.h"
#include "animatedwidgets/comboboxes/hierarchicalcombobox.h"

using animatedwidgets::styleeditor::StyleEditorWidgetType;
using animatedwidgets::styleeditor::StyleWidgetTypeOptions;

QString ComboBoxStylePlugin::pluginId() const {
    return QStringLiteral("animatedwidgets.comboboxes");
}

QString ComboBoxStylePlugin::pluginName() const {
    return QStringLiteral("Animated Widgets Comboboxes");
}

QList<StyleEditorWidgetType> ComboBoxStylePlugin::widgetTypes() const {
    StyleWidgetTypeOptions comboBox;
    comboBox.id = QStringLiteral("combobox");
    comboBox.name = QStringLiteral("Combo Box");
    comboBox.category = QStringLiteral("Combo boxes");
    comboBox.categoryOrder = 500.0;
    comboBox.order = 100.0;
    comboBox.suggestedSize = QSize(240, 40);
    comboBox.defineProperties = &ComboBoxStyleProperties::comboBox;

    StyleWidgetTypeOptions searchComboBox;
    searchComboBox.id = QStringLiteral("searchcombobox");
    searchComboBox.name = QStringLiteral("Search Combo Box");
    searchComboBox.category = QStringLiteral("Combo boxes");
    searchComboBox.categoryOrder = 500.0;
    searchComboBox.order = 200.0;
    searchComboBox.suggestedSize = QSize(240, 40);
    searchComboBox.defineProperties = &ComboBoxStyleProperties::searchComboBox;

    StyleWidgetTypeOptions creatableComboBox;
    creatableComboBox.id = QStringLiteral("creatablecombobox");
    creatableComboBox.name = QStringLiteral("Creatable Combo Box");
    creatableComboBox.category = QStringLiteral("Combo boxes");
    creatableComboBox.categoryOrder = 500.0;
    creatableComboBox.order = 300.0;
    creatableComboBox.suggestedSize = QSize(240, 40);
    creatableComboBox.defineProperties = &ComboBoxStyleProperties::creatableComboBox;

    StyleWidgetTypeOptions treeComboBox;
    treeComboBox.id = QStringLiteral("treecombobox");
    treeComboBox.name = QStringLiteral("Tree Combo Box");
    treeComboBox.category = QStringLiteral("Combo boxes");
    treeComboBox.categoryOrder = 500.0;
    treeComboBox.order = 400.0;
    treeComboBox.suggestedSize = QSize(240, 40);
    treeComboBox.defineProperties = &ComboBoxStyleProperties::treeComboBox;

    StyleWidgetTypeOptions cascaderComboBox;
    cascaderComboBox.id = QStringLiteral("cascadercombobox");
    cascaderComboBox.name = QStringLiteral("Cascader Combo Box");
    cascaderComboBox.category = QStringLiteral("Combo boxes");
    cascaderComboBox.categoryOrder = 500.0;
    cascaderComboBox.order = 500.0;
    cascaderComboBox.suggestedSize = QSize(240, 40);
    cascaderComboBox.defineProperties = &ComboBoxStyleProperties::cascaderComboBox;

    StyleWidgetTypeOptions multiSelectComboBox;
    multiSelectComboBox.id = QStringLiteral("multiselectcombobox");
    multiSelectComboBox.name = QStringLiteral("Multi Select Combo Box");
    multiSelectComboBox.category = QStringLiteral("Combo boxes");
    multiSelectComboBox.categoryOrder = 500.0;
    multiSelectComboBox.order = 600.0;
    multiSelectComboBox.suggestedSize = QSize(240, 40);
    multiSelectComboBox.defineProperties = &ComboBoxStyleProperties::multiSelectComboBox;

    StyleWidgetTypeOptions compositeComboBox;
    compositeComboBox.id = QStringLiteral("compositecombobox");
    compositeComboBox.name = QStringLiteral("Composite Combo Box");
    compositeComboBox.category = QStringLiteral("Combo boxes");
    compositeComboBox.categoryOrder = 500.0;
    compositeComboBox.order = 700.0;
    compositeComboBox.suggestedSize = QSize(240, 40);
    compositeComboBox.defineProperties = &ComboBoxStyleProperties::compositeComboBox;

    return {
        StylePluginTypeUtils::create<animatedwidgets::ComboBox>(comboBox, [](animatedwidgets::ComboBox* widget) {
            widget->addItems(StylePreviewData::choices());
        }),
        StylePluginTypeUtils::create<animatedwidgets::SearchComboBox>(searchComboBox, [](animatedwidgets::SearchComboBox* widget) {
            widget->addItems(StylePreviewData::choices());
        }),
        StylePluginTypeUtils::create<animatedwidgets::CreatableComboBox>(creatableComboBox, [](animatedwidgets::CreatableComboBox* widget) {
            widget->addItems(StylePreviewData::choices());
        }),
        StylePluginTypeUtils::create<animatedwidgets::TreeComboBox>(treeComboBox, [](animatedwidgets::TreeComboBox* widget) {
            widget->setModel(StylePreviewData::tree(widget));
            widget->setCurrentModelIndex(widget->model()->index(0, 0, widget->model()->index(0, 0)));
        }),
        StylePluginTypeUtils::create<animatedwidgets::CascaderComboBox>(cascaderComboBox, [](animatedwidgets::CascaderComboBox* widget) {
            widget->setModel(StylePreviewData::tree(widget));
            widget->setCurrentModelIndex(widget->model()->index(0, 0, widget->model()->index(0, 0)));
        }),
        StylePluginTypeUtils::create<animatedwidgets::MultiSelectComboBox>(multiSelectComboBox, [](animatedwidgets::MultiSelectComboBox* widget) {
            widget->setStrings(StylePreviewData::choices());
            widget->setSelectedIndexes({0, 1});
        }),
        StylePluginTypeUtils::create<animatedwidgets::CompositeComboBox>(compositeComboBox, [](animatedwidgets::CompositeComboBox* widget) {
            widget->editor()->addItems(StylePreviewData::choices());
        })
    };
}
