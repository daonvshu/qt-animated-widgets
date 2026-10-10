#pragma once

#include "animatedwidgets/comboboxes/combolistpopup.h"

#include <QPersistentModelIndex>

namespace animatedwidgets {

class CascaderColumnView;

class CascaderPopupMenu : public ComboListPopup {
    Q_OBJECT

public:
    explicit CascaderPopupMenu(QWidget* parent = nullptr);
    ~CascaderPopupMenu() override;

    CascaderColumnView* columnView() const;
    void setLeafOnly(bool enabled);
    void setColumnWidth(int width);
    void setItems(QAbstractItemModel* model, const QModelIndex& root, int column, int currentRow, int visibleRows, int width) override;
    void setAdaptiveHeight(int width, int maximumVisibleItems = 0) override;

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
    void showEvent(QShowEvent* event) override;

private:
    void configureColumn(QAbstractItemView* column);
    void updateColumnLayout();

    CascaderColumnView* m_columns = nullptr;
    QPersistentModelIndex m_lastTriggeredIndex;

    int m_columnWidth = 180;
    int m_popupWidth = 560;
    int m_maximumVisibleItems = 8;
    bool m_closing = false;
    bool m_updatingLayout = false;
};

} // namespace animatedwidgets
