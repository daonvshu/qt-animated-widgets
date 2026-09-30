#pragma once

#include "animatedwidgets/popups/basepopupdlg.h"

#include <QModelIndex>

class QAbstractItemModel;
class QListView;

namespace animatedwidgets {

class ComboListPopup final : public BasePopupDlg {
    Q_OBJECT

public:
    explicit ComboListPopup(QWidget* parent = nullptr);
    void setItems(QAbstractItemModel* model, const QModelIndex& root, int column, int currentRow, int visibleRows, int width);

signals:
    void itemChosen(int row);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    QListView* m_list = nullptr;
};

} // namespace animatedwidgets
