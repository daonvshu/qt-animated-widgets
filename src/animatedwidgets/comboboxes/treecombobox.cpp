#include "animatedwidgets/comboboxes/treecombobox.h"

#include "animatedwidgets/popups/treepopupmenu.h"

namespace animatedwidgets {

TreeComboBox::TreeComboBox(QWidget* parent)
    : HierarchicalComboBox(parent)
{
}

bool TreeComboBox::searchable() const {
    return m_searchable;
}

void TreeComboBox::setSearchable(bool enabled) {
    m_searchable = enabled;
    if (auto* popup = qobject_cast<TreePopupMenu*>(currentPopup()))
        popup->setSearchable(enabled);
}

ComboListPopup* TreeComboBox::createPopup() {
    return new TreePopupMenu(this);
}

void TreeComboBox::configurePopup(ComboListPopup* popup) {
    ComboBox::configurePopup(popup);
    auto* tree = qobject_cast<TreePopupMenu*>(popup);
    if (!tree)
        return;
    tree->setLeafOnly(leafOnly());
    tree->setSearchable(m_searchable);
    tree->setCurrentModelIndex(currentModelIndex());
}

} // namespace animatedwidgets
