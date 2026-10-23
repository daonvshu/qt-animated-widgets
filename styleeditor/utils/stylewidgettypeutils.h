#pragma once

#include "animatedwidgets/styleeditor/api/styleeditorplugin.h"

class StyleWidgetTypeUtils {
public:
    static QString categoryName(const animatedwidgets::styleeditor::StyleEditorWidgetType& type);
    static void sort(QList<animatedwidgets::styleeditor::StyleEditorWidgetType>& types);

private:
    static double normalizedOrder(double order);
};
