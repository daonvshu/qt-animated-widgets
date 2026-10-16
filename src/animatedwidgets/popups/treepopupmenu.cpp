#include "animatedwidgets/popups/treepopupmenu.h"

#include "animatedwidgets/comboboxes/treepopupmodel.h"
#include "animatedwidgets/inputs/iconlineedit.h"
#include "animatedwidgets/utils/treemodelutils.h"

#include <QHeaderView>
#include <QKeyEvent>
#include <QTreeView>

namespace animatedwidgets {

TreePopupMenu::TreePopupMenu(QWidget* parent)
    : ComboListPopup(new TreePopupModel, parent)
{
    m_treeView = new QTreeView(this);
    m_treeView->setHeaderHidden(true);
    m_treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_treeView->setExpandsOnDoubleClick(false);
    m_treeView->setIndentation(20);
    setView(m_treeView);

    m_searchEditor = new IconLineEdit(this);
    m_searchEditor->setStyleSheet(QStringLiteral("QLineEdit { background: transparent; border: none; }"));
    m_searchEditor->setLeadingIcon(QIcon(QStringLiteral(":/animatedwidgets/inputs/search.svg")));
    m_searchEditor->setPlaceholderText(tr("Search items"));
    m_searchEditor->installEventFilter(this);
    setTopWidget(m_searchEditor);
    connect(m_searchEditor, &QLineEdit::textChanged, this, [this](const QString& text) {
        setSearchText(text);
        if (!text.trimmed().isEmpty())
            m_treeView->expandAll();
        updateTreeLayout();
    });

    connect(m_treeView, &QTreeView::expanded, this, &TreePopupMenu::updateTreeLayout);
    connect(m_treeView, &QTreeView::collapsed, this, &TreePopupMenu::updateTreeLayout);
    connect(m_treeView, &QTreeView::clicked, this, [this](const QModelIndex& index) {
        if (leafOnly() && index.flags().testFlag(Qt::ItemIsEnabled)
            && !TreeModelUtils::isLeaf(mapToSource(index)))
            m_treeView->setExpanded(index, !m_treeView->isExpanded(index));
    });
}

QTreeView* TreePopupMenu::treeView() const {
    return m_treeView;
}

IconLineEdit* TreePopupMenu::searchEditor() const {
    return m_searchEditor;
}

void TreePopupMenu::setIconHint(bool enabled) {
    m_searchEditor->setIconHint(enabled);
    PopupMenu::setIconHint(enabled);
}

bool TreePopupMenu::leafOnly() const {
    return static_cast<TreePopupModel*>(selectionModel())->leafOnly();
}

void TreePopupMenu::setLeafOnly(bool enabled) {
    if (leafOnly() == enabled)
        return;
    const QPersistentModelIndex root = mapToSource(m_treeView->rootIndex());
    const QPersistentModelIndex current = mapToSource(m_treeView->currentIndex());
    static_cast<TreePopupModel*>(selectionModel())->setLeafOnly(enabled);
    m_treeView->setRootIndex(mapFromSource(root));
    setCurrentModelIndex(current);
    m_treeView->expandToDepth(0);
    updateTreeLayout();
}

bool TreePopupMenu::searchable() const {
    return m_searchable;
}

void TreePopupMenu::setSearchable(bool enabled) {
    m_searchable = enabled;
    m_searchEditor->setVisible(enabled);
    if (!enabled)
        m_searchEditor->clear();
    updateTreeLayout();
}

void TreePopupMenu::setItems(QAbstractItemModel* model, const QModelIndex& root, int column, int currentRow, int visibleRows, int width) {
    ComboListPopup::setItems(model, root, column, currentRow, visibleRows, width);
    for (int section = 0; section < this->model()->columnCount(m_treeView->rootIndex()); ++section)
        m_treeView->setColumnHidden(section, section != column);
    m_treeView->setTreePosition(column);
    m_treeView->header()->setSectionResizeMode(column, QHeaderView::Stretch);
    enableFiltering();
    m_treeView->expandToDepth(0);
    updateTreeLayout();
}

void TreePopupMenu::setAdaptiveHeight(int width, int maximumVisibleItems) {
    m_popupWidth = qMax(80, width);
    m_maximumVisibleItems = maximumVisibleItems;
    PopupMenu::setAdaptiveHeight(m_popupWidth, maximumVisibleItems);
}

void TreePopupMenu::updateTreeLayout() {
    setAdaptiveHeight(m_popupWidth, m_maximumVisibleItems);
}

void TreePopupMenu::moveSelection(int direction) {
    if (direction != -1 && direction != 1)
        return;
    QModelIndex index = m_treeView->currentIndex();
    if (!index.isValid())
        index = model()->index(0, m_treeView->treePosition(), m_treeView->rootIndex());
    else
        index = direction > 0 ? m_treeView->indexBelow(index) : m_treeView->indexAbove(index);
    while (index.isValid()) {
        if (index.flags().testFlag(Qt::ItemIsEnabled)) {
            m_treeView->setCurrentIndex(index);
            m_treeView->scrollTo(index);
            return;
        }
        index = direction > 0 ? m_treeView->indexBelow(index) : m_treeView->indexAbove(index);
    }
}

void TreePopupMenu::triggerCurrentItem() {
    const QModelIndex index = m_treeView->currentIndex();
    if (!index.isValid() || !index.flags().testFlag(Qt::ItemIsEnabled))
        return;
    if (leafOnly() && !TreeModelUtils::isLeaf(mapToSource(index))) {
        m_treeView->setExpanded(index, !m_treeView->isExpanded(index));
        return;
    }
    ComboListPopup::triggerCurrentItem();
}

bool TreePopupMenu::eventFilter(QObject* watched, QEvent* event) {
    if ((watched == m_treeView || watched == m_searchEditor) && event->type() == QEvent::KeyPress) {
        const int key = static_cast<QKeyEvent*>(event)->key();
        if (key == Qt::Key_Return || key == Qt::Key_Enter || (watched == m_treeView && key == Qt::Key_Space)) {
            triggerCurrentItem();
            return true;
        }
        if (watched == m_searchEditor && (key == Qt::Key_Up || key == Qt::Key_Down)) {
            moveSelection(key == Qt::Key_Down ? 1 : -1);
            return true;
        }
        if (watched == m_searchEditor && key == Qt::Key_Escape) {
            close();
            return true;
        }
    }
    return ComboListPopup::eventFilter(watched, event);
}

void TreePopupMenu::showEvent(QShowEvent* event) {
    ComboListPopup::showEvent(event);
    for (QModelIndex parent = m_treeView->currentIndex().parent(); parent.isValid(); parent = parent.parent())
        m_treeView->expand(parent);
    updateTreeLayout();
    if (m_treeView->currentIndex().isValid())
        m_treeView->scrollTo(m_treeView->currentIndex(), QAbstractItemView::EnsureVisible);
    if (m_searchable && !windowFlags().testFlag(Qt::WindowDoesNotAcceptFocus))
        m_searchEditor->setFocus();
}

} // namespace animatedwidgets
