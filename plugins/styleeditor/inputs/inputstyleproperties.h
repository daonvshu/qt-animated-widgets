#pragma once

#include "animatedwidgets/styleeditor/api/stylepropertybuilder.h"

class InputStyleProperties {
public:
    static void lineEdit(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void iconLineEdit(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void passwordLineEdit(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void plainTextEdit(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void spinBox(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void doubleSpinBox(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void compositeLineEdit(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void compositePlainTextEdit(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void compositeSpinBox(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void compositeDoubleSpinBox(animatedwidgets::styleeditor::StylePropertyBuilder& builder);

private:
    static animatedwidgets::styleeditor::StylePropertyGroup interaction();
    static animatedwidgets::styleeditor::StylePropertyGroup lineContent();
    static animatedwidgets::styleeditor::StylePropertyGroup compositeLineContent();
    static animatedwidgets::styleeditor::StylePropertyGroup multilineContent();
    static animatedwidgets::styleeditor::StylePropertyGroup numberContent();
    static animatedwidgets::styleeditor::StylePropertyGroup compositeNumberContent();
    static animatedwidgets::styleeditor::StylePropertyGroup decimals();
    static animatedwidgets::styleeditor::StylePropertyGroup iconLayout();
    static animatedwidgets::styleeditor::StylePropertyGroup passwordIconLayout();
    static animatedwidgets::styleeditor::StylePropertyGroup password();
    static animatedwidgets::styleeditor::StylePropertyGroup stepButtons();
};
