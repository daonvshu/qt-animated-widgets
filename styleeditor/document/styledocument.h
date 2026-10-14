#pragma once

#include "stylepreviewsettings.h"

#include <QJsonObject>
#include <QList>
#include <QString>

struct StyleDocumentItem {
    QString id;
    QString displayName;
    QString widgetTypeId;
    QJsonObject serializedProperties;
    StylePreviewSettings previewSettings;
};

struct StyleDocument {
    QString filePath;
    QString name = QStringLiteral("Untitled");
    QList<StyleDocumentItem> items;
    bool modified = false;
};
