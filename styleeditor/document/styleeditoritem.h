#pragma once

#include "styledocument.h"

#include "animatedwidgets/styleeditor/api/styleproperty.h"

struct StyleEditorItem : StyleDocumentItem {
    QString typeName;
    QList<animatedwidgets::styleeditor::StyleProperty> properties;
    QList<animatedwidgets::styleeditor::StyleProperty> inheritedProperties;
    QSize suggestedSize;
    QString generatedCode;
};
