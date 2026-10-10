#pragma once

#include "animatedwidgets/comboboxes/combobox.h"

#include <QPersistentModelIndex>

namespace animatedwidgets {

class TreeComboBox : public ComboBox {
    Q_OBJECT
    Q_PROPERTY(bool leafOnly READ leafOnly WRITE setLeafOnly)
    Q_PROPERTY(bool searchable READ searchable WRITE setSearchable)

public:
    explicit TreeComboBox(QWidget* parent = nullptr);

    void setModel(QAbstractItemModel* model);

    QModelIndex treeRootIndex() const;
    void setTreeRootIndex(const QModelIndex& index);
    QModelIndex currentModelIndex() const;
    void setCurrentModelIndex(const QModelIndex& index);

    bool leafOnly() const;
    void setLeafOnly(bool enabled);
    bool searchable() const;
    void setSearchable(bool enabled);

signals:
    void currentModelIndexChanged(const QModelIndex& index);
    void modelIndexActivated(const QModelIndex& index);

protected:
    ComboListPopup* createPopup() override;
    QModelIndex popupRootModelIndex() const override;
    void choosePopupIndex(const QModelIndex& index) override;
    void configurePopup(ComboListPopup* popup) override;

private:
    void notifySelection();

    QPersistentModelIndex m_treeRoot;
    QPersistentModelIndex m_lastSelection;
    bool m_leafOnly = true;
    bool m_searchable = true;
    bool m_changingSelection = false;
    bool m_hadSelection = false;
};

} // namespace animatedwidgets

#ifndef ANIMATEDWIDGETS_DESIGNER_PLUGIN
using TreeComboBox = animatedwidgets::TreeComboBox;
#endif
