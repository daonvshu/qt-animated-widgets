#pragma once

#include "stylepropertycondition.h"

#include <QList>
#include <QVariant>
#include <QString>
#include <QStringList>

#include <functional>

namespace animatedwidgets::styleeditor {

enum class StylePropertyType {
    Text,
    Color,
    Integer,
    Real,
    Boolean,
    Enum,
    Alignment,
    Icon,
    Size,
    KeySequence
};

struct StyleProperty {
    QString name;
    QString group;
    QString label;
    QString description;
    StylePropertyType type = StylePropertyType::Text;
    QVariant value;
    QVariant defaultValue;
    QStringList enumNames;
    QStringList enumExpressions;
    QList<StylePropertyCondition> enabledWhen;

    std::function<void(const QVariant&)> apply;
    std::function<QString(const QVariant&)> codeExpression;
    std::function<QString(const QVariant&)> codeStatement;
};

} // namespace animatedwidgets::styleeditor
