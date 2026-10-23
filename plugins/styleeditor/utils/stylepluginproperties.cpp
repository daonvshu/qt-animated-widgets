#include "stylepluginproperties.h"

#include <QDebug>
#include <QEasingCurve>

using animatedwidgets::styleeditor::StylePropertyBuilder;
using animatedwidgets::styleeditor::StylePropertyGroup;
using animatedwidgets::styleeditor::StylePropertyOptions;

StylePropertyOptions StylePluginProperties::field(const QString& name, const QString& label) {
    StylePropertyOptions property;
    property.name = name;
    property.label = label;
    return property;
}

StylePropertyOptions StylePluginProperties::icon(const QString& name, const QString& label, const QString& resource) {
    auto property = field(name, label);
    property.binding = StylePropertyOptions::Binding::Icon;
    property.iconResource = resource;
    property.defaultIconResource = resource;
    return property;
}

StylePropertyOptions StylePluginProperties::enumeration(const QString& name, const QString& label, const QString& scope) {
    auto property = field(name, label);
    property.binding = StylePropertyOptions::Binding::Enumeration;
    property.enumScope = scope;
    return property;
}

StylePropertyOptions StylePluginProperties::alignment(const QString& name, const QString& label) {
    auto property = field(name, label);
    property.binding = StylePropertyOptions::Binding::Alignment;
    return property;
}

StylePropertyOptions StylePluginProperties::horizontalAlignment(const QString& name, const QString& label, bool includeVertical) {
    auto property = field(name, label);
    property.binding = StylePropertyOptions::Binding::Enumeration;
    property.enumOptions = {{QStringLiteral("Left"), Qt::AlignLeft, QStringLiteral("Qt::AlignLeft")},
                            {QStringLiteral("Center"), Qt::AlignHCenter, QStringLiteral("Qt::AlignHCenter")},
                            {QStringLiteral("Right"), Qt::AlignRight, QStringLiteral("Qt::AlignRight")}};
    if (includeVertical)
        property.enumOptions.append(animatedwidgets::styleeditor::StyleAlignmentUtils::options());
    return property;
}

StylePropertyOptions StylePluginProperties::when(StylePropertyOptions property, const QString& controller, const QVariant& value) {
    property.enabledWhen.append({controller, value});
    return property;
}

StylePropertyGroup StylePluginProperties::inputBackground() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Background");
    for (const QString& state :
         {QStringLiteral("Normal"), QStringLiteral("Hover"), QStringLiteral("Focus"), QStringLiteral("Error"), QStringLiteral("Disabled")})
        group.properties.append(field(QStringLiteral("background") + state, state));
    return group;
}

StylePropertyGroup StylePluginProperties::inputBorder() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Border");
    group.properties = {
        field(QStringLiteral("borderVisible"), QStringLiteral("Visible"))
    };
    for (const QString& state :
         {QStringLiteral("Normal"), QStringLiteral("Hover"), QStringLiteral("Focus"), QStringLiteral("Error"), QStringLiteral("Disabled")})
        group.properties.append(when(field(QStringLiteral("border") + state, state), QStringLiteral("borderVisible")));
    group.properties.append(when(field(QStringLiteral("borderWidth"), QStringLiteral("Width")), QStringLiteral("borderVisible")));
    return group;
}

StylePropertyGroup StylePluginProperties::inputShape() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Shape");
    group.properties = {
        field(QStringLiteral("cornerRadius"), QStringLiteral("Corner radius")),
        field(QStringLiteral("contentPadding"), QStringLiteral("Content padding"))
    };
    return group;
}

StylePropertyGroup StylePluginProperties::textColors() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Text colors");
    group.properties = {
        field(QStringLiteral("textColor"), QStringLiteral("Normal")),
        field(QStringLiteral("disabledTextColor"), QStringLiteral("Disabled"))
    };
    return group;
}

StylePropertyGroup StylePluginProperties::compositeLayout() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Composite layout");
    group.properties = {
        field(QStringLiteral("spacing"), QStringLiteral("Slot spacing")),
        field(QStringLiteral("drawInputLeftBorder"), QStringLiteral("Input left border")),
        field(QStringLiteral("drawInputRightBorder"), QStringLiteral("Input right border")),
        field(QStringLiteral("drawInputTopBorder"), QStringLiteral("Input top border")),
        field(QStringLiteral("drawInputBottomBorder"), QStringLiteral("Input bottom border"))
    };
    return group;
}

StylePropertyGroup StylePluginProperties::animation() {
    StylePropertyGroup group;
    group.name = QStringLiteral("Animation");
    group.properties = {
        field(QStringLiteral("animationDuration"), QStringLiteral("Duration (ms)"))
    };
    return group;
}

StylePropertyGroup StylePluginProperties::valueAnimation() {
    StylePropertyGroup group = animation();
    auto duration = when(group.properties.first(), QStringLiteral("animationEnabled"));
    auto easing = enumeration(QStringLiteral("animationEasingCurve"), QStringLiteral("Easing curve"), QStringLiteral("QEasingCurve"));
    const QMetaEnum easingTypes = QMetaEnum::fromType<QEasingCurve::Type>();
    for (int index = 0; index < easingTypes.keyCount(); ++index) {
        const int value = easingTypes.value(index);
        if (value < QEasingCurve::Linear || value > QEasingCurve::OutInBounce)
            continue;
        const QString name = QString::fromLatin1(easingTypes.key(index));
        easing.enumOptions.append({name, value, QStringLiteral("QEasingCurve::") + name});
    }
    group.properties = {
        field(QStringLiteral("animationEnabled"), QStringLiteral("Enabled")),
        duration,
        when(easing, QStringLiteral("animationEnabled"))
    };
    return group;
}

StylePropertyGroup StylePluginProperties::iconButtonColors(const QString& prefix, const QString& title) {
    StylePropertyGroup group;
    group.name = title;
    for (const QString& state : {QStringLiteral("Normal"), QStringLiteral("Hover"), QStringLiteral("Pressed"), QStringLiteral("Disabled")})
        group.properties.append(when(field(prefix + state + QStringLiteral("Color"), state), QStringLiteral("iconHint")));
    for (const QString& state : {QStringLiteral("Hover"), QStringLiteral("Pressed"), QStringLiteral("Disabled")})
        group.properties.append(field(prefix + QStringLiteral("Background") + state, QStringLiteral("Background ") + state));
    return group;
}

void StylePluginProperties::editorProperties(StylePropertyBuilder& builder, QObject* editor, QObject* defaults, const QList<StylePropertyGroup>& groups) {
    StylePropertyBuilder nested(editor, defaults);
    if (!nested.addGroups(groups)) {
        for (const QString& error : nested.errors())
            qWarning().noquote() << QStringLiteral("Editor property definition:") << error;
        return;
    }

    for (auto property : nested.properties()) {
        QString setter = property.name;
        setter[0] = setter.at(0).toUpper();
        const auto expression = property.codeExpression;
        property.codeStatement = [setter, expression](const QVariant& value) {
            return QStringLiteral("widget->editor()->set%1(%2);\n").arg(setter, expression(value));
        };
        property.name.prepend(QStringLiteral("editor."));
        property.group.prepend(QStringLiteral("Editor / "));
        for (auto& condition : property.enabledWhen)
            condition.propertyName.prepend(QStringLiteral("editor."));
        builder.add(property);
    }
}
