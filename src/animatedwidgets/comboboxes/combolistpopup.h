#pragma once

#include "animatedwidgets/popups/popupmenu.h"

#include <QModelIndex>
#include <QPersistentModelIndex>

class QAbstractItemModel;

namespace animatedwidgets {

class ComboPopupModel;
class PopupMenuFilterModel;

class ComboListPopup : public PopupMenu {
    Q_OBJECT

public:
    explicit ComboListPopup(QWidget* parent = nullptr);
    virtual void setItems(QAbstractItemModel* model, const QModelIndex& root, int column, int currentRow, int visibleRows, int width);
    void setCurrentRow(int row);
    void setCurrentModelIndex(const QModelIndex& index);
    PopupMenuFilterModel* enableFiltering();
    void setSearchText(const QString& text);
    virtual void moveSelection(int direction);
    virtual void triggerCurrentItem();
    QWidget* emptyWidget() const;
    void setEmptyWidget(QWidget* widget);
    void releaseEmptyWidget();

signals:
    void itemChosen(int row);
    void modelIndexChosen(const QModelIndex& index);

protected:
    ComboListPopup(ComboPopupModel* model, QWidget* parent);
    void showEvent(QShowEvent* event) override;
    bool includeListInAdaptiveHeight() const override;
    ComboPopupModel* selectionModel() const;
    QModelIndex mapToSource(const QModelIndex& index) const;
    QModelIndex mapFromSource(const QModelIndex& index) const;

private:
    ComboPopupModel* m_popupModel = nullptr;
    PopupMenuFilterModel* m_filterModel = nullptr;
    QPointer<QWidget> m_emptyWidget;
    QPersistentModelIndex m_root;
    int m_column = 0;
    int m_visibleRows = 0;
    int m_width = 0;
    bool m_heightUpdatePending = false;

    void updateFilteredLayout();
    void scheduleFilteredLayout();
};

} // namespace animatedwidgets
