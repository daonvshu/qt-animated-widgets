#pragma once

#include "animatedwidgets/styleeditor/api/stylepropertybuilder.h"

class ComboBoxStyleProperties {
public:
    static void comboBox(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void searchComboBox(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void creatableComboBox(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void treeComboBox(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void cascaderComboBox(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void multiSelectComboBox(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void compositeComboBox(animatedwidgets::styleeditor::StylePropertyBuilder& builder);

private:
    static animatedwidgets::styleeditor::StylePropertyGroup interaction();
    static animatedwidgets::styleeditor::StylePropertyGroup editing();
    static animatedwidgets::styleeditor::StylePropertyGroup arrow();
    static animatedwidgets::styleeditor::StylePropertyGroup popup();
    static animatedwidgets::styleeditor::StylePropertyGroup search();
    static animatedwidgets::styleeditor::StylePropertyGroup creation();
    static animatedwidgets::styleeditor::StylePropertyGroup hierarchy();
    static animatedwidgets::styleeditor::StylePropertyGroup treeSearch();
    static animatedwidgets::styleeditor::StylePropertyGroup cascader();
    static animatedwidgets::styleeditor::StylePropertyGroup multiContent();
    static animatedwidgets::styleeditor::StylePropertyGroup multiArrow();
    static animatedwidgets::styleeditor::StylePropertyGroup tags();
    static animatedwidgets::styleeditor::StylePropertyGroup tagCloseButton();
    static animatedwidgets::styleeditor::StylePropertyGroup multiPopup();
    static animatedwidgets::styleeditor::StylePropertyGroup compositeContent();
};
