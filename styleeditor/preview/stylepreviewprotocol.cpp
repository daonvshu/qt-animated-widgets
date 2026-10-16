#include "stylepreviewprotocol.h"

#include "../utils/styledocumentcodec.h"

#include <QJsonDocument>
#include <QJsonParseError>
#include <QtEndian>

using animatedwidgets::styleeditor::StyleProperty;
using animatedwidgets::styleeditor::StylePropertyCondition;
using animatedwidgets::styleeditor::StylePropertyType;

QByteArray StylePreviewProtocol::encode(QJsonObject message) {
    message.insert(QStringLiteral("version"), Version);
    const QByteArray payload = QJsonDocument(message).toJson(QJsonDocument::Compact);
    if (payload.size() > static_cast<int>(MaximumFrameSize))
        return {};

    QByteArray frame(4, '\0');
    qToBigEndian<quint32>(static_cast<quint32>(payload.size()), reinterpret_cast<uchar*>(frame.data()));
    frame.append(payload);
    return frame;
}

bool StylePreviewProtocol::takeMessages(QByteArray& buffer, QList<QJsonObject>& messages, QString& error) {
    error.clear();
    while (buffer.size() >= 4) {
        const quint32 length = qFromBigEndian<quint32>(reinterpret_cast<const uchar*>(buffer.constData()));
        if (length == 0 || length > MaximumFrameSize) {
            error = QStringLiteral("Invalid preview message size.");
            return false;
        }
        if (buffer.size() - 4 < static_cast<int>(length))
            return true;

        QJsonParseError parseError;
        const QJsonDocument document = QJsonDocument::fromJson(buffer.mid(4, static_cast<int>(length)), &parseError);
        if (parseError.error != QJsonParseError::NoError || !document.isObject()) {
            error = QStringLiteral("Invalid preview message JSON.");
            return false;
        }
        const QJsonObject message = document.object();
        if (message.value(QStringLiteral("version")).toInt(-1) != Version) {
            error = QStringLiteral("Incompatible preview protocol version.");
            return false;
        }
        messages.append(message);
        buffer.remove(0, 4 + static_cast<int>(length));
    }
    return true;
}

QJsonArray StylePreviewProtocol::encodeProperties(const QList<StyleProperty>& properties) {
    QJsonArray result;
    for (const StyleProperty& property : properties) {
        QJsonObject object;
        object.insert(QStringLiteral("name"), property.name);
        object.insert(QStringLiteral("group"), property.group);
        object.insert(QStringLiteral("label"), property.label);
        object.insert(QStringLiteral("description"), property.description);
        object.insert(QStringLiteral("type"), static_cast<int>(property.type));
        object.insert(QStringLiteral("value"), StyleDocumentCodec::variantToJson(property, property.value));
        object.insert(QStringLiteral("hasDefault"), property.defaultValue.isValid());
        object.insert(QStringLiteral("default"), StyleDocumentCodec::variantToJson(property, property.defaultValue));
        object.insert(QStringLiteral("enumNames"), QJsonArray::fromStringList(property.enumNames));
        object.insert(QStringLiteral("enumExpressions"), QJsonArray::fromStringList(property.enumExpressions));
        QJsonArray conditions;
        for (const StylePropertyCondition& condition : property.enabledWhen) {
            QJsonObject entry;
            entry.insert(QStringLiteral("name"), condition.propertyName);
            entry.insert(QStringLiteral("value"), QJsonValue::fromVariant(condition.value));
            conditions.append(entry);
        }
        object.insert(QStringLiteral("enabledWhen"), conditions);
        result.append(object);
    }
    return result;
}

bool StylePreviewProtocol::decodeProperties(const QJsonArray& data, QList<StyleProperty>& properties) {
    QList<StyleProperty> decoded;
    for (const QJsonValue& value : data) {
        if (!value.isObject())
            return false;
        const QJsonObject object = value.toObject();
        const int type = object.value(QStringLiteral("type")).toInt(-1);
        if (type < static_cast<int>(StylePropertyType::Text) || type > static_cast<int>(StylePropertyType::KeySequence))
            return false;

        StyleProperty property;
        property.type = static_cast<StylePropertyType>(type);
        property.name = object.value(QStringLiteral("name")).toString();
        if (property.name.isEmpty())
            return false;
        for (const StyleProperty& previous : decoded) {
            if (previous.name == property.name)
                return false;
        }
        property.group = object.value(QStringLiteral("group")).toString();
        property.label = object.value(QStringLiteral("label")).toString();
        property.description = object.value(QStringLiteral("description")).toString();
        for (const QJsonValue& name : object.value(QStringLiteral("enumNames")).toArray())
            property.enumNames.append(name.toString());
        for (const QJsonValue& expression : object.value(QStringLiteral("enumExpressions")).toArray())
            property.enumExpressions.append(expression.toString());
        if (!decodeValue(property, object.value(QStringLiteral("value")), property.value))
            return false;
        if (object.value(QStringLiteral("hasDefault")).toBool()
            && !decodeValue(property, object.value(QStringLiteral("default")), property.defaultValue))
            return false;
        for (const QJsonValue& entry : object.value(QStringLiteral("enabledWhen")).toArray()) {
            const QJsonObject condition = entry.toObject();
            property.enabledWhen.append({condition.value(QStringLiteral("name")).toString(), condition.value(QStringLiteral("value")).toVariant()});
        }
        decoded.append(property);
    }
    properties = decoded;
    return true;
}

bool StylePreviewProtocol::decodeValue(const StyleProperty& property, const QJsonValue& data, QVariant& value) {
    if (property.type == StylePropertyType::Size) {
        const QJsonObject size = data.toObject();
        if (!size.value(QStringLiteral("width")).isDouble() || !size.value(QStringLiteral("height")).isDouble())
            return false;
        value = QSize(size.value(QStringLiteral("width")).toInt(), size.value(QStringLiteral("height")).toInt());
        return true;
    }
    return StyleDocumentCodec::jsonToVariant(property, data, value);
}
