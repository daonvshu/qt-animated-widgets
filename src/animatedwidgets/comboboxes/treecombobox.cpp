#include "animatedwidgets/comboboxes/treecombobox.h"

#include "animatedwidgets/popups/treepopupmenu.h"
#include "animatedwidgets/utils/treemodelutils.h"

namespace animatedwidgets {

TreeComboBox::TreeComboBox(QWidget* parent)
    : ComboBox(parent)
{
    connect(this, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this] {
        if (m_changingSelection)
            return;
        const QModelIndex index = currentModelIndex();
        if (index.isValid() && !TreeModelUtils::isSelectable(index, m_leafOnly)) {
            setCurrentIndex(-1);
            return;
        }
        notifySelection();
    });
}

void TreeComboBox::setModel(QAbstractItemModel* model) {
    if (!model || model == this->model())
        return;
    hidePopup();
    m_treeRoot = QModelIndex();
    m_changingSelection = true;
    QComboBox::setModel(model);
    setRootModelIndex(QModelIndex());
    setCurrentIndex(-1);
    m_changingSelection = false;
    notifySelection();
}

QModelIndex TreeComboBox::treeRootIndex() const {
    return m_treeRoot;
}

void TreeComboBox::setTreeRootIndex(const QModelIndex& index) {
    if ((index.isValid() && (index.model() != model() || index.column() != 0)) || m_treeRoot == index)
        return;
    hidePopup();
    m_treeRoot = index;
    if (!TreeModelUtils::belongsToRoot(currentModelIndex(), index))
        setCurrentModelIndex(QModelIndex());
}

QModelIndex TreeComboBox::currentModelIndex() const {
    return model()->index(currentIndex(), modelColumn(), rootModelIndex());
}

void TreeComboBox::setCurrentModelIndex(const QModelIndex& index) {
    if (index.isValid() && (index.model() != model() || index.column() != modelColumn()
        || !TreeModelUtils::belongsToRoot(index, m_treeRoot) || !TreeModelUtils::isSelectable(index, m_leafOnly)))
        return;
    m_changingSelection = true;
    // The native combo stores a row relative to its root; preserve the full tree root separately.
    setRootModelIndex(index.isValid() ? index.parent() : QModelIndex(m_treeRoot));
    setCurrentIndex(index.isValid() ? index.row() : -1);
    m_changingSelection = false;
    QPointer<TreeComboBox> guard(this);
    notifySelection();
    if (!guard)
        return;
    if (auto* popup = currentPopup())
        popup->setCurrentModelIndex(index);
}

bool TreeComboBox::leafOnly() const {
    return m_leafOnly;
}

void TreeComboBox::setLeafOnly(bool enabled) {
    if (m_leafOnly == enabled)
        return;
    hidePopup();
    m_leafOnly = enabled;
    if (currentModelIndex().isValid() && !TreeModelUtils::isSelectable(currentModelIndex(), enabled))
        setCurrentModelIndex(QModelIndex());
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

QModelIndex TreeComboBox::popupRootModelIndex() const {
    return m_treeRoot;
}

void TreeComboBox::choosePopupIndex(const QModelIndex& index) {
    QPointer<TreeComboBox> guard(this);
    setCurrentModelIndex(index);
    if (guard && currentModelIndex() == index)
        emit modelIndexActivated(currentModelIndex());
}

void TreeComboBox::configurePopup(ComboListPopup* popup) {
    ComboBox::configurePopup(popup);
    auto* tree = qobject_cast<TreePopupMenu*>(popup);
    if (!tree)
        return;
    tree->setLeafOnly(m_leafOnly);
    tree->setSearchable(m_searchable);
    tree->setCurrentModelIndex(currentModelIndex());
}

void TreeComboBox::notifySelection() {
    const QModelIndex index = currentModelIndex();
    if (m_lastSelection == index && m_hadSelection == index.isValid())
        return;
    m_lastSelection = index;
    m_hadSelection = index.isValid();
    emit currentModelIndexChanged(index);
}

} // namespace animatedwidgets
