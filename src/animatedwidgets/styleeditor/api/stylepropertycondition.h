#pragma once

#include <QString>
#include <QVariant>

namespace animatedwidgets::styleeditor {

struct StylePropertyCondition {
    QString propertyName;
    QVariant value;
};

} // namespace animatedwidgets::styleeditor
