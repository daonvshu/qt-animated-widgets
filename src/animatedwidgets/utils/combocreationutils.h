#pragma once

#include <QModelIndex>
#include <QString>

class QAbstractItemModel;

namespace animatedwidgets {

class ComboCreationUtils {
public:
    static QString candidate(const QAbstractItemModel* model, const QModelIndex& root, int column, const QString& text,
                             Qt::CaseSensitivity sensitivity = Qt::CaseInsensitive, int minimumLength = 1);
};

} // namespace animatedwidgets
