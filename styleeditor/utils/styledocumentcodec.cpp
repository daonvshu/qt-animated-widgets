#include "styledocumentcodec.h"
#include "styleresourceutils.h"

#include <QColor>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QSaveFile>

using animatedwidgets::styleeditor::StyleProperty;
using animatedwidgets::styleeditor::StylePropertyType;

bool StyleDocumentCodec::readFile(const QString& filePath, StyleDocument& document, QString& errorMessage) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        errorMessage = file.errorString();
        return false;
    }

    QJsonParseError parseError;
    const QJsonDocument json = QJsonDocument::fromJson(file.readAll(), &parseError);
    if (parseError.error != QJsonParseError::NoError || !json.isObject()) {
        errorMessage = parseError.error == QJsonParseError::NoError
            ? QStringLiteral("The document root must be a JSON object.")
            : parseError.errorString();
        return false;
    }

    const QJsonObject root = json.object();
    StyleDocument loaded;
    loaded.filePath = filePath;
    loaded.name = root.value(QStringLiteral("name")).toString(QStringLiteral("Untitled"));
    const QJsonObject documentPreview = root.value(QStringLiteral("preview")).toObject();
    const bool hasSharedBackground = documentPreview.contains(QStringLiteral("background"));
    loaded.previewBackground = QColor(documentPreview.value(QStringLiteral("background")).toString());
    const QJsonObject resources = root.value(QStringLiteral("resources")).toObject();
    const QString projectDirectory = resources.value(QStringLiteral("projectDirectory")).toString();
    if (!projectDirectory.isEmpty())
        loaded.resources.projectDirectory = QDir::cleanPath(QDir(QFileInfo(filePath).absolutePath()).absoluteFilePath(projectDirectory));
    loaded.resources.source = resources.value(QStringLiteral("source")).toString();
    const QJsonArray qrcFiles = resources.value(QStringLiteral("qrcFiles")).toArray();
    for (const QJsonValue& value : qrcFiles) {
        if (value.isString())
            loaded.resources.qrcFiles.append(value.toString());
    }
    loaded.resources.qrcFiles = StyleResourceUtils::qrcSources(loaded.resources);
    const QJsonArray items = root.value(QStringLiteral("items")).toArray();
    for (const QJsonValue& itemValue : items) {
        if (!itemValue.isObject())
            continue;
        const QJsonObject itemObject = itemValue.toObject();
        StyleDocumentItem item;
        item.id = itemObject.value(QStringLiteral("id")).toString();
        item.displayName = itemObject.value(QStringLiteral("name")).toString();
        item.widgetTypeId = itemObject.value(QStringLiteral("widgetType")).toString();
        item.serializedProperties = itemObject.value(QStringLiteral("properties")).toObject();
        const QJsonObject preview = itemObject.value(QStringLiteral("preview")).toObject();
        const int width = preview.value(QStringLiteral("width")).toInt();
        const int height = preview.value(QStringLiteral("height")).toInt();
        if (width >= 1 && width <= 4096)
            item.previewSettings.size.setWidth(width);
        if (height >= 1 && height <= 4096)
            item.previewSettings.size.setHeight(height);
        item.previewSettings.widthAuto = preview.value(QStringLiteral("widthAuto")).toBool();
        item.previewSettings.heightAuto = preview.value(QStringLiteral("heightAuto")).toBool();
        if (item.widgetTypeId.isEmpty())
            continue;
        if (item.id.isEmpty())
            item.id = item.widgetTypeId;
        if (item.displayName.isEmpty())
            item.displayName = item.widgetTypeId;
        if (!hasSharedBackground && !loaded.previewBackground.isValid())
            loaded.previewBackground = QColor(preview.value(QStringLiteral("background")).toString());
        loaded.items.append(item);
    }
    loaded.modified = false;
    document = loaded;
    return true;
}

void StyleDocumentCodec::applySerializedProperties(QList<StyleProperty>& properties, const QJsonObject& values) {
    for (StyleProperty& property : properties) {
        QVariant value;
        if (!jsonToVariant(property, values.value(property.name), value))
            continue;
        property.value = value;
        if (property.apply)
            property.apply(value);
    }
}

QJsonObject StyleDocumentCodec::serializeProperties(const QList<StyleProperty>& properties) {
    QJsonObject values;
    for (const StyleProperty& property : properties)
        values.insert(property.name, valueToJson(property));
    return values;
}

void StyleDocumentCodec::applyCompatibleProperties(QList<StyleProperty>& properties, const QList<StyleProperty>& previous) {
    for (StyleProperty& property : properties) {
        for (const StyleProperty& old : previous) {
            const bool enumCompatible = (property.type == StylePropertyType::Enum || property.type == StylePropertyType::Alignment)
                && (old.type == StylePropertyType::Enum || old.type == StylePropertyType::Alignment);
            const bool iconCompatible = property.type == StylePropertyType::Icon && old.type == StylePropertyType::Text;
            if (property.name != old.name || (property.type != old.type && !enumCompatible && !iconCompatible))
                continue;
            if (enumCompatible && !property.enumNames.contains(old.value.toString()))
                break;

            property.value = old.value;
            if (property.apply)
                property.apply(property.value);
            break;
        }
    }
}

