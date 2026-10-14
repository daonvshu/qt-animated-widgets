#include "stylepluginregistry.h"

#include <QDir>
#include <QPluginLoader>

using animatedwidgets::styleeditor::StyleEditorWidgetType;
using animatedwidgets::styleeditor::StyleEditorPlugin;

StylePluginRegistry::StylePluginRegistry(QObject* parent)
    : QObject(parent)
{
}

void StylePluginRegistry::load(const QString& directory) {
    const QDir pluginDir(directory);
    for (const QString& file : pluginDir.entryList(QDir::Files)) {
        auto* loader = new QPluginLoader(pluginDir.absoluteFilePath(file), this);
        auto* plugin = qobject_cast<StyleEditorPlugin*>(loader->instance());
        if (!plugin) {
            delete loader;
            continue;
        }

        m_widgetTypes.append(plugin->widgetTypes());
    }
}

const QList<StyleEditorWidgetType>& StylePluginRegistry::widgetTypes() const {
    return m_widgetTypes;
}

const StyleEditorWidgetType* StylePluginRegistry::findType(const QString& id) const {
    for (const StyleEditorWidgetType& type : m_widgetTypes) {
        if (type.id == id)
            return &type;
    }
    return nullptr;
}
