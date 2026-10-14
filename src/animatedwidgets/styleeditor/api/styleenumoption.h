#pragma once

#include <QString>

namespace animatedwidgets::styleeditor {

struct StyleEnumOption {
    QString name;
    int value = 0;
    QString expression;
};

} // namespace animatedwidgets::styleeditor