QJsonObject StyleDocumentCodec::toJson(const StyleDocument& document) {
    QJsonObject root;
    root.insert(QStringLiteral("version"), 1);
    root.insert(QStringLiteral("name"), document.name);
    if (document.previewBackground.isValid()) {
        QJsonObject preview;
        preview.insert(QStringLiteral("background"), document.previewBackground.name(QColor::HexArgb));
        root.insert(QStringLiteral("preview"), preview);
    }
    if (!document.resources.projectDirectory.isEmpty()) {
        QJsonObject resources;
        const QDir directory(document.filePath.isEmpty() ? QDir::currentPath() : QFileInfo(document.filePath).absolutePath());
        resources.insert(QStringLiteral("projectDirectory"), QDir::fromNativeSeparators(directory.relativeFilePath(document.resources.projectDirectory)));
        resources.insert(QStringLiteral("source"), document.resources.source);
        const QStringList qrcFiles = StyleResourceUtils::qrcSources(document.resources);
        if (!qrcFiles.isEmpty())
            resources.insert(QStringLiteral("qrcFiles"), QJsonArray::fromStringList(qrcFiles));
        root.insert(QStringLiteral("resources"), resources);
    }

    QJsonArray items;
    for (const StyleDocumentItem& item : document.items) {
        QJsonObject itemObject;
        itemObject.insert(QStringLiteral("id"), item.id);
        itemObject.insert(QStringLiteral("name"), item.displayName);
        itemObject.insert(QStringLiteral("widgetType"), item.widgetTypeId);

        itemObject.insert(QStringLiteral("properties"), item.serializedProperties);
        QJsonObject preview;
        if (!item.previewSettings.widthAuto && item.previewSettings.size.width() > 0)
            preview.insert(QStringLiteral("width"), item.previewSettings.size.width());
        if (!item.previewSettings.heightAuto && item.previewSettings.size.height() > 0)
            preview.insert(QStringLiteral("height"), item.previewSettings.size.height());
        if (item.previewSettings.widthAuto)
            preview.insert(QStringLiteral("widthAuto"), true);
        if (item.previewSettings.heightAuto)
            preview.insert(QStringLiteral("heightAuto"), true);
        if (!preview.isEmpty())
            itemObject.insert(QStringLiteral("preview"), preview);
        items.append(itemObject);
    }
    root.insert(QStringLiteral("items"), items);
    return root;
}

bool StyleDocumentCodec::writeFile(const QString& filePath, const StyleDocument& document, QString& errorMessage) {
    QSaveFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        errorMessage = file.errorString();
        return false;
    }
    StyleDocument saved = document;
    saved.filePath = filePath;
    const QByteArray contents = QJsonDocument(toJson(saved)).toJson(QJsonDocument::Indented);
    if (file.write(contents) != contents.size() || !file.commit()) {
        errorMessage = file.errorString();
        return false;
    }
    return true;
}

QJsonValue StyleDocumentCodec::valueToJson(const StyleProperty& property) {
    return variantToJson(property, property.value);
}

QJsonValue StyleDocumentCodec::variantToJson(const StyleProperty& property, const QVariant& value) {
    if (property.type == StylePropertyType::Color) {
        const QColor color = value.value<QColor>();
        return color.isValid() ? QJsonValue(color.name(QColor::HexArgb)) : QJsonValue(QJsonValue::Null);
    }
    if (property.type == StylePropertyType::Boolean)
        return value.toBool();
    if (property.type == StylePropertyType::Integer)
        return value.toInt();
    if (property.type == StylePropertyType::Real)
        return value.toDouble();
    if (property.type == StylePropertyType::Size) {
        const QSize size = value.toSize();
        QJsonObject object;
        object.insert(QStringLiteral("width"), size.width());
        object.insert(QStringLiteral("height"), size.height());
        return object;
    }
    return value.toString();
}

bool StyleDocumentCodec::jsonToVariant(const StyleProperty& property, const QJsonValue& value, QVariant& result) {
    if (property.type == StylePropertyType::Color && value.isNull()) {
        result = QColor();
        return true;
    }
    if (value.isUndefined() || value.isNull())
        return false;
    switch (property.type) {
    case StylePropertyType::Size: {
        const QJsonObject object = value.toObject();
        const QJsonValue width = object.value(QStringLiteral("width"));
        const QJsonValue height = object.value(QStringLiteral("height"));
        if (!width.isDouble() || !height.isDouble() || width.toInt() < 1 || width.toInt() > 4096 || height.toInt() < 1 || height.toInt() > 4096)
            return false;
        result = QSize(width.toInt(), height.toInt());
        return true;
    }
    case StylePropertyType::Color: {
        const QColor color(value.toString());
        if (!color.isValid())
            return false;
        result = color;
        return true;
    }
    case StylePropertyType::Boolean:
        if (!value.isBool())
            return false;
        result = value.toBool();
        return true;
    case StylePropertyType::Integer:
        if (!value.isDouble())
            return false;
        result = value.toInt();
        return true;
    case StylePropertyType::Real:
        if (!value.isDouble())
            return false;
        result = value.toDouble();
        return true;
    case StylePropertyType::Alignment:
        if (!value.isString() || !property.enumNames.contains(value.toString()))
            return false;
        result = value.toString();
        return true;
    case StylePropertyType::Enum:
    case StylePropertyType::Text:
    default:
        if (!value.isString())
            return false;
        result = value.toString();
        return true;
    }
}
