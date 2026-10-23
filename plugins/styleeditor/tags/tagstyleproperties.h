#pragma once

#include "animatedwidgets/styleeditor/api/stylepropertybuilder.h"

class TagStyleProperties {
public:
    static void tagWidget(animatedwidgets::styleeditor::StylePropertyBuilder& builder);

private:
    static animatedwidgets::styleeditor::StylePropertyGroup content();
    static animatedwidgets::styleeditor::StylePropertyGroup colors();
    static animatedwidgets::styleeditor::StylePropertyGroup shape();
    static animatedwidgets::styleeditor::StylePropertyGroup closeButton();
};
