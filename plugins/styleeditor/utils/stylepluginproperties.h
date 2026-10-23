#pragma once

#include "animatedwidgets/styleeditor/api/stylepropertybuilder.h"

class StylePluginProperties {
public:
    static animatedwidgets::styleeditor::StylePropertyOptions field(const QString& name, const QString& label);
    static animatedwidgets::styleeditor::StylePropertyOptions icon(const QString& name, const QString& label, const QString& resource = {});
    static animatedwidgets::styleeditor::StylePropertyOptions enumeration(const QString& name, const QString& label, const QString& scope);
    static animatedwidgets::styleeditor::StylePropertyOptions alignment(const QString& name, const QString& label);
    static animatedwidgets::styleeditor::StylePropertyOptions horizontalAlignment(const QString& name, const QString& label, bool includeVertical = false);
    static animatedwidgets::styleeditor::StylePropertyOptions when(animatedwidgets::styleeditor::StylePropertyOptions property, const QString& controller,
                                                                   const QVariant& value = true);

    static animatedwidgets::styleeditor::StylePropertyGroup inputBackground();
    static animatedwidgets::styleeditor::StylePropertyGroup inputBorder();
    static animatedwidgets::styleeditor::StylePropertyGroup inputShape();
    static animatedwidgets::styleeditor::StylePropertyGroup textColors();
    static animatedwidgets::styleeditor::StylePropertyGroup compositeLayout();
    static animatedwidgets::styleeditor::StylePropertyGroup animation();
    static animatedwidgets::styleeditor::StylePropertyGroup valueAnimation();
    static animatedwidgets::styleeditor::StylePropertyGroup iconButtonColors(const QString& prefix, const QString& title);

    static void editorProperties(animatedwidgets::styleeditor::StylePropertyBuilder& builder, QObject* editor, QObject* defaults,
                                 const QList<animatedwidgets::styleeditor::StylePropertyGroup>& groups);
};
