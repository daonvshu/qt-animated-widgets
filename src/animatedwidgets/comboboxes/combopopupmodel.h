#pragma once

#include <QIdentityProxyModel>
#include <QPersistentModelIndex>

namespace animatedwidgets {

class ComboPopupModel : public QIdentityProxyModel {
    Q_OBJECT

public:
    explicit ComboPopupModel(QObject* parent = nullptr);

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    void setSourceModel(QAbstractItemModel* sourceModel) override;
    void setCurrentIndex(const QModelIndex& sourceIndex);

private:
    QPersistentModelIndex m_currentIndex;
};

} // namespace animatedwidgets
