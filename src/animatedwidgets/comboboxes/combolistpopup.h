#pragma once

#include "animatedwidgets/popups/popupmenu.h"

#include <QModelIndex>
#include <QPersistentModelIndex>

class QAbstractItemModel;

namespace animatedwidgets {

class ComboPopupModel;

class ComboListPopup final : public PopupMenu {
    Q_OBJECT

public:
    explicit ComboListPopup(QWidget* parent = nullptr);
    void setItems(QAbstractItemModel* model, const QModelIndex& root, int column, int currentRow, int visibleRows, int width);
    void setCurrentRow(int row);

signals:
    void itemChosen(int row);

protected:
    void showEvent(QShowEvent* event) override;

private:
    ComboPopupModel* m_popupModel = nullptr;
    QPersistentModelIndex m_root;
    int m_column = 0;
};

} // namespace animatedwidgets
