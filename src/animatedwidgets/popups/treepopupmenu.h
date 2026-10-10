#pragma once

#include "animatedwidgets/comboboxes/combolistpopup.h"

class QTreeView;

namespace animatedwidgets {

class IconLineEdit;

class TreePopupMenu : public ComboListPopup {
    Q_OBJECT

public:
    explicit TreePopupMenu(QWidget* parent = nullptr);

    QTreeView* treeView() const;
    IconLineEdit* searchEditor() const;

    bool leafOnly() const;
    void setLeafOnly(bool enabled);
    bool searchable() const;
    void setSearchable(bool enabled);

    void setItems(QAbstractItemModel* model, const QModelIndex& root, int column, int currentRow, int visibleRows, int width) override;
    void setAdaptiveHeight(int width, int maximumVisibleItems = 0) override;
    void moveSelection(int direction) override;
    void triggerCurrentItem() override;

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
    void showEvent(QShowEvent* event) override;

private:
    void updateTreeLayout();

    QTreeView* m_treeView = nullptr;
    IconLineEdit* m_searchEditor = nullptr;
    int m_popupWidth = 240;
    int m_maximumVisibleItems = 10;
    bool m_searchable = true;
};

} // namespace animatedwidgets
