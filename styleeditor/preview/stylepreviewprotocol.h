#pragma once

#include "animatedwidgets/styleeditor/api/styleproperty.h"

#include <QByteArray>
#include <QJsonArray>
#include <QJsonObject>

class StylePreviewProtocol {
public:
    static constexpr quint32 MaximumFrameSize = 16 * 1024 * 1024;
    static constexpr int Version = 1;

    static QByteArray encode(QJsonObject message);
    static bool takeMessages(QByteArray& buffer, QList<QJsonObject>& messages, QString& error);

    static QJsonArray encodeProperties(const QList<animatedwidgets::styleeditor::StyleProperty>& properties);
    static bool decodeProperties(const QJsonArray& data, QList<animatedwidgets::styleeditor::StyleProperty>& properties);

private:
    static bool decodeValue(const animatedwidgets::styleeditor::StyleProperty& property, const QJsonValue& data, QVariant& value);
};
