#pragma once

#include "animatedwidgets/styleeditor/api/stylepropertybuilder.h"

class ProgressBarStyleProperties {
public:
    static void progressBar(animatedwidgets::styleeditor::StylePropertyBuilder& builder);

private:
    static animatedwidgets::styleeditor::StylePropertyGroup content();
    static animatedwidgets::styleeditor::StylePropertyGroup appearance();
};
