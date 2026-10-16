#pragma once

#include "../document/styledocument.h"

#include "animatedwidgets/styleeditor/api/styleproperty.h"

class StyleDocumentCodec {
public:
    static bool readFile(const QString& filePath, StyleDocument& document, QString& errorMessage);
    static bool writeFile(const QString& filePath, const StyleDocument& document, QString& errorMessage);
    static QJsonObject toJson(const StyleDocument& document);
    static QJsonObject serializeProperties(const QList<animatedwidgets::styleeditor::StyleProperty>& properties);
    static void applySerializedProperties(QList<animatedwidgets::styleeditor::StyleProperty>& properties, const QJsonObject& values);
    static void applyCompatibleProperties(QList<animatedwidgets::styleeditor::StyleProperty>& properties,
                                          const QList<animatedwidgets::styleeditor::StyleProperty>& previous);
    static QJsonValue variantToJson(const animatedwidgets::styleeditor::StyleProperty& property, const QVariant& value);
    static bool jsonToVariant(const animatedwidgets::styleeditor::StyleProperty& property, const QJsonValue& value, QVariant& result);

private:
    static QJsonValue valueToJson(const animatedwidgets::styleeditor::StyleProperty& property);
};
