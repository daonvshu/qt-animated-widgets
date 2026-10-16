#pragma once

#include "styleeditoritem.h"
#include "animatedwidgets/styleeditor/api/styleeditorplugin.h"

#include <QObject>

class StylePluginRegistry;

class StyleEditorSession : public QObject {
    Q_OBJECT

public:
    explicit StyleEditorSession(const StylePluginRegistry& registry, QObject* parent = nullptr);

    const QList<StyleEditorItem>& items() const;
    void loadItems(const QList<StyleDocumentItem>& items);
    QList<StyleDocumentItem> collectItems() const;
    int addItem(const QString& typeId, const QString& displayName);
    bool updateItem(int itemIndex, const QString& typeId, const QString& displayName);
    int indexOfItem(const QString& id) const;

    void setPropertyValue(int itemIndex, const QString& name, const QVariant& value);
    void resetProperties(int itemIndex);
    void updatePreviewResult(int itemIndex, const QList<animatedwidgets::styleeditor::StyleProperty>& properties, const QString& code);

    void setPreviewSettings(int itemIndex, const StylePreviewSettings& settings);

    const StyleResourceSettings& resourceSettings() const;
    void setResourceSettings(const StyleResourceSettings& settings);

signals:
    void contentChanged();
    void propertyValuesChanged(int itemIndex);

private:
    bool initializeItem(StyleEditorItem& item, const animatedwidgets::styleeditor::StyleEditorWidgetType& type);
    void clearItems();

    const StylePluginRegistry& m_registry;
    QList<StyleEditorItem> m_items;

    StyleResourceSettings m_resources;
};
