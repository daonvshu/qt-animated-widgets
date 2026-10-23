#include "inputstyleplugin.h"

#include "inputstyleproperties.h"
#include "../utils/styleplugintypeutils.h"
#include "animatedwidgets/inputs/lineedit.h"
#include "animatedwidgets/inputs/iconlineedit.h"
#include "animatedwidgets/inputs/passwordlineedit.h"
#include "animatedwidgets/inputs/plaintextedit.h"
#include "animatedwidgets/inputs/spinbox.h"
#include "animatedwidgets/inputs/doublespinbox.h"
#include "animatedwidgets/inputs/compositelineedit.h"
#include "animatedwidgets/inputs/compositeplaintextedit.h"
#include "animatedwidgets/inputs/compositespinbox.h"
#include "animatedwidgets/inputs/compositedoublespinbox.h"

using animatedwidgets::styleeditor::StyleEditorWidgetType;
using animatedwidgets::styleeditor::StyleWidgetTypeOptions;

QString InputStylePlugin::pluginId() const {
    return QStringLiteral("animatedwidgets.inputs");
}

QString InputStylePlugin::pluginName() const {
    return QStringLiteral("Animated Widgets Inputs");
}

QList<StyleEditorWidgetType> InputStylePlugin::widgetTypes() const {
    StyleWidgetTypeOptions lineEdit;
    lineEdit.id = QStringLiteral("lineedit");
    lineEdit.name = QStringLiteral("Line Edit");
    lineEdit.category = QStringLiteral("Inputs");
    lineEdit.categoryOrder = 400.0;
    lineEdit.order = 100.0;
    lineEdit.suggestedSize = QSize(240, 40);
    lineEdit.defineProperties = &InputStyleProperties::lineEdit;

    StyleWidgetTypeOptions iconLineEdit;
    iconLineEdit.id = QStringLiteral("iconlineedit");
    iconLineEdit.name = QStringLiteral("Icon Line Edit");
    iconLineEdit.category = QStringLiteral("Inputs");
    iconLineEdit.categoryOrder = 400.0;
    iconLineEdit.order = 200.0;
    iconLineEdit.suggestedSize = QSize(240, 40);
    iconLineEdit.defineProperties = &InputStyleProperties::iconLineEdit;

    StyleWidgetTypeOptions passwordLineEdit;
    passwordLineEdit.id = QStringLiteral("passwordlineedit");
    passwordLineEdit.name = QStringLiteral("Password Line Edit");
    passwordLineEdit.category = QStringLiteral("Inputs");
    passwordLineEdit.categoryOrder = 400.0;
    passwordLineEdit.order = 300.0;
    passwordLineEdit.suggestedSize = QSize(240, 40);
    passwordLineEdit.defineProperties = &InputStyleProperties::passwordLineEdit;

    StyleWidgetTypeOptions plainTextEdit;
    plainTextEdit.id = QStringLiteral("plaintextedit");
    plainTextEdit.name = QStringLiteral("Plain Text Edit");
    plainTextEdit.category = QStringLiteral("Inputs");
    plainTextEdit.categoryOrder = 400.0;
    plainTextEdit.order = 400.0;
    plainTextEdit.suggestedSize = QSize(320, 150);
    plainTextEdit.defineProperties = &InputStyleProperties::plainTextEdit;

    StyleWidgetTypeOptions spinBox;
    spinBox.id = QStringLiteral("spinbox");
    spinBox.name = QStringLiteral("Spin Box");
    spinBox.category = QStringLiteral("Inputs");
    spinBox.categoryOrder = 400.0;
    spinBox.order = 500.0;
    spinBox.suggestedSize = QSize(240, 40);
    spinBox.defineProperties = &InputStyleProperties::spinBox;

    StyleWidgetTypeOptions doubleSpinBox;
    doubleSpinBox.id = QStringLiteral("doublespinbox");
    doubleSpinBox.name = QStringLiteral("Double Spin Box");
    doubleSpinBox.category = QStringLiteral("Inputs");
    doubleSpinBox.categoryOrder = 400.0;
    doubleSpinBox.order = 600.0;
    doubleSpinBox.suggestedSize = QSize(240, 40);
    doubleSpinBox.defineProperties = &InputStyleProperties::doubleSpinBox;

    StyleWidgetTypeOptions compositeLineEdit;
    compositeLineEdit.id = QStringLiteral("compositelineedit");
    compositeLineEdit.name = QStringLiteral("Composite Line Edit");
    compositeLineEdit.category = QStringLiteral("Inputs");
    compositeLineEdit.categoryOrder = 400.0;
    compositeLineEdit.order = 700.0;
    compositeLineEdit.suggestedSize = QSize(240, 40);
    compositeLineEdit.defineProperties = &InputStyleProperties::compositeLineEdit;

    StyleWidgetTypeOptions compositePlainTextEdit;
    compositePlainTextEdit.id = QStringLiteral("compositeplaintextedit");
    compositePlainTextEdit.name = QStringLiteral("Composite Plain Text Edit");
    compositePlainTextEdit.category = QStringLiteral("Inputs");
    compositePlainTextEdit.categoryOrder = 400.0;
    compositePlainTextEdit.order = 800.0;
    compositePlainTextEdit.suggestedSize = QSize(320, 150);
    compositePlainTextEdit.defineProperties = &InputStyleProperties::compositePlainTextEdit;

    StyleWidgetTypeOptions compositeSpinBox;
    compositeSpinBox.id = QStringLiteral("compositespinbox");
    compositeSpinBox.name = QStringLiteral("Composite Spin Box");
    compositeSpinBox.category = QStringLiteral("Inputs");
    compositeSpinBox.categoryOrder = 400.0;
    compositeSpinBox.order = 900.0;
    compositeSpinBox.suggestedSize = QSize(240, 40);
    compositeSpinBox.defineProperties = &InputStyleProperties::compositeSpinBox;

    StyleWidgetTypeOptions compositeDoubleSpinBox;
    compositeDoubleSpinBox.id = QStringLiteral("compositedoublespinbox");
    compositeDoubleSpinBox.name = QStringLiteral("Composite Double Spin Box");
    compositeDoubleSpinBox.category = QStringLiteral("Inputs");
    compositeDoubleSpinBox.categoryOrder = 400.0;
    compositeDoubleSpinBox.order = 1000.0;
    compositeDoubleSpinBox.suggestedSize = QSize(240, 40);
    compositeDoubleSpinBox.defineProperties = &InputStyleProperties::compositeDoubleSpinBox;

    return {
        StylePluginTypeUtils::create<animatedwidgets::LineEdit>(lineEdit, [](animatedwidgets::LineEdit* widget) {
            widget->setPlaceholderText(QStringLiteral("Preview"));
        }),
        StylePluginTypeUtils::create<animatedwidgets::IconLineEdit>(iconLineEdit, [](animatedwidgets::IconLineEdit* widget) {
            widget->setPlaceholderText(QStringLiteral("Preview"));
        }),
        StylePluginTypeUtils::create<animatedwidgets::PasswordLineEdit>(passwordLineEdit, [](animatedwidgets::PasswordLineEdit* widget) {
            widget->setPlaceholderText(QStringLiteral("Preview"));
        }),
        StylePluginTypeUtils::create<animatedwidgets::PlainTextEdit>(plainTextEdit, [](animatedwidgets::PlainTextEdit* widget) {
            widget->setPlaceholderText(QStringLiteral("Enter a message..."));
        }),
        StylePluginTypeUtils::create<animatedwidgets::SpinBox>(spinBox, [](animatedwidgets::SpinBox* widget) {
            widget->setValue(24);
        }),
        StylePluginTypeUtils::create<animatedwidgets::DoubleSpinBox>(doubleSpinBox, [](animatedwidgets::DoubleSpinBox* widget) {
            widget->setValue(24);
        }),
        StylePluginTypeUtils::create<animatedwidgets::CompositeLineEdit>(compositeLineEdit, [](animatedwidgets::CompositeLineEdit* widget) {
            widget->setPlaceholderText(QStringLiteral("Preview"));
        }),
        StylePluginTypeUtils::create<animatedwidgets::CompositePlainTextEdit>(compositePlainTextEdit, [](animatedwidgets::CompositePlainTextEdit* widget) {
            widget->setPlaceholderText(QStringLiteral("Enter a message..."));
        }),
        StylePluginTypeUtils::create<animatedwidgets::CompositeSpinBox>(compositeSpinBox, [](animatedwidgets::CompositeSpinBox* widget) {
            widget->setValue(24);
        }),
        StylePluginTypeUtils::create<animatedwidgets::CompositeDoubleSpinBox>(compositeDoubleSpinBox, [](animatedwidgets::CompositeDoubleSpinBox* widget) {
            widget->setValue(24);
        })
    };
}
