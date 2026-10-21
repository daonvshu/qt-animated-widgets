#pragma once

#include "styleenumoption.h"
#include "styleproperty.h"
#include "stylepropertygroup.h"
#include "animatedwidgets/styleeditor/utils/stylealignmentutils.h"
#include "animatedwidgets/styleeditor/utils/stylecodeexpressions.h"

#include <QIcon>
#include <QKeySequence>
#include <QMetaEnum>
#include <QMetaProperty>
#include <QPointer>
#include <QSize>

namespace animatedwidgets::styleeditor {

class StylePropertyBuilder {
public:
    StylePropertyBuilder(QObject* object, QObject* defaults)
        : m_object(object)
        , m_defaults(defaults)
    {
    }

    void setGroup(const QString& group) {
        m_group = group;
    }

    bool addGroups(const QList<StylePropertyGroup>& groups) {
        StylePropertyBuilder pending = *this;
        for (const StylePropertyGroup& group : groups) {
            pending.setGroup(group.name.isEmpty() ? m_group : group.name);
            for (const StylePropertyOptions& options : group.properties) {
                if (!pending.bind(options)) {
                    m_errors = pending.m_errors;
                    return false;
                }
            }
        }

        for (const StylePropertyGroup& group : groups) {
            for (const StylePropertyOptions& options : group.properties) {
                for (const StylePropertyCondition& condition : options.enabledWhen) {
                    if (!pending.enabledWhen(options.name, condition.propertyName, condition.value)) {
                        m_errors = pending.m_errors;
                        return false;
                    }
                }
            }
        }

        m_properties = pending.m_properties;
        return true;
    }

    bool add(const QString& name, const QString& label, const QString& description = {}) {
        const QMetaProperty meta = metaProperty(name);
        if (!meta.isValid())
            return false;

        if (meta.isEnumType()) {
            m_errors.append(QStringLiteral("Property '%1' requires an enum binding.").arg(name));
            return false;
        }

        StyleProperty property = describe(meta, name, label, description);
        switch (meta.userType()) {
            case QMetaType::QString:
                property.type = StylePropertyType::Text;
                property.codeExpression = [](const QVariant& value) { return StyleCodeExpressions::text(value.toString()); };
                break;
            case QMetaType::QColor:
                property.type = StylePropertyType::Color;
                property.codeExpression = [](const QVariant& value) { return StyleCodeExpressions::color(value.value<QColor>()); };
                break;
            case QMetaType::Bool:
                property.type = StylePropertyType::Boolean;
                property.codeExpression = [](const QVariant& value) { return value.toBool() ? QStringLiteral("true") : QStringLiteral("false"); };
                break;
            case QMetaType::Int:
                property.type = StylePropertyType::Integer;
                property.codeExpression = [](const QVariant& value) { return QString::number(value.toInt()); };
                break;
            case QMetaType::Double:
            case QMetaType::Float:
                property.type = StylePropertyType::Real;
                property.codeExpression = [](const QVariant& value) { return StyleCodeExpressions::real(value.toDouble()); };
                break;
            case QMetaType::QSize:
                property.type = StylePropertyType::Size;
                property.codeExpression = [](const QVariant& value) {
                    const QSize size = value.toSize();
                    return QStringLiteral("QSize(%1, %2)").arg(size.width()).arg(size.height());
                };
                break;
            case QMetaType::QKeySequence:
                property.type = StylePropertyType::KeySequence;
                property.value = meta.read(m_object.data()).value<QKeySequence>().toString(QKeySequence::PortableText);
                property.defaultValue = meta.read(m_defaults.data()).value<QKeySequence>().toString(QKeySequence::PortableText);
                property.codeExpression = [](const QVariant& value) {
                    return QStringLiteral("QKeySequence(%1, QKeySequence::PortableText)").arg(StyleCodeExpressions::text(value.toString()));
                };
                break;
            default:
                m_errors.append(QStringLiteral("Property '%1' requires an enum, icon or custom binding.").arg(name));
                return false;
        }

        property.apply = [object = m_object, meta](const QVariant& value) {
            if (object)
                meta.write(object.data(), meta.userType() == QMetaType::QKeySequence
                    ? QVariant::fromValue(QKeySequence(value.toString(), QKeySequence::PortableText)) : value);
        };
        return append(property);
    }

