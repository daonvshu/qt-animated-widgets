#include "animatedwidgets/utils/combocreationutils.h"

#include <QAbstractItemModel>

namespace animatedwidgets {

QString ComboCreationUtils::candidate(const QAbstractItemModel* model, const QModelIndex& root, int column, const QString& text,
                                      Qt::CaseSensitivity sensitivity, int minimumLength) {
    const QString value = text.trimmed();
    if (!model || (root.isValid() && root.model() != model) || column < 0
        || value.size() < qMax(1, minimumLength))
        return QString();

    // An empty model may not expose columns until its first option is inserted.
    if (model->rowCount(root) > 0 && column >= model->columnCount(root))
        return QString();
    for (int row = 0; row < model->rowCount(root); ++row) {
        const QModelIndex index = model->index(row, column, root);
        const QString description = index.data(Qt::AccessibleDescriptionRole).toString();
        if (description == QStringLiteral("separator") || description == QStringLiteral("groupTitle"))
            continue;
        if (QString::compare(index.data(Qt::DisplayRole).toString().trimmed(), value, sensitivity) == 0)
            return QString();
    }
    return value;
}

} // namespace animatedwidgets
