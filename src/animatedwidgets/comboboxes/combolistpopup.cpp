#include "animatedwidgets/comboboxes/combolistpopup.h"
#include "animatedwidgets/comboboxes/combopopupmodel.h"
#include "animatedwidgets/popups/defaultpopupmenuliststyle.h"

#include <QAbstractItemModel>
#include <QListView>
#include <QLayout>

namespace animatedwidgets {

ComboListPopup::ComboListPopup(QWidget* parent)
    : PopupMenu(parent)
{
    Q_INIT_RESOURCE(comboboxes);
    m_popupModel = new ComboPopupModel(this);
    auto* style = new DefaultPopupMenuListStyle;
    style->checkedIcon = QIcon(QStringLiteral(":/animatedwidgets/comboboxes/checked.svg"));
    style->checkedIconAlignment = Qt::AlignRight;
    setListStyle(style);

    connect(this, &PopupMenu::itemTriggered, this, [this](const QModelIndex& index) {
        const QModelIndex source = m_popupModel->mapToSource(index);
        if (source.isValid())
            emit itemChosen(source.row());
    });
}

void ComboListPopup::setItems(QAbstractItemModel* model, const QModelIndex& root, int column, int currentRow, int visibleRows, int width) {
    m_root = root;
    m_column = column;
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
    m_popupModel->setCurrentIndex(source);
    const QModelIndex current = m_popupModel->mapFromSource(source);
    view()->setCurrentIndex(current);
    if (isVisible() && current.isValid())
        view()->scrollTo(current, QAbstractItemView::EnsureVisible);
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