    bool enumeration(const QString& name, const QString& label, const QString& scope, const QString& description = {}) {
        const QMetaProperty meta = metaProperty(name);
        if (!meta.isValid())
            return false;
        const QMetaEnum values = meta.enumerator();
        if (!values.isValid() || values.isFlag()) {
            m_errors.append(QStringLiteral("Property '%1' requires explicit enum options.").arg(name));
            return false;
        }

        QList<StyleEnumOption> options;
        for (int index = 0; index < values.keyCount(); ++index) {
            const QString key = QString::fromLatin1(values.key(index));
            options.append({key, values.value(index), scope + QStringLiteral("::") + key});
        }
        return enumeration(name, label, options, description);
    }

    bool enumeration(const QString& name, const QString& label, const QList<StyleEnumOption>& options, const QString& description = {}) {
        const QMetaProperty meta = metaProperty(name);
        if (!meta.isValid())
            return false;
        if (!meta.isEnumType() && meta.userType() != QMetaType::Int) {
            m_errors.append(QStringLiteral("Property '%1' is not an enum or integer choice.").arg(name));
            return false;
        }

        StyleProperty property = describe(meta, name, label, description);
        property.type = StylePropertyType::Enum;
        const int current = property.value.toInt();
        const int defaultValue = property.defaultValue.toInt();
        property.value = QVariant();
        property.defaultValue = QVariant();
        for (const StyleEnumOption& option : options) {
            if (option.name.isEmpty() || option.expression.isEmpty() || property.enumNames.contains(option.name)) {
                m_errors.append(QStringLiteral("Property '%1' has invalid or duplicate enum options.").arg(name));
                return false;
            }
            property.enumNames.append(option.name);
            property.enumExpressions.append(option.expression);
            if (option.value == current)
                property.value = option.name;
            if (option.value == defaultValue)
                property.defaultValue = option.name;
        }
        if (!property.value.isValid() || !property.defaultValue.isValid()) {
            m_errors.append(QStringLiteral("Property '%1' has no option for its current or default value.").arg(name));
            return false;
        }

        property.apply = [object = m_object, meta, options](const QVariant& value) {
            if (!object)
                return;
            for (const StyleEnumOption& option : options) {
                if (option.name == value.toString()) {
                    meta.write(object.data(), option.value);
                    return;
                }
            }
        };
        property.codeExpression = [options](const QVariant& value) {
            for (const StyleEnumOption& option : options) {
                if (option.name == value.toString())
                    return option.expression;
            }
            return QString();
        };
        return append(property);
    }

    bool alignment(const QString& name, const QString& label, const QString& description = {}) {
        if (!enumeration(name, label, StyleAlignmentUtils::options(), description))
            return false;
        m_properties.last().type = StylePropertyType::Alignment;
        return true;
    }

    bool icon(const QString& name, const QString& label, const QString& resource = {}, const QString& defaultResource = {}, const QString& description = {}) {
        const QMetaProperty meta = metaProperty(name);
        if (!meta.isValid())
            return false;
        if (meta.userType() != QMetaType::QIcon) {
            m_errors.append(QStringLiteral("Property '%1' is not a QIcon.").arg(name));
            return false;
        }

        StyleProperty property = describe(meta, name, label, description);
        property.type = StylePropertyType::Icon;
        property.value = resource;
        property.defaultValue = defaultResource;
        property.apply = [object = m_object, meta](const QVariant& value) {
            if (object)
                meta.write(object.data(), QIcon(value.toString()));
        };
        property.codeExpression = [](const QVariant& value) { return StyleCodeExpressions::icon(value.toString()); };
        return append(property);
    }

    bool add(StyleProperty property) {
        if (property.group.isEmpty())
            property.group = m_group;
        return append(property);
    }

