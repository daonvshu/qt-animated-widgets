#pragma once

#include "styledocument.h"

#include "animatedwidgets/styleeditor/api/styleproperty.h"

#include <QPointer>

class QWidget;

struct StyleEditorItem : StyleDocumentItem {
    QString typeName;
    QList<animatedwidgets::styleeditor::StyleProperty> properties;
    QPointer<QWidget> preview;
    QSize suggestedSize;
};
