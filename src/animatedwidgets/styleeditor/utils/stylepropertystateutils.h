#pragma once

#include "animatedwidgets/styleeditor/api/styleproperty.h"

namespace animatedwidgets::styleeditor {

class StylePropertyStateUtils {
public:
    static bool isEnabled(const StyleProperty& property, const QList<StyleProperty>& properties) {
        for (const StylePropertyCondition& condition : property.enabledWhen) {
            if (condition.propertyName.isEmpty() || !condition.value.isValid())
                return false;

            bool matched = false;
            for (const StyleProperty& controller : properties) {
                if (controller.name != condition.propertyName)
                    continue;
                matched = controller.value.isValid() && controller.value == condition.value;
                break;
            }
            if (!matched)
                return false;
        }
        return true;
    }
};

} // namespace animatedwidgets::styleeditor
