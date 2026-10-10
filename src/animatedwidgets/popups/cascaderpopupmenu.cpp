#include "animatedwidgets/popups/cascaderpopupmenu.h"

#include "animatedwidgets/comboboxes/treepopupmodel.h"
#include "animatedwidgets/popups/cascadercolumnview.h"
#include "animatedwidgets/popups/cascaderitemdelegate.h"
#include "animatedwidgets/popups/popupmenuliststyle.h"
#include "animatedwidgets/popups/popupmenustyle.h"
#include "animatedwidgets/utils/treemodelutils.h"

#include <QKeyEvent>
#include <QLayout>
#include <QListView>
#include <QScrollBar>

namespace animatedwidgets {

CascaderPopupMenu::CascaderPopupMenu(QWidget* parent)
    : ComboListPopup(new TreePopupModel, parent)
{
    m_columns = new CascaderColumnView(this);
    setView(m_columns);
    setItemDelegate(new CascaderItemDelegate(this));
    setAutoClose(false);
    disconnect(m_columns, &QAbstractItemView::clicked, this, nullptr);

    connect(m_columns, &CascaderColumnView::columnCreated, this, [this](QAbstractItemView* column) {
        configureColumn(column);
    });
    connect(m_columns, &CascaderColumnView::columnsChanged, this, &CascaderPopupMenu::updateColumnLayout);
    connect(m_columns, &CascaderColumnView::columnTriggered, this, [this](const QModelIndex& index) {
        if (m_closing || !index.flags().testFlag(Qt::ItemIsEnabled))
            return;

        const bool repeated = m_lastTriggeredIndex == index;
        m_lastTriggeredIndex = index;
        if (!TreeModelUtils::isLeaf(index) && !repeated)
            return;

        if (index.flags().testFlag(Qt::ItemIsSelectable))
            emit itemTriggered(index);
    });
    connect(m_columns, &CascaderColumnView::dismissRequested, this, &QWidget::close);
    connect(this, &BasePopupDlg::closeRequested, this, [this] { m_closing = true; });
}

CascaderPopupMenu::~CascaderPopupMenu() {
    // Child views can reset while QWidget destroys their source models.
    disconnect(m_columns, nullptr, this, nullptr);
}

CascaderColumnView* CascaderPopupMenu::columnView() const {
    return m_columns;
}

void CascaderPopupMenu::setLeafOnly(bool enabled) {
    static_cast<TreePopupModel*>(selectionModel())->setLeafOnly(enabled);
}

void CascaderPopupMenu::setColumnWidth(int width) {
    m_columnWidth = qMax(80, width);
    m_columns->setColumnWidth(m_columnWidth);
    updateColumnLayout();
}

void CascaderPopupMenu::configureColumn(QAbstractItemView* column) {
    column->setItemDelegate(itemDelegate());
    listStyle()->apply(column);
    column->verticalScrollBar()->setStyleSheet(scrollBarStyleSheet());
    column->horizontalScrollBar()->setStyleSheet(scrollBarStyleSheet());
}

void CascaderPopupMenu::setItems(QAbstractItemModel* model, const QModelIndex& root, int column, int currentRow, int visibleRows, int width) {
    m_popupWidth = width;
    m_maximumVisibleItems = visibleRows;
    ComboListPopup::setItems(model, root, column, currentRow, visibleRows, width);
}

void CascaderPopupMenu::updateColumnLayout() {
    if (m_lastTriggeredIndex != m_columns->currentIndex())
        m_lastTriggeredIndex = QModelIndex();
    setAdaptiveHeight(m_popupWidth, m_maximumVisibleItems);
}

void CascaderPopupMenu::setAdaptiveHeight(int width, int maximumVisibleItems) {
    if (m_closing || m_updatingLayout)
        return;
    m_updatingLayout = true;
    m_popupWidth = width;
    m_maximumVisibleItems = maximumVisibleItems;
    const QSize previousSize = size();

    ensureShadowSizeExpanded();
    ensurePolished();
    setContentFixedWidth(qMax(1, width));
    m_columns->setColumnWidth(m_columnWidth);
    if (auto* delegate = qobject_cast<CascaderItemDelegate*>(itemDelegate()))
        delegate->setPathEndIndex(m_columns->currentIndex());
    listStyle()->apply(m_columns);
    for (auto* column : m_columns->columns())
        configureColumn(column);

    int height = 1;
    for (const QModelIndex& root : m_columns->columnRoots()) {
        if (!model())
            break;
        int columnHeight = 0;
        const int rows = model()->rowCount(root);
        const int count = maximumVisibleItems > 0 ? qMin(rows, maximumVisibleItems) : rows;
        for (int row = 0; row < count; ++row)
            columnHeight += qMax(0, m_columns->sizeHintForIndex(model()->index(row, 0, root)).height());
        height = qMax(height, columnHeight);
    }

    if (layout())
        layout()->activate();
    const QMargins margins = layout()->contentsMargins() + popupStyle()->margins() + m_columns->contentsMargins();
    if (m_columns->contentWidth() > m_columns->viewport()->width())
        height += m_columns->horizontalScrollBar()->sizeHint().height();
    setContentFixedHeight(height + margins.top() + margins.bottom());
    if (layout())
        layout()->activate();
    m_updatingLayout = false;

    // Resizing an upward popup must keep its bottom edge attached to the owner.
    if (isVisible() && size() != previousSize)
        updatePopupPlacement();
}

void CascaderPopupMenu::showEvent(QShowEvent* event) {
    const QModelIndex current = m_columns->currentIndex();
    m_lastTriggeredIndex = current.data(Qt::CheckStateRole).toInt() == Qt::Checked ? current : QModelIndex();
    updateColumnLayout();
    ComboListPopup::showEvent(event);
}

bool CascaderPopupMenu::eventFilter(QObject* watched, QEvent* event) {
    if (watched == m_columns && event->type() == QEvent::KeyPress) {
        const int key = static_cast<QKeyEvent*>(event)->key();
        if (key == Qt::Key_Right || key == Qt::Key_Left)
            return false;
    }
    return ComboListPopup::eventFilter(watched, event);
}

} // namespace animatedwidgets
