#include "stylewidgettreemodel.h"

#include "../utils/stylewidgettypeutils.h"

#include <QHash>

using animatedwidgets::styleeditor::StyleEditorWidgetType;

StyleWidgetTreeModel::StyleWidgetTreeModel(QObject* parent)
    : QStandardItemModel(parent)
{
}

QString StyleWidgetTreeModel::categoryName(const StyleEditorWidgetType& type) {
    return StyleWidgetTypeUtils::categoryName(type);
}

void StyleWidgetTreeModel::rebuild(const QList<StyleEditorWidgetType>& types, const QList<StyleDocumentItem>& items, const QString& filter) {
    clear();
    QHash<QString, const StyleEditorWidgetType*> typeById;
    QHash<QString, QStandardItem*> categoryItems;
    for (const StyleEditorWidgetType& type : types) {
        typeById.insert(type.id, &type);
        const QString category = categoryName(type);
        if (categoryItems.contains(category))
            continue;

        auto* group = new QStandardItem(category);
        group->setFlags(Qt::ItemIsEnabled);
        group->setData(category, CategoryRole);
        group->setData(true, ActionEnabledRole);
        appendRow(group);
        categoryItems.insert(category, group);
    }

    const QString query = filter.trimmed();
    for (int index = 0; index < items.size(); ++index) {
        const StyleDocumentItem& item = items.at(index);
        const StyleEditorWidgetType* type = typeById.value(item.widgetTypeId, nullptr);
        const QString category = type ? categoryName(*type) : QStringLiteral("Unavailable widgets");
        const QString typeName = type ? type->name : item.widgetTypeId;
        if (!query.isEmpty() && !category.contains(query, Qt::CaseInsensitive)
            && !item.displayName.contains(query, Qt::CaseInsensitive) && !typeName.contains(query, Qt::CaseInsensitive)
            && !item.widgetTypeId.contains(query, Qt::CaseInsensitive))
            continue;

        QStandardItem* group = categoryItems.value(category, nullptr);
        if (!group) {
            group = new QStandardItem(category);
            group->setFlags(Qt::ItemIsEnabled);
            group->setData(category, CategoryRole);
            group->setData(false, ActionEnabledRole);
            appendRow(group);
            categoryItems.insert(category, group);
        }

        auto* child = new QStandardItem(item.displayName);
        child->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        child->setData(category, CategoryRole);
        child->setData(item.id, ItemIdRole);
        child->setData(index, ItemIndexRole);
        child->setData(item.widgetTypeId, TypeIdRole);
        child->setData(typeName, DetailRole);
        child->setData(!types.isEmpty(), ActionEnabledRole);
        child->setToolTip(item.displayName + QStringLiteral("\n") + typeName);
        group->appendRow(child);
    }

    for (int row = rowCount() - 1; row >= 0; --row) {
        QStandardItem* group = item(row);
        if (!query.isEmpty() && group->rowCount() == 0 && !group->text().contains(query, Qt::CaseInsensitive)) {
            removeRow(row);
        } else {
            group->setData(QStringLiteral("%1 styles").arg(group->rowCount()), DetailRole);
        }
    }
}

QModelIndex StyleWidgetTreeModel::indexForItem(const QString& id) const {
    if (id.isEmpty())
        return QModelIndex();

    for (int group = 0; group < rowCount(); ++group) {
        const QModelIndex parent = index(group, 0);
        for (int row = 0; row < rowCount(parent); ++row) {
            const QModelIndex child = index(row, 0, parent);
            if (child.data(ItemIdRole).toString() == id)
                return child;
        }
    }
    return QModelIndex();
}
