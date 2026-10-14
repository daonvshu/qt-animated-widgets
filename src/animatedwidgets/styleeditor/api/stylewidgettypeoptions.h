#pragma once

#include <QSize>
#include <QString>

#include <functional>

namespace animatedwidgets::styleeditor {

class StylePropertyBuilder;

struct StyleWidgetTypeOptions {
    QString id;
    QString name;
    QString category;
    QString typeName;

    QSize suggestedSize;

    std::function<void(StylePropertyBuilder&)> defineProperties;
};

} // namespace animatedwidgets::styleeditor
