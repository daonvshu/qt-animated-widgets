#pragma once

#include "../document/styledocument.h"

#include "animatedwidgets/styleeditor/api/styleeditorplugin.h"

#include <QStandardItemModel>

class StyleWidgetTreeModel : public QStandardItemModel {
public:
    enum Role {
        CategoryRole = Qt::UserRole + 1,
        ItemIdRole,
        ItemIndexRole,
        TypeIdRole,
        DetailRole,
        ActionEnabledRole
    };

    explicit StyleWidgetTreeModel(QObject* parent = nullptr);

    void rebuild(const QList<animatedwidgets::styleeditor::StyleEditorWidgetType>& types,
                 const QList<StyleDocumentItem>& items, const QString& filter);
    QModelIndex indexForItem(const QString& id) const;

    static QString categoryName(const animatedwidgets::styleeditor::StyleEditorWidgetType& type);
};
