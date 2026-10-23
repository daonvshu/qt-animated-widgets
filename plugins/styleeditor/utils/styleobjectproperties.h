#pragma once

#include "animatedwidgets/styleeditor/api/stylepropertybuilder.h"

#include <QFont>
#include <QMargins>

#include <type_traits>

class StyleObjectProperties {
public:
    template <typename Widget, typename Style, typename Value, typename Getter, typename Setter>
    static void choice(animatedwidgets::styleeditor::StylePropertyBuilder& builder, const QString& group, const QString& name, const QString& label,
                       Value Style::* member, Getter getter, Setter setter, const QList<animatedwidgets::styleeditor::StyleEnumOption>& options,
                       const QString& getterCode, const QString& setterCode) {
        auto* widget = qobject_cast<Widget*>(builder.object());
        auto* defaults = qobject_cast<Widget*>(builder.defaults());
        if (!widget || !defaults)
            return;

        animatedwidgets::styleeditor::StyleProperty property;
        property.name = name;
        property.group = group;
        property.label = label;
        property.type = animatedwidgets::styleeditor::StylePropertyType::Enum;
        for (const auto& option : options) {
            property.enumNames.append(option.name);
            property.enumExpressions.append(option.expression);
            if (int(getter(widget).*member) == option.value)
                property.value = option.name;
            if (int(getter(defaults).*member) == option.value)
                property.defaultValue = option.name;
        }
        property.apply = [target = QPointer<Widget>(widget), member, getter, setter, options](const QVariant& value) {
            if (!target)
                return;
            for (const auto& option : options) {
                if (option.name == value.toString()) {
                    Style style = getter(target.data());
                    style.*member = Value(option.value);
                    setter(target.data(), style);
                    return;
                }
            }
        };
        property.codeExpression = [options](const QVariant& value) {
            for (const auto& option : options) {
                if (option.name == value.toString())
                    return option.expression;
            }
            return QString();
        };
        property.codeStatement = [getterCode, setterCode, name, expression = property.codeExpression](const QVariant& value) {
            return QStringLiteral("{\n    auto style = %1;\n    style.%2 = %3;\n    widget->%4;\n}\n")
                    .arg(getterCode, name.section(QLatin1Char('.'), -1), expression(value), setterCode);
        };
        builder.add(property);
    }

    template <typename Widget, typename Value, typename Getter, typename Setter>
    static void method(animatedwidgets::styleeditor::StylePropertyBuilder& builder, const QString& group, const QString& name, const QString& label,
                       Getter getter, Setter setter, const QString& setterCode) {
        auto* widget = qobject_cast<Widget*>(builder.object());
        auto* defaults = qobject_cast<Widget*>(builder.defaults());
        if (!widget || !defaults)
            return;

        animatedwidgets::styleeditor::StyleProperty property;
        property.name = name;
        property.group = group;
        property.label = label;
        property.type = type<Value>();
        property.value = QVariant::fromValue(getter(widget));
        property.defaultValue = QVariant::fromValue(getter(defaults));
        property.apply = [target = QPointer<Widget>(widget), setter](const QVariant& value) {
            if (target)
                setter(target.data(), value.value<Value>());
        };
        property.codeExpression = &expression<Value>;
        property.codeStatement = [setterCode](const QVariant& value) { return QStringLiteral("widget->%1;\n").arg(setterCode.arg(expression<Value>(value))); };
        builder.add(property);
    }

    template <typename Widget, typename Style, typename Value, typename Getter, typename Setter>
    static void add(animatedwidgets::styleeditor::StylePropertyBuilder& builder, const QString& group, const QString& name, const QString& label,
                    Value Style::* member, Getter getter, Setter setter, const QString& getterCode, const QString& setterCode) {
        bind<Widget, Style, Value>(
                builder, group, name, label, getter, setter, [member](const Style& style) { return style.*member; },
                [member](Style& style, const Value& value) { style.*member = value; }, getterCode, setterCode,
                name.section(QLatin1Char('.'), -1) + QStringLiteral(" = %1;"));
    }

    template <typename Widget, typename Style, typename Getter, typename Setter>
    static void margins(animatedwidgets::styleeditor::StylePropertyBuilder& builder, const QString& group, const QString& name, QMargins Style::* member,
                        Getter getter, Setter setter, const QString& getterCode, const QString& setterCode) {
        const QStringList edges = {QStringLiteral("Left"), QStringLiteral("Top"), QStringLiteral("Right"), QStringLiteral("Bottom")};
        for (int edge = 0; edge < edges.size(); ++edge) {
            bind<Widget, Style, int>(
                    builder, group, name + edges.at(edge), edges.at(edge), getter, setter,
                    [member, edge](const Style& style) {
                        const auto margins = style.*member;
                        switch (edge) {
                            case 0:
                                return margins.left();
                            case 1:
                                return margins.top();
                            case 2:
                                return margins.right();
                            default:
                                return margins.bottom();
                        }
                    },
                    [member, edge](Style& style, int value) {
                        auto& margins = style.*member;
                        switch (edge) {
                            case 0:
                                margins.setLeft(value);
                                break;
                            case 1:
                                margins.setTop(value);
                                break;
                            case 2:
                                margins.setRight(value);
                                break;
                            default:
                                margins.setBottom(value);
                                break;
                        }
                    },
                    getterCode, setterCode, name.section(QLatin1Char('.'), -1) + QStringLiteral(".set") + edges.at(edge) + QStringLiteral("(%1);"));
        }
    }

