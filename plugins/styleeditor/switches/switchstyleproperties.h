#pragma once

#include "animatedwidgets/styleeditor/api/stylepropertybuilder.h"

class SwitchStyleProperties {
public:
    static void switchControl(animatedwidgets::styleeditor::StylePropertyBuilder& builder);

private:
    static animatedwidgets::styleeditor::StylePropertyGroup content();
    static animatedwidgets::styleeditor::StylePropertyGroup track();
    static animatedwidgets::styleeditor::StylePropertyGroup handle();
};
