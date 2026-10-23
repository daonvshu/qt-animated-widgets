#pragma once

#include "animatedwidgets/styleeditor/api/stylepropertybuilder.h"

class SliderStyleProperties {
public:
    static void slider(animatedwidgets::styleeditor::StylePropertyBuilder& builder);

private:
    static animatedwidgets::styleeditor::StylePropertyGroup range();
    static animatedwidgets::styleeditor::StylePropertyGroup layout();
};
