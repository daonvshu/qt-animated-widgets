#include "styleicontreemodel.h"

#include <QStringList>

StyleIconTreeModel::StyleIconTreeModel(QObject* parent)
    : QStandardItemModel(parent)
{
}

void StyleIconTreeModel::rebuild(const QList<StyleResourceEntry>& entries, const QString& filter) {
    m_references.clear();
    clear();
    const QString query = filter.trimmed();
    QHash<QString, QStandardItem*> categories;
    const auto categoryItem = [&categories](QStandardItem* parent, const QString& key, const QString& name) {
        auto* item = categories.value(key);
        if (!item) {
            item = new QStandardItem(name);
            item->setFlags(Qt::ItemIsEnabled);
            parent->appendRow(item);
            categories.insert(key, item);
        }
        return item;
    };

    for (const StyleResourceEntry& entry : entries) {
        if (entry.reference.isEmpty() || m_references.contains(entry.reference)
            || (!query.isEmpty() && !entry.reference.contains(query, Qt::CaseInsensitive)))
            continue;

        auto* parent = invisibleRootItem();
        QString path = entry.reference;
        QString categoryKey = QStringLiteral("file:");
        if (!entry.qrcPrefix.isEmpty()) {
            categoryKey = QStringLiteral("qrc:") + entry.qrcPrefix + QLatin1Char(':');
            parent = categoryItem(parent, categoryKey, entry.qrcPrefix);
            path.remove(0, 2);
            const QString prefix = entry.qrcPrefix.mid(1);
            if (!prefix.isEmpty() && path.startsWith(prefix + QLatin1Char('/')))
                path.remove(0, prefix.size() + 1);
        }

        QStringList parts = path.split(QLatin1Char('/'));
        parts.removeAll(QString());
        if (parts.isEmpty())
            continue;
        const QString name = parts.takeLast();
        for (const QString& part : parts) {
            categoryKey += QLatin1Char('/') + part;
            parent = categoryItem(parent, categoryKey, part);
        }

        auto* item = new QStandardItem(name);
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        item->setData(entry.reference, ReferenceRole);
        item->setData(entry.filePath, FilePathRole);
        item->setToolTip(entry.reference);
        parent->appendRow(item);
        m_references.insert(entry.reference, QPersistentModelIndex(item->index()));
    }
}

QModelIndex StyleIconTreeModel::indexForReference(const QString& reference) const {
    return m_references.value(reference);
}

int StyleIconTreeModel::iconCount() const {
    return static_cast<int>(m_references.size());
}
