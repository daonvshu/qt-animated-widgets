#pragma once

#include "styleeditorplugin.h"
#include "stylepropertybuilder.h"
#include "stylewidgettypeoptions.h"

#include <QDebug>

#include <memory>
#include <type_traits>

namespace animatedwidgets::styleeditor {

class StyleWidgetTypeBuilder {
public:
    template<typename Widget>
    static StyleEditorWidgetType create(const StyleWidgetTypeOptions& options) {
        static_assert(std::is_base_of<QWidget, Widget>::value, "A style editor widget must derive from QWidget.");

        StyleEditorWidgetType type;
        type.id = options.id;
        type.name = options.name;
        type.category = options.category;
        type.typeName = options.typeName.isEmpty() ? QString::fromLatin1(Widget::staticMetaObject.className()) : options.typeName;
        type.suggestedSize = options.suggestedSize;
        type.categoryOrder = options.categoryOrder;
        type.order = options.order;
        type.create = [](QWidget* parent) { return new Widget(parent); };
        type.properties = [defineProperties = options.defineProperties](QWidget* widget) {
            if (!qobject_cast<Widget*>(widget) || !defineProperties)
                return QList<StyleProperty>();

            const auto defaults = std::make_unique<Widget>();
            StylePropertyBuilder builder(widget, defaults.get());
            defineProperties(builder);
            for (const QString& error : builder.errors())
                qWarning().noquote() << QStringLiteral("Style property definition:") << error;
            return builder.properties();
        };
        return type;
    }
};

} // namespace animatedwidgets::styleeditor
