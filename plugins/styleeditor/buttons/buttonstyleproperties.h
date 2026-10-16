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
    static void addButtonProperties(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void addTextContent(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void addIconContent(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void addBackground(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void addHoverBackground(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void addBorder(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void addContentColors(animatedwidgets::styleeditor::StylePropertyBuilder& builder, bool iconOnly = false);
    static void addCornerRadius(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void addAnimation(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void addTextLayout(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void addIconShape(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
};
