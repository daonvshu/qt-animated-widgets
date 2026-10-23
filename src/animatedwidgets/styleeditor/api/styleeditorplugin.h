#pragma once

#include "animatedwidgets/styleeditor/api/styleproperty.h"

#include <QWidget>

#include <functional>

namespace animatedwidgets::styleeditor {

struct StyleEditorWidgetType {
    QString id;
    QString name;
    QString typeName;
    QString category;
    std::function<QWidget*(QWidget*)> create;
    std::function<QList<StyleProperty>(QWidget*)> properties;
    QSize suggestedSize;

    double categoryOrder = 1000.0;
    double order = 1000.0;
};

class StyleEditorPlugin {
public:
    virtual ~StyleEditorPlugin() = default;

    virtual QString pluginId() const = 0;
    virtual QString pluginName() const = 0;
    virtual QList<StyleEditorWidgetType> widgetTypes() const = 0;
};

} // namespace animatedwidgets::styleeditor

#define ANIMATEDWIDGETS_STYLE_EDITOR_PLUGIN_IID "org.animatedwidgets.StyleEditorPlugin/6.0"
Q_DECLARE_INTERFACE(animatedwidgets::styleeditor::StyleEditorPlugin, ANIMATEDWIDGETS_STYLE_EDITOR_PLUGIN_IID)