    bool enabledWhen(const QString& name, const QString& controllingProperty, const QVariant& value = true) {
        int targetIndex = -1;
        bool controllerExists = false;
        for (int index = 0; index < m_properties.size(); ++index) {
            const QString& propertyName = m_properties.at(index).name;
            if (propertyName == name)
                targetIndex = index;
            if (propertyName == controllingProperty)
                controllerExists = true;
        }
        if (targetIndex < 0 || !controllerExists || name == controllingProperty || !value.isValid()) {
            m_errors.append(QStringLiteral("Enable condition for '%1' requires distinct registered properties and a valid value.").arg(name));
            return false;
        }

        StyleProperty& property = m_properties[targetIndex];
        for (const StylePropertyCondition& condition : property.enabledWhen) {
            if (condition.propertyName == controllingProperty) {
                m_errors.append(QStringLiteral("Property '%1' already has an enable condition for '%2'.").arg(name, controllingProperty));
                return false;
            }
        }
        property.enabledWhen.append({controllingProperty, value});
        return true;
    }

    QList<StyleProperty> properties() const {
        return m_properties;
    }

    QStringList errors() const {
        return m_errors;
    }

private:
    bool bind(const StylePropertyOptions& options) {
        switch (options.binding) {
            case StylePropertyOptions::Binding::Automatic:
                return add(options.name, options.label, options.description);
            case StylePropertyOptions::Binding::Icon:
                return icon(options.name, options.label, options.iconResource, options.defaultIconResource, options.description);
            case StylePropertyOptions::Binding::Enumeration:
                if (!options.enumOptions.isEmpty())
                    return enumeration(options.name, options.label, options.enumOptions, options.description);
                if (!options.enumScope.isEmpty())
                    return enumeration(options.name, options.label, options.enumScope, options.description);
                break;
            case StylePropertyOptions::Binding::Alignment:
                return alignment(options.name, options.label, options.description);
        }

        m_errors.append(QStringLiteral("Property '%1' has an invalid or incomplete binding.").arg(options.name));
        return false;
    }

    QMetaProperty metaProperty(const QString& name) {
        const int index = m_object ? m_object->metaObject()->indexOfProperty(name.toLatin1().constData()) : -1;
        const int defaultIndex = m_defaults ? m_defaults->metaObject()->indexOfProperty(name.toLatin1().constData()) : -1;
        const QMetaProperty meta = index >= 0 ? m_object->metaObject()->property(index) : QMetaProperty();
        const QMetaProperty defaults = defaultIndex >= 0 ? m_defaults->metaObject()->property(defaultIndex) : QMetaProperty();
        if (!meta.isValid() || !meta.isReadable() || !meta.isWritable() || !defaults.isReadable() || meta.userType() != defaults.userType()) {
            m_errors.append(QStringLiteral("Property '%1' must be readable, writable and present on the default object with the same type.").arg(name));
            return QMetaProperty();
        }
        return meta;
    }

    StyleProperty describe(const QMetaProperty& meta, const QString& name, const QString& label, const QString& description) const {
        StyleProperty property;
        property.name = name;
        property.group = m_group;
        property.label = label;
        property.description = description;
        property.value = meta.read(m_object.data());
        property.defaultValue = m_defaults->property(name.toLatin1().constData());
        return property;
    }

    bool append(const StyleProperty& property) {
        if (property.name.isEmpty() || !property.apply || !property.codeExpression) {
            m_errors.append(QStringLiteral("A property requires a name, an apply callback and a code expression."));
            return false;
        }
        for (const StyleProperty& existing : m_properties) {
            if (existing.name == property.name) {
                m_errors.append(QStringLiteral("Property '%1' is already defined.").arg(property.name));
                return false;
            }
        }
        m_properties.append(property);
        return true;
    }

    QPointer<QObject> m_object;
    QPointer<QObject> m_defaults;
    QString m_group;

    QList<StyleProperty> m_properties;
    QStringList m_errors;
};

} // namespace animatedwidgets::styleeditor