    template <typename Widget, typename Style, typename Getter, typename Setter>
    static void font(animatedwidgets::styleeditor::StylePropertyBuilder& builder, const QString& group, const QString& name, QFont Style::* member,
                     Getter getter, Setter setter, const QString& getterCode, const QString& setterCode) {
        const QString field = name.section(QLatin1Char('.'), -1);
        auto* defaults = qobject_cast<Widget*>(builder.defaults());
        if (!defaults)
            return;
        const QFont defaultFont = getter(defaults).*member;
        bind<Widget, Style, QString>(
                builder, group, name + QStringLiteral("Family"), QStringLiteral("Family"), getter, setter,
                [member](const Style& style) { return (style.*member).family(); },
                [member](Style& style, const QString& value) { (style.*member).setFamily(value); }, getterCode, setterCode,
                field + QStringLiteral(".setFamily(%1);"));
        bind<Widget, Style, int>(
                builder, group, name + QStringLiteral("PixelSize"), QStringLiteral("Pixel size"), getter, setter,
                [member](const Style& style) { return (style.*member).pixelSize(); },
                [member, defaultFont](Style& style, int value) {
                    auto& font = style.*member;
                    if (value > 0) {
                        font.setPixelSize(value);
                    } else {
                        QFont restored = defaultFont;
                        restored.setFamily(font.family());
                        restored.setWeight(font.weight());
                        restored.setItalic(font.italic());
                        font = restored;
                    }
                },
                getterCode, setterCode, QStringLiteral("if (%1 > 0) style.") + field + QStringLiteral(".setPixelSize(%1);"));
        bind<Widget, Style, bool>(
                builder, group, name + QStringLiteral("Bold"), QStringLiteral("Bold"), getter, setter,
                [member](const Style& style) { return (style.*member).bold(); }, [member](Style& style, bool value) { (style.*member).setBold(value); },
                getterCode, setterCode, field + QStringLiteral(".setBold(%1);"));
        bind<Widget, Style, bool>(
                builder, group, name + QStringLiteral("Italic"), QStringLiteral("Italic"), getter, setter,
                [member](const Style& style) { return (style.*member).italic(); }, [member](Style& style, bool value) { (style.*member).setItalic(value); },
                getterCode, setterCode, field + QStringLiteral(".setItalic(%1);"));
    }

private:
    template <typename Value>
    static animatedwidgets::styleeditor::StylePropertyType type() {
        using animatedwidgets::styleeditor::StylePropertyType;
        if constexpr (std::is_same_v<Value, QColor>)
            return StylePropertyType::Color;
        else if constexpr (std::is_same_v<Value, bool>)
            return StylePropertyType::Boolean;
        else if constexpr (std::is_same_v<Value, int>)
            return StylePropertyType::Integer;
        else if constexpr (std::is_same_v<Value, QSize>)
            return StylePropertyType::Size;
        else if constexpr (std::is_same_v<Value, QIcon>)
            return StylePropertyType::Icon;
        else
            return StylePropertyType::Text;
    }

    template <typename Value>
    static QString expression(const QVariant& value) {
        using animatedwidgets::styleeditor::StyleCodeExpressions;
        if constexpr (std::is_same_v<Value, QColor>)
            return StyleCodeExpressions::color(value.value<QColor>());
        else if constexpr (std::is_same_v<Value, bool>)
            return value.toBool() ? QStringLiteral("true") : QStringLiteral("false");
        else if constexpr (std::is_same_v<Value, int>)
            return QString::number(value.toInt());
        else if constexpr (std::is_same_v<Value, QSize>)
            return QStringLiteral("QSize(%1, %2)").arg(value.toSize().width()).arg(value.toSize().height());
        else if constexpr (std::is_same_v<Value, QIcon>)
            return StyleCodeExpressions::icon(value.toString());
        else
            return StyleCodeExpressions::text(value.toString());
    }

    template <typename Widget, typename Style, typename Value, typename Getter, typename Setter, typename Reader, typename Writer>
    static void bind(animatedwidgets::styleeditor::StylePropertyBuilder& builder, const QString& group, const QString& name, const QString& label,
                     Getter getter, Setter setter, Reader read, Writer write, const QString& getterCode, const QString& setterCode, const QString& assignment) {
        auto* widget = qobject_cast<Widget*>(builder.object());
        auto* defaults = qobject_cast<Widget*>(builder.defaults());
        if (!widget || !defaults)
            return;

        animatedwidgets::styleeditor::StyleProperty property;
        property.name = name;
        property.group = group;
        property.label = label;
        property.type = type<Value>();
        if constexpr (std::is_same_v<Value, QIcon>) {
            property.value = QString();
            property.defaultValue = QString();
        } else {
            property.value = QVariant::fromValue(read(getter(widget)));
            property.defaultValue = QVariant::fromValue(read(getter(defaults)));
        }
        property.apply = [target = QPointer<Widget>(widget), getter, setter, write](const QVariant& value) {
            if (!target)
                return;
            Style style = getter(target.data());
            if constexpr (std::is_same_v<Value, QIcon>)
                write(style, QIcon(value.toString()));
            else
                write(style, value.value<Value>());
            setter(target.data(), style);
        };
        property.codeExpression = &expression<Value>;
        property.codeStatement = [getterCode, setterCode, assignment](const QVariant& value) {
            return QStringLiteral("{\n    auto style = %1;\n    style.%2\n    widget->%3;\n}\n")
                    .arg(getterCode, assignment.arg(expression<Value>(value)), setterCode);
        };
        builder.add(property);
    }
};
