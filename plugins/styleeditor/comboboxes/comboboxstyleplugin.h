#pragma once

#include "animatedwidgets/styleeditor/api/styleeditorplugin.h"

#include <QObject>

class ComboBoxStylePlugin : public QObject, public animatedwidgets::styleeditor::StyleEditorPlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID ANIMATEDWIDGETS_STYLE_EDITOR_PLUGIN_IID FILE "comboboxstyleplugin.json")
    Q_INTERFACES(animatedwidgets::styleeditor::StyleEditorPlugin)

public:
    QString pluginId() const override;
    QString pluginName() const override;
    QList<animatedwidgets::styleeditor::StyleEditorWidgetType> widgetTypes() const override;
};
