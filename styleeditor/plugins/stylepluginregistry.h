#pragma once

#include "animatedwidgets/styleeditor/api/styleeditorplugin.h"

#include <QObject>

class StylePluginRegistry : public QObject {
public:
    explicit StylePluginRegistry(QObject* parent = nullptr);

    void load(const QString& directory);
    const QList<animatedwidgets::styleeditor::StyleEditorWidgetType>& widgetTypes() const;
    const animatedwidgets::styleeditor::StyleEditorWidgetType* findType(const QString& id) const;

private:
    QList<animatedwidgets::styleeditor::StyleEditorWidgetType> m_widgetTypes;
};
