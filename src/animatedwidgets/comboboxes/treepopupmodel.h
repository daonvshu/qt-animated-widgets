#pragma once

#include "animatedwidgets/comboboxes/combopopupmodel.h"

namespace animatedwidgets {

class TreePopupModel : public ComboPopupModel {
    Q_OBJECT

public:
    explicit TreePopupModel(QObject* parent = nullptr);

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

    bool leafOnly() const;
    void setLeafOnly(bool enabled);

private:
    bool m_leafOnly = true;
};

} // namespace animatedwidgets
