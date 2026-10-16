#pragma once

#include "../document/styleresourceentry.h"

#include <QHash>
#include <QPersistentModelIndex>
#include <QStandardItemModel>

class StyleIconTreeModel : public QStandardItemModel {
public:
    enum Role {
        ReferenceRole = Qt::UserRole + 1,
        FilePathRole
    };

    explicit StyleIconTreeModel(QObject* parent = nullptr);

    void rebuild(const QList<StyleResourceEntry>& entries, const QString& filter);
    QModelIndex indexForReference(const QString& reference) const;
    int iconCount() const;

private:
    QHash<QString, QPersistentModelIndex> m_references;
};
