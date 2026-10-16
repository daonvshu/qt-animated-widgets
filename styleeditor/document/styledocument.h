#pragma once

#include "stylepreviewsettings.h"
#include "styleresourcesettings.h"

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
    StyleResourceSettings resources;
    bool modified = false;
};
