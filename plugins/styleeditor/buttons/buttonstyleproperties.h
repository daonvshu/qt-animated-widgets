#pragma once

#include "animatedwidgets/styleeditor/api/stylepropertybuilder.h"

class ButtonStyleProperties {
public:
    static void filled(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void outline(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void bordered(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void hover(animatedwidgets::styleeditor::StylePropertyBuilder& builder);

    static void filledIcon(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void outlineIcon(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void borderedIcon(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void hoverIcon(animatedwidgets::styleeditor::StylePropertyBuilder& builder);

private:
    static animatedwidgets::styleeditor::StylePropertyGroup buttonProperties();
    static animatedwidgets::styleeditor::StylePropertyGroup textContent();
    static animatedwidgets::styleeditor::StylePropertyGroup background();
    static animatedwidgets::styleeditor::StylePropertyGroup hoverBackground();
    static animatedwidgets::styleeditor::StylePropertyGroup border();
    static animatedwidgets::styleeditor::StylePropertyGroup contentColors(bool iconOnly = false);
    static animatedwidgets::styleeditor::StylePropertyGroup cornerRadius();
    static animatedwidgets::styleeditor::StylePropertyGroup animation(bool iconOnly = false);
    static animatedwidgets::styleeditor::StylePropertyGroup textLayout();
    static animatedwidgets::styleeditor::StylePropertyGroup iconShape();
};
