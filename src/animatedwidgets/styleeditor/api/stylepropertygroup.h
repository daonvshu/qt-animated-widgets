#pragma once

#include "stylepropertyoptions.h"

namespace animatedwidgets::styleeditor {

struct StylePropertyGroup {
    QString name;
    QList<StylePropertyOptions> properties;
};

} // namespace animatedwidgets::styleeditor
