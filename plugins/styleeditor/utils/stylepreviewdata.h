#pragma once

#include <QStandardItemModel>
#include <QStringList>

class StylePreviewData {
public:
    static QStringList choices();
    static QStandardItemModel* tree(QObject* parent);
};
