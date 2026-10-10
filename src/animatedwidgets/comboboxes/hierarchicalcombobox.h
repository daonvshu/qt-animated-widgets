#pragma once

#include "animatedwidgets/comboboxes/combobox.h"

#include <QPersistentModelIndex>

namespace animatedwidgets {

class HierarchicalComboBox : public ComboBox {
    Q_OBJECT
    Q_PROPERTY(bool leafOnly READ leafOnly WRITE setLeafOnly)

public:
    explicit HierarchicalComboBox(QWidget* parent = nullptr);

    void setModel(QAbstractItemModel* model);
    QModelIndex treeRootIndex() const;
    void setTreeRootIndex(const QModelIndex& index);
    QModelIndex currentModelIndex() const;
    void setCurrentModelIndex(const QModelIndex& index);
    QList<QModelIndex> currentPath() const;

    bool leafOnly() const;
    void setLeafOnly(bool enabled);

signals:
    void currentModelIndexChanged(const QModelIndex& index);
    void modelIndexActivated(const QModelIndex& index);

protected:
    QModelIndex popupRootModelIndex() const override;
    void choosePopupIndex(const QModelIndex& index) override;

private:
    void notifySelection();

    QPersistentModelIndex m_treeRoot;
    QPersistentModelIndex m_lastSelection;
    bool m_leafOnly = true;
    bool m_changingSelection = false;
    bool m_hadSelection = false;
};

} // namespace animatedwidgets
