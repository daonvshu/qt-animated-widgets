#pragma once

#include "animatedwidgets/styleeditor/api/stylepropertybuilder.h"

class SelectionStyleProperties {
public:
    static void checkBox(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void radioButton(animatedwidgets::styleeditor::StylePropertyBuilder& builder);

private:
    static void checkState(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static animatedwidgets::styleeditor::StylePropertyGroup selection();
    static animatedwidgets::styleeditor::StylePropertyGroup checkOptions();
    static animatedwidgets::styleeditor::StylePropertyGroup radioOptions();
    static animatedwidgets::styleeditor::StylePropertyGroup checkIcons();
    static animatedwidgets::styleeditor::StylePropertyGroup radioIcons();
    static animatedwidgets::styleeditor::StylePropertyGroup iconColors();
    static animatedwidgets::styleeditor::StylePropertyGroup partialColor();
};
