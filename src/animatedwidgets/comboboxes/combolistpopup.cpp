#include "animatedwidgets/comboboxes/combolistpopup.h"
#include "animatedwidgets/popups/popupmenurowtype.h"
#include "animatedwidgets/comboboxes/combopopupmodel.h"
#include "animatedwidgets/popups/defaultpopupmenuliststyle.h"
#include "animatedwidgets/popups/popupmenufiltermodel.h"

#include <QAbstractItemModel>
#include <QListView>
#include <QLayout>
#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>

namespace animatedwidgets {

ComboListPopup::ComboListPopup(QWidget* parent)
    : ComboListPopup(new ComboPopupModel, parent)
{
}

ComboListPopup::ComboListPopup(ComboPopupModel* model, QWidget* parent)
    : PopupMenu(parent)
{
    Q_INIT_RESOURCE(comboboxes);
    m_popupModel = model;
    m_popupModel->setParent(this);
    auto* style = new DefaultPopupMenuListStyle;
    style->checkedIcon = QIcon(QStringLiteral(":/animatedwidgets/comboboxes/checked.svg"));
    style->checkedIconAlignment = Qt::AlignRight;
    setListStyle(style);

    connect(this, &BasePopupDlg::closeRequested, this, [this] {
        // Preserve the last frame while the hide animation is running.
        m_closing = true;
        setUpdatesEnabled(false);
        layout()->setEnabled(false);
    });

    connect(this, &PopupMenu::itemTriggered, this, [this](const QModelIndex& index) {
        const Qt::ItemFlags flags = index.flags();
        if (!flags.testFlag(Qt::ItemIsEnabled) || !flags.testFlag(Qt::ItemIsSelectable))
            return;
        const QPersistentModelIndex source = mapToSource(index);
        if (source.isValid()) {
            QPointer<ComboListPopup> guard(this);
            emit itemChosen(source.row());
            if (guard && source.isValid())
                emit modelIndexChosen(source);
        }
    });
}

void ComboListPopup::setItems(QAbstractItemModel* model, const QModelIndex& root, int column, int currentRow, int visibleRows, int width) {
    m_root = root;
    m_column = column;
    m_visibleRows = qMax(1, visibleRows);
    m_width = qMax(80, width);
    m_popupModel->setSourceModel(model);
    setModel(m_popupModel);
    view()->setRootIndex(m_popupModel->mapFromSource(root));
    view()->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    if (auto* list = qobject_cast<QListView*>(view()))
        list->setModelColumn(column);

    setAdaptiveHeight(qMax(80, width), qMax(1, visibleRows));
    setCurrentRow(currentRow);
}

void ComboListPopup::setCurrentRow(int row) {
    auto* sourceModel = m_popupModel->sourceModel();
    const QModelIndex source = sourceModel ? sourceModel->index(row, m_column, m_root) : QModelIndex();
    setCurrentModelIndex(source);
}

void ComboListPopup::setCurrentModelIndex(const QModelIndex& source) {
    if (m_closing)
        return;

    m_popupModel->setCurrentIndex(source);
    const QModelIndex current = mapFromSource(source);
    view()->setCurrentIndex(current);
    if (isVisible() && current.isValid())
        view()->scrollTo(current, QAbstractItemView::EnsureVisible);
}

ComboPopupModel* ComboListPopup::selectionModel() const {
    return m_popupModel;
}

PopupMenuFilterModel* ComboListPopup::enableFiltering() {
    if (m_filterModel)
        return m_filterModel;

    m_filterModel = new PopupMenuFilterModel(this);
    m_filterModel->setRecursiveFilteringEnabled(true);
    m_filterModel->setFilterKeyColumn(m_column);
    m_filterModel->setSourceModel(m_popupModel);
    setModel(m_filterModel);
    view()->setRootIndex(mapFromSource(m_root));
    auto* label = new QLabel(tr("No results"), this);
    label->setAlignment(Qt::AlignCenter);
    label->setMinimumHeight(64);
    setEmptyWidget(label);

    connect(m_filterModel, &QAbstractItemModel::rowsInserted, this, &ComboListPopup::scheduleFilteredLayout);
    connect(m_filterModel, &QAbstractItemModel::rowsRemoved, this, &ComboListPopup::scheduleFilteredLayout);
    connect(m_filterModel, &QAbstractItemModel::modelReset, this, &ComboListPopup::scheduleFilteredLayout);
    connect(m_filterModel, &QAbstractItemModel::layoutChanged, this, &ComboListPopup::scheduleFilteredLayout);
    connect(m_filterModel, &QAbstractItemModel::dataChanged, this, &ComboListPopup::scheduleFilteredLayout);
    updateFilteredLayout();
    return m_filterModel;
}

void ComboListPopup::setSearchText(const QString& text) {
    if (m_closing || !m_filterModel)
        return;
    m_filterModel->setSearchText(text);
    updateFilteredLayout();
}

QModelIndex ComboListPopup::mapToSource(const QModelIndex& index) const {
    return m_popupModel->mapToSource(m_filterModel ? m_filterModel->mapToSource(index) : index);
}

QModelIndex ComboListPopup::mapFromSource(const QModelIndex& index) const {
    const QModelIndex comboIndex = m_popupModel->mapFromSource(index);
    return m_filterModel ? m_filterModel->mapFromSource(comboIndex) : comboIndex;
}

void ComboListPopup::moveSelection(int direction) {
    if (direction != -1 && direction != 1)
        return;
    if (view()->isHidden())
        return;
    const int count = model()->rowCount(view()->rootIndex());
    const QModelIndex current = view()->currentIndex();
    int row = current.isValid() ? current.row() + direction : direction > 0 ? 0 : count - 1;
    for (; row >= 0 && row < count; row += direction) {
        const QModelIndex index = model()->index(row, m_column, view()->rootIndex());
        if (index.flags().testFlag(Qt::ItemIsEnabled) && index.flags().testFlag(Qt::ItemIsSelectable)) {
            view()->setCurrentIndex(index);
            view()->scrollTo(index, QAbstractItemView::EnsureVisible);
            return;
        }
    }
}

void ComboListPopup::triggerCurrentItem() {
    const QModelIndex index = view()->currentIndex();
    if (index.isValid() && index.flags().testFlag(Qt::ItemIsEnabled)
        && index.flags().testFlag(Qt::ItemIsSelectable))
        emit itemTriggered(index);
}

QWidget* ComboListPopup::emptyWidget() const {
    return m_emptyWidget;
}

void ComboListPopup::setEmptyWidget(QWidget* widget) {
    if (!widget || widget == m_emptyWidget || widget == this || widget == view()
        || widget == topWidget() || widget == bottomWidget() || widget->isAncestorOf(this))
        return;
    if (m_emptyWidget) {
        layout()->removeWidget(m_emptyWidget);
        m_emptyWidget->hide();
        m_emptyWidget->deleteLater();
    }
    m_emptyWidget = widget;
    widget->setParent(this);
    auto* box = qobject_cast<QVBoxLayout*>(layout());
    box->insertWidget(box->indexOf(view()), widget, 1);
    updateFilteredLayout();
}

void ComboListPopup::releaseEmptyWidget() {
    if (!m_emptyWidget)
        return;
    layout()->removeWidget(m_emptyWidget);
    m_emptyWidget->hide();
    m_emptyWidget = nullptr;
}

bool ComboListPopup::includeListInAdaptiveHeight() const {
    return !m_filterModel || !view()->isHidden();
}

void ComboListPopup::scheduleFilteredLayout() {
    if (m_closing || m_heightUpdatePending)
        return;
    m_heightUpdatePending = true;
    QTimer::singleShot(0, this, [this] {
        m_heightUpdatePending = false;
        updateFilteredLayout();
    });
}

void ComboListPopup::updateFilteredLayout() {
    if (m_closing || !m_filterModel)
        return;

    const QModelIndex root = mapFromSource(m_root);
    view()->setRootIndex(root);
    bool hasOptions = false;
    const int rows = m_root.isValid() && !root.isValid() ? 0 : model()->rowCount(root);
    for (int row = 0; row < rows; ++row) {
        if (model()->index(row, m_column, view()->rootIndex()).data(PopupMenuItem::TypeRole).toInt() == static_cast<int>(PopupMenuRowType::Option)) {
            hasOptions = true;
            break;
        }
    }
    view()->setVisible(hasOptions);
    if (m_emptyWidget)
        m_emptyWidget->setVisible(!hasOptions);
    setAdaptiveHeight(m_width, m_visibleRows);
}

void ComboListPopup::showEvent(QShowEvent* event) {
    PopupMenu::showEvent(event);

    // Shadow margins are expanded on show; use the final viewport size for initial scrolling.
    if (layout())
        layout()->activate();
    view()->doItemsLayout();
    view()->scrollToTop();

    const QModelIndex current = view()->currentIndex();
    if (current.isValid())
        view()->scrollTo(current, QAbstractItemView::EnsureVisible);
}

} // namespace animatedwidgets
