#include "animatedwidgets/comboboxes/hierarchicalcombobox.h"

#include "animatedwidgets/comboboxes/combolistpopup.h"
#include "animatedwidgets/utils/treemodelutils.h"

namespace animatedwidgets {

HierarchicalComboBox::HierarchicalComboBox(QWidget* parent)
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

void HierarchicalComboBox::setModel(QAbstractItemModel* model) {
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

QModelIndex HierarchicalComboBox::treeRootIndex() const {
    return m_treeRoot;
}

void HierarchicalComboBox::setTreeRootIndex(const QModelIndex& index) {
    if ((index.isValid() && (index.model() != model() || index.column() != 0)) || m_treeRoot == index)
        return;
    hidePopup();
    m_treeRoot = index;
    if (!TreeModelUtils::belongsToRoot(currentModelIndex(), index))
        setCurrentModelIndex(QModelIndex());
}

QModelIndex HierarchicalComboBox::currentModelIndex() const {
    return model()->index(currentIndex(), modelColumn(), rootModelIndex());
}

void HierarchicalComboBox::setCurrentModelIndex(const QModelIndex& index) {
    if (index.isValid() && (index.model() != model() || index.column() != modelColumn()
        || !TreeModelUtils::belongsToRoot(index, m_treeRoot) || !TreeModelUtils::isSelectable(index, m_leafOnly)))
        return;
    m_changingSelection = true;
    // The native combo stores a row relative to its root; preserve the full tree root separately.
    setRootModelIndex(index.isValid() ? index.parent() : QModelIndex(m_treeRoot));
    setCurrentIndex(index.isValid() ? index.row() : -1);
    m_changingSelection = false;
    QPointer<HierarchicalComboBox> guard(this);
    notifySelection();
    if (!guard)
        return;
    if (auto* popup = currentPopup())
        popup->setCurrentModelIndex(index);
}

QList<QModelIndex> HierarchicalComboBox::currentPath() const {
    return TreeModelUtils::path(currentModelIndex(), m_treeRoot);
}

bool HierarchicalComboBox::leafOnly() const {
    return m_leafOnly;
}

void HierarchicalComboBox::setLeafOnly(bool enabled) {
    if (m_leafOnly == enabled)
        return;
    hidePopup();
    m_leafOnly = enabled;
    if (currentModelIndex().isValid() && !TreeModelUtils::isSelectable(currentModelIndex(), enabled))
        setCurrentModelIndex(QModelIndex());
}

QModelIndex HierarchicalComboBox::popupRootModelIndex() const {
    return m_treeRoot;
}

void HierarchicalComboBox::choosePopupIndex(const QModelIndex& index) {
    QPointer<HierarchicalComboBox> guard(this);
    setCurrentModelIndex(index);
    if (guard && currentModelIndex() == index)
        emit modelIndexActivated(currentModelIndex());
}

void HierarchicalComboBox::notifySelection() {
    const QModelIndex index = currentModelIndex();
    if (m_lastSelection == index && m_hadSelection == index.isValid())
        return;
    m_lastSelection = index;
    m_hadSelection = index.isValid();
    emit currentModelIndexChanged(index);
}

} // namespace animatedwidgets
