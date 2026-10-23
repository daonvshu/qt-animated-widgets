#pragma once

#include "animatedwidgets/styleeditor/api/stylewidgettypebuilder.h"

class StylePluginTypeUtils {
public:
    template <typename Widget>
    static animatedwidgets::styleeditor::StyleEditorWidgetType create(const animatedwidgets::styleeditor::StyleWidgetTypeOptions& options,
                                                                      const std::function<void(Widget*)>& initialize = {}) {
        auto type = animatedwidgets::styleeditor::StyleWidgetTypeBuilder::create<Widget>(options);
        type.create = [initialize](QWidget* parent) {
            auto* widget = new Widget(parent);
            if (initialize)
                initialize(widget);
            if (widget->isWindow())
                widget->setWindowFlags(Qt::Widget);
            return widget;
        };
        return type;
    }
};
