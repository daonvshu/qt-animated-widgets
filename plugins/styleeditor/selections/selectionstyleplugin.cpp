#include "selectionstyleplugin.h"

#include "selectionstyleproperties.h"
#include "../utils/styleplugintypeutils.h"
#include "animatedwidgets/buttons/checkbox.h"
#include "animatedwidgets/buttons/radiobutton.h"

#include <QPixmap>

using animatedwidgets::styleeditor::StyleEditorWidgetType;
using animatedwidgets::styleeditor::StyleWidgetTypeOptions;

QString SelectionStylePlugin::pluginId() const {
    return QStringLiteral("animatedwidgets.selections");
}

QString SelectionStylePlugin::pluginName() const {
    return QStringLiteral("Animated Widgets Selections");
}

QList<StyleEditorWidgetType> SelectionStylePlugin::widgetTypes() const {
    StyleWidgetTypeOptions checkBox;
    checkBox.id = QStringLiteral("checkbox");
    checkBox.name = QStringLiteral("Check Box");
    checkBox.category = QStringLiteral("Selections");
    checkBox.categoryOrder = 200.0;
    checkBox.order = 100.0;
    checkBox.suggestedSize = QSize(160, 36);
    checkBox.defineProperties = &SelectionStyleProperties::checkBox;

    StyleWidgetTypeOptions radioButton;
    radioButton.id = QStringLiteral("radiobutton");
    radioButton.name = QStringLiteral("Radio Button");
    radioButton.category = QStringLiteral("Selections");
    radioButton.categoryOrder = 200.0;
    radioButton.order = 200.0;
    radioButton.suggestedSize = QSize(160, 36);
    radioButton.defineProperties = &SelectionStyleProperties::radioButton;

    return {
        StylePluginTypeUtils::create<animatedwidgets::CheckBox>(checkBox, [](animatedwidgets::CheckBox* widget) {
            widget->setText(QStringLiteral("Preview"));
            widget->setCheckedIcon(QPixmap(QStringLiteral(":/animatedwidgets/styleeditor/selections/checkboxchecked.svg")));
            widget->setUncheckedIcon(QPixmap(QStringLiteral(":/animatedwidgets/styleeditor/selections/checkboxunchecked.svg")));
            widget->setPartiallyCheckedIcon(QPixmap(QStringLiteral(":/animatedwidgets/styleeditor/selections/checkboxpartial.svg")));
        }),
        StylePluginTypeUtils::create<animatedwidgets::RadioButton>(radioButton, [](animatedwidgets::RadioButton* widget) {
            widget->setText(QStringLiteral("Preview"));
            widget->setCheckedIcon(QPixmap(QStringLiteral(":/animatedwidgets/styleeditor/selections/radiochecked.svg")));
            widget->setUncheckedIcon(QPixmap(QStringLiteral(":/animatedwidgets/styleeditor/selections/radiounchecked.svg")));
        })
    };
}
