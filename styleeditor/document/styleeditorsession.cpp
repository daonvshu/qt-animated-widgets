#include "styleeditorsession.h"

#include "../plugins/stylepluginregistry.h"
#include "../utils/styledocumentcodec.h"

#include <QUuid>

#include <algorithm>

using animatedwidgets::styleeditor::StyleEditorWidgetType;
using animatedwidgets::styleeditor::StyleProperty;

StyleEditorSession::StyleEditorSession(const StylePluginRegistry& registry, QObject* parent)
    : QObject(parent)
    , m_registry(registry)
{
}

const QList<StyleEditorItem>& StyleEditorSession::items() const {
    return m_items;
}

void StyleEditorSession::loadItems(const QList<StyleDocumentItem>& items) {
    clearItems();
    for (const StyleDocumentItem& stored : items) {
        StyleEditorItem item;
        static_cast<StyleDocumentItem&>(item) = stored;
        const StyleEditorWidgetType* type = m_registry.findType(item.widgetTypeId);
        if (type)
            initializeItem(item, *type);
        m_items.append(item);
    }
}

QList<StyleDocumentItem> StyleEditorSession::collectItems() const {
    QList<StyleDocumentItem> stored;
    for (const StyleEditorItem& item : m_items) {
        StyleDocumentItem data = item;
        const QJsonObject values = StyleDocumentCodec::serializeProperties(item.properties);
        for (auto it = values.constBegin(); it != values.constEnd(); ++it)
            data.serializedProperties.insert(it.key(), it.value());
        stored.append(data);
    }
    return stored;
}

int StyleEditorSession::addItem(const QString& typeId, const QString& displayName) {
    const StyleEditorWidgetType* type = m_registry.findType(typeId);
    if (!type || displayName.trimmed().isEmpty())
        return -1;

    StyleEditorItem item;
    item.id = typeId + QStringLiteral("-") + QUuid::createUuid().toString(QUuid::WithoutBraces);
    item.displayName = displayName.trimmed();
    item.widgetTypeId = typeId;
    if (!initializeItem(item, *type))
        return -1;

    m_items.append(item);
    emit contentChanged();
    return static_cast<int>(m_items.size()) - 1;
}

bool StyleEditorSession::updateItem(int itemIndex, const QString& typeId, const QString& displayName) {
    const StyleEditorWidgetType* type = m_registry.findType(typeId);
    if (itemIndex < 0 || itemIndex >= m_items.size() || !type || displayName.trimmed().isEmpty())
        return false;

    StyleEditorItem& item = m_items[itemIndex];
    const QString name = displayName.trimmed();
    if (item.widgetTypeId == typeId) {
        if (item.displayName == name)
            return true;
        item.displayName = name;
        item.generatedCode.clear();
    } else {
        StyleEditorItem replacement;
        replacement.id = item.id;
        replacement.displayName = name;
        replacement.widgetTypeId = typeId;
        replacement.previewSettings = item.previewSettings;
        if (!initializeItem(replacement, *type))
            return false;

        replacement.inheritedProperties = item.properties;
        replacement.serializedProperties = StyleDocumentCodec::serializeProperties(item.properties);
        item = replacement;
    }

    emit contentChanged();
    return true;
}

int StyleEditorSession::indexOfItem(const QString& id) const {
    for (int index = 0; index < m_items.size(); ++index) {
        if (m_items.at(index).id == id)
            return index;
    }
    return -1;
}

void StyleEditorSession::setPropertyValue(int itemIndex, const QString& name, const QVariant& value) {
    if (itemIndex < 0 || itemIndex >= m_items.size())
        return;

    for (StyleProperty& property : m_items[itemIndex].properties) {
        if (property.name != name)
            continue;
        if (property.value == value)
            return;

        property.value = value;
        m_items[itemIndex].generatedCode.clear();
        emit contentChanged();
        emit propertyValuesChanged(itemIndex);
        return;
    }
}

void StyleEditorSession::resetProperties(int itemIndex) {
    if (itemIndex < 0 || itemIndex >= m_items.size())
        return;

    for (StyleProperty& property : m_items[itemIndex].properties)
        property.value = property.defaultValue;
    m_items[itemIndex].generatedCode.clear();
    emit contentChanged();
    emit propertyValuesChanged(itemIndex);
}

void StyleEditorSession::setPreviewSettings(int itemIndex, const StylePreviewSettings& settings) {
    if (itemIndex < 0 || itemIndex >= m_items.size())
        return;

    StylePreviewSettings& current = m_items[itemIndex].previewSettings;
    if (current.size == settings.size && current.widthAuto == settings.widthAuto && current.heightAuto == settings.heightAuto)
        return;
    current = settings;
    emit contentChanged();
}

void StyleEditorSession::updatePreviewResult(int itemIndex, const QList<StyleProperty>& properties, const QString& code) {
    if (itemIndex < 0 || itemIndex >= m_items.size())
        return;
    if (!m_items[itemIndex].inheritedProperties.isEmpty())
        m_items[itemIndex].serializedProperties = {};
    m_items[itemIndex].properties = properties;
    m_items[itemIndex].inheritedProperties.clear();
    m_items[itemIndex].generatedCode = code;
}

const StyleResourceSettings& StyleEditorSession::resourceSettings() const {
    return m_resources;
}

void StyleEditorSession::setResourceSettings(const StyleResourceSettings& settings) {
    const bool historyChanged = m_resources.qrcFiles.size() != settings.qrcFiles.size()
        || !std::equal(m_resources.qrcFiles.cbegin(), m_resources.qrcFiles.cend(), settings.qrcFiles.cbegin());
    const bool changed = m_resources.projectDirectory != settings.projectDirectory || m_resources.source != settings.source || historyChanged;
    m_resources = settings;
    if (changed) {
        for (StyleEditorItem& item : m_items)
            item.generatedCode.clear();
        emit contentChanged();
    }
}

bool StyleEditorSession::initializeItem(StyleEditorItem& item, const StyleEditorWidgetType& type) {
    if (!type.create || !type.properties)
        return false;

    item.typeName = type.typeName;
    item.suggestedSize = type.suggestedSize;
    return true;
}

void StyleEditorSession::clearItems() {
    m_items.clear();
}
