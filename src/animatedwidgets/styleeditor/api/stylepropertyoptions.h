#pragma once

#include "styleenumoption.h"
#include "stylepropertycondition.h"

#include <QList>
#include <QString>

namespace animatedwidgets::styleeditor {

struct StylePropertyOptions {
    enum class Binding { Automatic, Icon, Enumeration, Alignment };

    QString name;
    QString label;
    QString description;
    Binding binding = Binding::Automatic;

    QString enumScope;
    QList<StyleEnumOption> enumOptions;

    QString iconResource;
    QString defaultIconResource;

    QList<StylePropertyCondition> enabledWhen;
};

} // namespace animatedwidgets::styleeditor
