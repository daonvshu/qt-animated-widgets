#include "animatedwidgets/popups/popupmenu.h"

#include "animatedwidgets/popups/defaultpopupmenustyle.h"
#include "animatedwidgets/popups/defaultpopupmenuliststyle.h"
#include "animatedwidgets/popups/popupmenuitemdelegate.h"
#include "animatedwidgets/popups/defaultpopupmenumodel.h"
#include "animatedwidgets/popups/groupedpopupmenumodel.h"
#include "animatedwidgets/popups/popupmenurowtype.h"
#include "animatedwidgets/utils/smoothscrolling.h"

#include <QAbstractItemDelegate>
#include <QAbstractItemModel>
#include <QAbstractItemView>
#include <QEvent>
#include <QKeyEvent>
#include <QListView>
#include <QTreeView>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QApplication>
#include <QMouseEvent>
#include <QHideEvent>
#include <QScreen>
#include <QAbstractProxyModel>
#include <QWindow>
#include <QItemSelectionModel>

namespace animatedwidgets {

PopupMenu::PopupMenu(QWidget* parent)
    : BasePopupDlg(parent)
{
    m_scrollBarStyleSheet = QStringLiteral(
        "QScrollBar { background: transparent; border: none; }"
        "QScrollBar:vertical { width: 6px; margin: 0; }"
        "QScrollBar:horizontal { height: 6px; margin: 0; }"
        "QScrollBar::handle { background: rgba(128, 128, 128, 64); border-radius: 3px; }"
        "QScrollBar::handle:hover { background: rgba(128, 128, 128, 112); }"
        "QScrollBar::handle:pressed { background: rgba(128, 128, 128, 160); }"
        "QScrollBar::handle:vertical { min-height: 24px; }"
        "QScrollBar::handle:horizontal { min-width: 24px; }"
        "QScrollBar::add-line, QScrollBar::sub-line { width: 0; height: 0; border: none; background: transparent; }"
        "QScrollBar::add-page, QScrollBar::sub-page { background: transparent; }");

    setObjectName(QStringLiteral("animatedPopupMenu"));
    auto* outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->setSpacing(0);

    m_style = new DefaultPopupMenuStyle;
    m_listStyle = new DefaultPopupMenuListStyle;
    createDefaultView();
    m_defaultModel = new DefaultPopupMenuModel(this);
    m_groupedModel = new GroupedPopupMenuModel(this);
    PopupMenu::setModel(m_defaultModel);
    connect(this, &BasePopupDlg::closeRequested, this, &PopupMenu::hideSubmenus);
    qApp->installEventFilter(this);
}

PopupMenu::~PopupMenu()
{
    // QWidget destroys child models/views after this class's members have been destroyed.
    for (const auto& connection : m_submenuModelConnections)
        disconnect(connection);
    for (const auto& connection : m_viewConnections)
        disconnect(connection);
    disconnect(m_modelTriggerConnection);
    disconnect(m_currentIndexConnection);
    disconnect(this, &BasePopupDlg::closeRequested, this, &PopupMenu::hideSubmenus);

    if (qApp)
        qApp->removeEventFilter(this);
    if (m_view) {
        m_view->removeEventFilter(this);
        m_view->viewport()->removeEventFilter(this);
    }

    hideSubmenus();
    delete m_style;
    delete m_listStyle;
}

QAbstractItemView* PopupMenu::view() const {
    return m_view;
}

void PopupMenu::setViewFactory(ViewFactory factory) {
    m_viewFactory = std::move(factory);
    if (!isVisible())
        createDefaultView();
}

void PopupMenu::setView(QAbstractItemView* view) {
    if (!view || view == m_view)
        return;

    QAbstractItemModel* previousModel = model();
    clearSubmenus();
    setHoveredIndex(QModelIndex());
    disconnect(m_currentIndexConnection);
    for (const auto& connection : m_viewConnections)
        disconnect(connection);
    m_viewConnections.clear();
    if (m_view) {
        m_view->viewport()->removeEventFilter(this);
        m_view->removeEventFilter(this);
        disconnect(m_view, nullptr, this, nullptr);
        layout()->removeWidget(m_view);
        m_view->hide();
        m_view->deleteLater();
    }
    m_view = view;
    m_view->setParent(this);
    m_view->installEventFilter(this);
    m_view->viewport()->installEventFilter(this);
    m_view->setMouseTracking(true);
    m_view->viewport()->setMouseTracking(true);
    SmoothScrolling::install(m_view);
    setFocusProxy(m_view);
    if (auto* boxLayout = qobject_cast<QVBoxLayout*>(layout()))
        boxLayout->insertWidget(m_topWidget ? 1 : 0, m_view, 1);
    if (previousModel)
        m_view->setModel(previousModel);
    if (m_delegate)
        m_view->setItemDelegate(m_delegate);
    connectViewSignals();
    connectCurrentIndexSignal();
    applyStyle();
}

QWidget* PopupMenu::topWidget() const {
    return m_topWidget;
}

void PopupMenu::setTopWidget(QWidget* widget) {
    setSlotWidget(m_topWidget, widget, true);
}

QWidget* PopupMenu::bottomWidget() const {
    return m_bottomWidget;
}

void PopupMenu::setBottomWidget(QWidget* widget) {
    setSlotWidget(m_bottomWidget, widget, false);
}

void PopupMenu::setSlotWidget(QPointer<QWidget>& slot, QWidget* widget, bool top) {
    if (widget == slot || widget == this || widget == m_view || (widget && widget->isAncestorOf(this)))
        return;
    if (widget && (widget == m_topWidget || widget == m_bottomWidget))
        return;

    if (slot) {
        layout()->removeWidget(slot);
        slot->hide();
        slot->deleteLater();
    }
    slot = widget;
    if (slot) {
        slot->setParent(this);
        auto* boxLayout = qobject_cast<QVBoxLayout*>(layout());
        boxLayout->insertWidget(top ? 0 : boxLayout->count(), slot);
        slot->show();
    }
}

QAbstractItemDelegate* PopupMenu::itemDelegate() const {
    return m_delegate;
}

void PopupMenu::setItemDelegate(QAbstractItemDelegate* delegate) {
    if (!m_view || !delegate || delegate == m_delegate)
        return;
    auto* previousDelegate = m_delegate.data();
    m_delegate = delegate;
    m_delegate->setParent(this);
    m_view->setItemDelegate(m_delegate);
    if (auto* menuDelegate = qobject_cast<PopupMenuItemDelegate*>(delegate))
        menuDelegate->setHoveredIndex(m_hoveredIndex);
    if (previousDelegate && previousDelegate->parent() == this)
        previousDelegate->deleteLater();
    applyStyle();
}

PopupMenuStyle* PopupMenu::popupStyle() const {
    return m_parentMenu ? m_parentMenu->popupStyle() : m_style;
}

void PopupMenu::setPopupStyle(PopupMenuStyle* style) {
    if (!style || style == m_style)
        return;
    if (m_style)
        delete m_style;
    m_style = style;
    applyStyle();
}

void PopupMenu::refreshStyle() {
    applyStyle();
    if (m_view)
        m_view->viewport()->update();
}

void PopupMenu::setSurfaceMetrics(int radius, int cornerRadius) {
    setShadowRadius(qMax(0, radius));
    shadowCornerRadius = qMax(0, cornerRadius);
    shadowImage = QImage();
    update();
}

PopupMenuListStyle* PopupMenu::listStyle() const {
    return m_parentMenu ? m_parentMenu->listStyle() : m_listStyle;
}

void PopupMenu::setListStyle(PopupMenuListStyle* style) {
    if (!style || style == m_listStyle)
        return;
    delete m_listStyle;
    m_listStyle = style;
    applyStyle();
}

bool PopupMenu::iconHint() const {
    return listStyle()->iconHint;
}

void PopupMenu::setIconHint(bool enabled) {
    if (listStyle()->iconHint == enabled)
        return;
    listStyle()->iconHint = enabled;
    refreshStyle();
}

QString PopupMenu::scrollBarStyleSheet() const {
    return m_scrollBarStyleSheet;
}

void PopupMenu::setScrollBarStyleSheet(const QString& styleSheet) {
    if (m_scrollBarStyleSheet == styleSheet)
        return;

    m_scrollBarStyleSheet = styleSheet;
    applyStyle();
}

QAbstractItemModel* PopupMenu::model() const {
    return m_view ? m_view->model() : nullptr;
}

void PopupMenu::setModel(QAbstractItemModel* model) {
    clearSubmenus();
    setHoveredIndex(QModelIndex());
    disconnect(m_currentIndexConnection);
    for (const auto& connection : m_submenuModelConnections)
        disconnect(connection);
    m_submenuModelConnections.clear();
    disconnect(m_modelTriggerConnection);
    if (m_view)
        m_view->setModel(model);
    connectCurrentIndexSignal();
    if (auto* menuModel = qobject_cast<DefaultPopupMenuModel*>(model))
        m_modelTriggerConnection = connect(this, &PopupMenu::itemTriggered,
                                           menuModel, &DefaultPopupMenuModel::triggerItem);
    else if (auto* groupedModel = qobject_cast<GroupedPopupMenuModel*>(model))
        m_modelTriggerConnection = connect(this, &PopupMenu::itemTriggered,
                                           groupedModel, &GroupedPopupMenuModel::triggerItem);
    if (model) {
        m_submenuModelConnections.append(connect(model, &QAbstractItemModel::modelAboutToBeReset,
                                                  this, &PopupMenu::clearSubmenus));
        m_submenuModelConnections.append(connect(model, &QAbstractItemModel::rowsAboutToBeRemoved,
                                                  this, &PopupMenu::clearSubmenus));
        m_submenuModelConnections.append(connect(model, &QAbstractItemModel::rowsAboutToBeInserted,
                                                  this, &PopupMenu::clearSubmenus));
        m_submenuModelConnections.append(connect(model, &QAbstractItemModel::rowsAboutToBeMoved,
                                                  this, &PopupMenu::clearSubmenus));
        m_submenuModelConnections.append(connect(model, &QAbstractItemModel::layoutAboutToBeChanged,
                                                  this, &PopupMenu::clearSubmenus));
        m_submenuModelConnections.append(connect(model, &QAbstractItemModel::dataChanged, this,
            [this](const QModelIndex&, const QModelIndex&, const QVector<int>& roles) {
                if (roles.isEmpty() || roles.contains(PopupMenuItem::ChildrenRole)
                    || roles.contains(PopupMenuItem::SubmenuMultiSelectionRole))
                    clearSubmenus();
            }));
        m_submenuModelConnections.append(connect(model, &QObject::destroyed, this, &PopupMenu::clearSubmenus));
    }
}

DefaultPopupMenuModel* PopupMenu::defaultModel() const {
    return m_defaultModel;
}

GroupedPopupMenuModel* PopupMenu::groupedModel() const {
    return m_groupedModel;
}

void PopupMenu::setStrings(const QStringList& strings) {
    m_defaultModel->setStrings(strings);
    if (model() != m_defaultModel)
        setModel(m_defaultModel);
}

bool PopupMenu::autoClose() const {
    return m_autoClose;
}

void PopupMenu::setItems(const QList<PopupMenuItem>& items) {
    m_defaultModel->setItems(items);
    if (model() != m_defaultModel)
        setModel(m_defaultModel);
}

void PopupMenu::setGroups(const QList<PopupMenuGroup>& groups) {
    m_groupedModel->setGroups(groups);
    if (model() != m_groupedModel)
        setModel(m_groupedModel);
}

void PopupMenu::setAutoClose(bool enabled) {
    m_autoClose = enabled;
}

void PopupMenu::setChildren(const QList<PopupMenuNode>& children) {
    QList<PopupMenuItem> items;
    for (const auto& child : children)
        if (child.item() || child.group())
            items.append(child.toItem());
    setItems(items);
}

PopupMenu* PopupMenu::activeSubmenu() const {
    return m_activeSubmenu;
}

void PopupMenu::setPopupSize(const QSize& size) {
    setContentFixedSize(size.width(), size.height());
}

void PopupMenu::setAdaptiveHeight(int width, int maximumVisibleItems) {
    if (!m_view)
        return;

    ensureShadowSizeExpanded();
    ensurePolished();
    m_view->ensurePolished();
    setContentFixedWidth(qMax(1, width));
    if (layout())
        layout()->activate();
    m_view->doItemsLayout();

    int height = 0;
    if (auto* itemModel = includeListInAdaptiveHeight() ? model() : nullptr) {
        const QModelIndex root = m_view->rootIndex();
        const auto* listView = qobject_cast<QListView*>(m_view.data());
        const auto* treeView = qobject_cast<QTreeView*>(m_view.data());
        int visibleItems = 0;
        if (treeView) {
            QModelIndex index = itemModel->index(0, 0, root);
            while (index.isValid() && (maximumVisibleItems <= 0 || visibleItems < maximumVisibleItems)) {
                if (!treeView->isRowHidden(index.row(), index.parent())) {
                    const int column = qMax(0, treeView->treePosition());
                    height += qMax(0, m_view->sizeHintForIndex(index.sibling(index.row(), column)).height());
                    ++visibleItems;
                }
                index = treeView->indexBelow(index);
            }
        }
        for (int row = 0; !treeView && row < itemModel->rowCount(root); ++row) {
            if (listView && listView->isRowHidden(row))
                continue;
            if (maximumVisibleItems > 0 && visibleItems >= maximumVisibleItems)
                break;
            height += qMax(0, m_view->sizeHintForRow(row));
            if (listView)
                height += 2 * qMax(0, listView->spacing());
            ++visibleItems;
        }
    }

    const QMargins listMargins = layout() ? layout()->contentsMargins() : QMargins();
    const QMargins bodyMargins = popupStyle() ? popupStyle()->margins() : QMargins();
    const QMargins viewMargins = m_view->contentsMargins();
    height += listMargins.top() + listMargins.bottom() + bodyMargins.top() + bodyMargins.bottom()
        + viewMargins.top() + viewMargins.bottom();
    for (int item = 0; layout() && item < layout()->count(); ++item) {
        auto* widget = layout()->itemAt(item)->widget();
        if (!widget || widget == m_view || widget->isHidden())
            continue;
        const int availableWidth = qMax(1, width - listMargins.left() - listMargins.right()
                                          - bodyMargins.left() - bodyMargins.right());
        const int hint = widget->hasHeightForWidth() ? widget->heightForWidth(availableWidth) : widget->sizeHint().height();
        height += qBound(widget->minimumHeight(), qMax(widget->minimumSizeHint().height(), hint), widget->maximumHeight());
        height += qMax(0, layout()->spacing());
    }
    setContentFixedHeight(qMax(1, height));
}

void PopupMenu::showAt(const QPoint& globalPos) {
    moveTo(globalPos, Qt::TopEdge, Qt::AlignLeft);
    show();
}

bool PopupMenu::includeListInAdaptiveHeight() const {
    return true;
}

bool PopupMenu::eventFilter(QObject* watched, QEvent* event) {
    if (!m_parentMenu && isVisible() && filterMenuChainEvent(watched, event))
        return true;

    if (watched == m_view && event->type() == QEvent::KeyPress) {
        const auto* key = static_cast<QKeyEvent*>(event);
        if (key->key() == Qt::Key_Escape) {
            if (m_parentMenu) {
                m_parentMenu->hideSubmenus();
                m_parentMenu->view()->setFocus();
            } else {
                close();
            }
            return true;
        }
        if (key->key() == Qt::Key_Right && !qobject_cast<QTreeView*>(m_view.data())) {
            openSubmenu(m_view->currentIndex(), true);
            return true;
        }
        if (key->key() == Qt::Key_Left && m_parentMenu) {
            m_parentMenu->hideSubmenus();
            m_parentMenu->view()->setFocus();
            return true;
        }
        if (key->key() == Qt::Key_Return || key->key() == Qt::Key_Enter || key->key() == Qt::Key_Space) {
            activateItem(m_view->currentIndex());
            return true;
        }
    }
    return BasePopupDlg::eventFilter(watched, event);
}

void PopupMenu::hideEvent(QHideEvent* event) {
    m_pressedMenu.clear();
    m_pressedIndex = QPersistentModelIndex();
    m_submenuMousePress = false;
    hideSubmenus();
    setHoveredIndex(QModelIndex());
    BasePopupDlg::hideEvent(event);
}

void PopupMenu::createDefaultView() {
    QAbstractItemView* view = m_viewFactory ? m_viewFactory(this) : new QListView(this);
    if (!view)
        view = new QListView(this);
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view->setMouseTracking(true);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    setView(view);

    if (!m_delegate) {
        auto* delegate = new PopupMenuItemDelegate(this);
        setItemDelegate(delegate);
    }
}

void PopupMenu::applyStyle() {
    auto* style = popupStyle();
    auto* list = listStyle();
    if (!style)
        return;
    style->applyPopup(this);
    if (list) {
        list->apply(m_view);
        if (auto* boxLayout = qobject_cast<QVBoxLayout*>(layout()))
            boxLayout->setContentsMargins(list->margins());
    }
    for (const auto& submenu : m_submenus)
        if (submenu)
            submenu->applyStyle();

    if (m_view) {
        m_view->verticalScrollBar()->setStyleSheet(m_scrollBarStyleSheet);
        m_view->horizontalScrollBar()->setStyleSheet(m_scrollBarStyleSheet);
    }
}

void PopupMenu::connectViewSignals() {
    if (!m_view)
        return;
    m_viewConnections.append(connect(m_view, &QAbstractItemView::clicked, this, &PopupMenu::activateItem));
    m_viewConnections.append(connect(m_view->verticalScrollBar(), &QScrollBar::valueChanged,
                                     this, &PopupMenu::hideSubmenus));
    m_viewConnections.append(connect(m_view->horizontalScrollBar(), &QScrollBar::valueChanged,
                                     this, &PopupMenu::hideSubmenus));
}

void PopupMenu::connectCurrentIndexSignal() {
    disconnect(m_currentIndexConnection);
    if (!m_view || !m_view->selectionModel())
        return;

    m_currentIndexConnection = connect(m_view->selectionModel(), &QItemSelectionModel::currentChanged,
                                       this, [this](const QModelIndex& current) {
        if (!current.isValid() && m_activeSubmenu)
            return;
        if (current.isValid() && m_activeSubmenu && m_submenuIndex != current)
            hideSubmenus();
        setHoveredIndex(current);
    });
}

void PopupMenu::activateItem(const QModelIndex& index) {
    if (!index.isValid() || !index.flags().testFlag(Qt::ItemIsEnabled)
        || !index.flags().testFlag(Qt::ItemIsSelectable))
        return;
    if (!index.data(PopupMenuItem::ChildrenRole).value<QList<PopupMenuNode>>().isEmpty()) {
        openSubmenu(index, true);
        return;
    }

    QPointer<PopupMenu> guard(this);
    QPointer<PopupMenu> root(rootMenu());
    const QList<int> path = itemPath(index);
    const QPersistentModelIndex persistent(index);
    emit itemTriggered(index);
    if (!guard || !root || !persistent.isValid())
        return;
    emit root->itemPathTriggered(path, persistent);
    if (guard && root && root->autoClose())
        root->close();
}

QList<int> PopupMenu::itemPath(const QModelIndex& index) const {
    QList<int> path;
    if (m_parentMenu)
        path = m_parentMenu->itemPath(m_parentMenu->m_submenuIndex);
    QModelIndex source = index;
    while (const auto* proxy = qobject_cast<const QAbstractProxyModel*>(source.model()))
        source = proxy->mapToSource(source);
    if (!source.isValid())
        return path;

    int logicalIndex = -1;
    for (int row = 0; row <= source.row(); ++row) {
        const QModelIndex item = source.model()->index(row, source.column(), source.parent());
        if (!item.data(PopupMenuItem::SeparatorRole).toBool()
            && item.data(PopupMenuItem::TypeRole).toInt() == static_cast<int>(PopupMenuRowType::Option))
            ++logicalIndex;
    }
    path.append(logicalIndex);
    return path;
}

void PopupMenu::openSubmenu(const QModelIndex& index, bool keyboard) {
    const auto children = index.data(PopupMenuItem::ChildrenRole).value<QList<PopupMenuNode>>();
    if (!index.isValid() || children.isEmpty() || !index.flags().testFlag(Qt::ItemIsEnabled)
        || !index.flags().testFlag(Qt::ItemIsSelectable)) {
        hideSubmenus();
        return;
    }
    if (m_activeSubmenu && m_submenuIndex == index && m_activeSubmenu->isVisible()) {
        if (keyboard) {
            m_activeSubmenu->setHoveredIndex(m_activeSubmenu->view()->currentIndex());
            m_activeSubmenu->view()->setFocus();
        }
        return;
    }

    hideSubmenus();
    auto* submenu = m_submenus.value(index).data();
    if (!submenu) {
        submenu = new PopupMenu(this);
        submenu->m_parentMenu = this;
        submenu->setPopupAnimation(nullptr);
        if (rootMenu()->windowFlags().testFlag(Qt::WindowDoesNotAcceptFocus)) {
            submenu->setWindowFlag(Qt::WindowDoesNotAcceptFocus);
            submenu->setAttribute(Qt::WA_ShowWithoutActivating);
        }
        submenu->setChildren(children);
        submenu->defaultModel()->setSelectionMode(index.data(PopupMenuItem::SubmenuMultiSelectionRole).toBool()
            ? DefaultPopupMenuModel::MultiSelection : DefaultPopupMenuModel::SingleSelection);
        m_submenus.insert(index, submenu);
    }
    m_activeSubmenu = submenu;
    m_submenuIndex = index;
    setHoveredIndex(index);
    if (keyboard)
        m_view->setCurrentIndex(index);
    submenu->setScrollBarStyleSheet(rootMenu()->scrollBarStyleSheet());
    submenu->applyStyle();

    int width = qMax(160, m_view->width());
    for (int row = 0; row < submenu->model()->rowCount(); ++row)
        width = qMax(width, submenu->view()->sizeHintForIndex(submenu->model()->index(row, 0)).width()
                             + submenu->layout()->contentsMargins().left()
                             + submenu->layout()->contentsMargins().right());
    const QScreen* screen = QGuiApplication::screenAt(m_view->viewport()->mapToGlobal(m_view->visualRect(index).topRight()));
    const QRect available = screen ? screen->availableGeometry() : QRect(0, 0, 1920, 1080);
    submenu->setAdaptiveHeight(qMax(1, qMin(width, available.width() - 2 * submenu->shadowRadius)));
    submenu->setContentFixedHeight(qMin(submenu->height() - submenu->shadowMargins().top()
                                       - submenu->shadowMargins().bottom(),
                                       qMax(1, available.height() - 2 * submenu->shadowRadius)));
    const QRect rowRect(m_view->viewport()->mapToGlobal(m_view->visualRect(index).topLeft()),
                        m_view->visualRect(index).size());
    // Inherit the parent's resolved placement; BasePopupDlg flips again only at a screen edge.
    if (m_parentMenu && arrowEdgeAlign == Qt::RightEdge)
        submenu->moveToTargetLeft(rowRect, Qt::AlignTop, 4);
    else
        submenu->moveToTargetRight(rowRect, Qt::AlignTop, 4);
    submenu->show();
    if (keyboard) {
        for (int row = 0; row < submenu->model()->rowCount(); ++row) {
            const QModelIndex first = submenu->model()->index(row, 0);
            if (first.flags().testFlag(Qt::ItemIsEnabled) && first.flags().testFlag(Qt::ItemIsSelectable)) {
                submenu->view()->setCurrentIndex(first);
                submenu->setHoveredIndex(first);
                break;
            }
        }
        submenu->view()->setFocus();
    } else {
        submenu->setHoveredIndex(QModelIndex());
    }
}

void PopupMenu::hideSubmenus() {
    if (m_activeSubmenu) {
        m_activeSubmenu->hideSubmenus();
        m_activeSubmenu->hide();
    }
    m_activeSubmenu.clear();
    m_submenuIndex = QPersistentModelIndex();
}

void PopupMenu::clearSubmenus() {
    hideSubmenus();
    const auto submenus = m_submenus;
    m_submenus.clear();
    for (const auto& submenu : submenus)
        if (submenu)
            submenu->deleteLater();
}

PopupMenu* PopupMenu::rootMenu() {
    return m_parentMenu ? m_parentMenu->rootMenu() : this;
}

PopupMenu* PopupMenu::menuAt(const QPoint& globalPos) {
    if (m_activeSubmenu && m_activeSubmenu->isVisible())
        if (auto* menu = m_activeSubmenu->menuAt(globalPos))
            return menu;
    const QMargins margins = shadowMargins();
    const QRect body = rect().adjusted(margins.left(), margins.top(), -margins.right(), -margins.bottom());
    return isVisible() && (body.contains(mapFromGlobal(globalPos)) || submenuTransitionRect().contains(globalPos))
        ? this : nullptr;
}

QRect PopupMenu::submenuTransitionRect() const {
    if (!m_view || !m_activeSubmenu || !m_activeSubmenu->isVisible() || !m_submenuIndex.isValid())
        return QRect();

    const QRect rowRect(m_view->viewport()->mapToGlobal(m_view->visualRect(m_submenuIndex).topLeft()),
                        m_view->visualRect(m_submenuIndex).size());
    const QMargins margins = m_activeSubmenu->shadowMargins();
    const QRect childBody = m_activeSubmenu->geometry().adjusted(margins.left(), margins.top(),
                                                                -margins.right(), -margins.bottom());
    if (childBody.left() > rowRect.right())
        return QRect(rowRect.right() + 1, rowRect.top(), childBody.left() - rowRect.right() - 1, rowRect.height());
    if (childBody.right() < rowRect.left())
        return QRect(childBody.right() + 1, rowRect.top(), rowRect.left() - childBody.right() - 1, rowRect.height());
    return QRect();
}

void PopupMenu::setHoveredIndex(const QModelIndex& index) {
    const QModelIndex hovered = m_view && index.model() == m_view->model()
        && index.flags().testFlag(Qt::ItemIsEnabled) && index.flags().testFlag(Qt::ItemIsSelectable)
        ? index : QModelIndex();
    if (m_hoveredIndex == hovered)
        return;

    const QPersistentModelIndex previous = m_hoveredIndex;
    m_hoveredIndex = hovered;
    if (auto* delegate = qobject_cast<PopupMenuItemDelegate*>(m_delegate.data()))
        delegate->setHoveredIndex(hovered);
    if (m_view) {
        if (previous.isValid())
            m_view->viewport()->update(m_view->visualRect(previous));
        if (hovered.isValid())
            m_view->viewport()->update(m_view->visualRect(hovered));
    }
}

void PopupMenu::updateHoveredItem(const QPoint& globalPos) {
    // Keep the active branch open while crossing the gap in either placement direction.
    if (submenuTransitionRect().contains(globalPos))
        return;

    const QPoint localPos = m_view->viewport()->mapFromGlobal(globalPos);
    if (m_view->viewport()->rect().contains(localPos)) {
        const QModelIndex index = m_view->indexAt(localPos);
        if (!index.isValid() || index.data(PopupMenuItem::SeparatorRole).toBool()
            || index.data(PopupMenuItem::TypeRole).toInt() != static_cast<int>(PopupMenuRowType::Option)) {
            if (!m_activeSubmenu)
                setHoveredIndex(QModelIndex());
            return;
        }
        setHoveredIndex(index);
        openSubmenu(index);
    } else if (!m_activeSubmenu) {
        setHoveredIndex(QModelIndex());
    }
}

bool PopupMenu::filterMenuChainEvent(QObject* watched, QEvent* event) {
    const auto type = event->type();
    if (type != QEvent::MouseMove && type != QEvent::MouseButtonPress
        && type != QEvent::MouseButtonRelease)
        return false;

    const auto* mouse = static_cast<QMouseEvent*>(event);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    const QPoint pos = mouse->globalPosition().toPoint();
#else
    const QPoint pos = mouse->globalPos();
#endif
    auto* target = menuAt(pos);
    if (type == QEvent::MouseMove) {
        if (target)
            target->updateHoveredItem(pos);
        for (auto* menu = this; menu; menu = menu->m_activeSubmenu.data())
            if (menu != target && !menu->m_activeSubmenu)
                menu->setHoveredIndex(QModelIndex());
        const auto* widget = qobject_cast<QWidget*>(watched);
        if (target && widget && widget->window() != target->window())
            return true;
        const auto* window = qobject_cast<QWindow*>(watched);
        if (target && window && window != target->windowHandle())
            return true;
        return false;
    }
    if (type == QEvent::MouseButtonRelease && mouse->button() == Qt::LeftButton && m_submenuMousePress) {
        const QPointer<PopupMenu> pressedMenu = m_pressedMenu;
        const QPersistentModelIndex pressedIndex = m_pressedIndex;
        m_pressedMenu.clear();
        m_pressedIndex = QPersistentModelIndex();
        m_submenuMousePress = false;
        if (target && target == pressedMenu && pressedIndex.isValid()
            && target->view()->indexAt(target->view()->viewport()->mapFromGlobal(pos)) == pressedIndex)
            target->activateItem(pressedIndex);
        return true;
    }
    if (!target) {
        if (m_activeSubmenu && type == QEvent::MouseButtonPress) {
            close();
            return true;
        }
        return false;
    }

    if (type == QEvent::MouseButtonPress && mouse->button() == Qt::LeftButton && m_activeSubmenu) {
        const QPoint localPos = target->view()->viewport()->mapFromGlobal(pos);
        if (target->view()->viewport()->rect().contains(localPos)) {
            // Native popup grabs can deliver a parent-row click to the child window.
            m_pressedMenu = target;
            m_pressedIndex = target->view()->indexAt(localPos);
            m_submenuMousePress = true;
            return true;
        }
    }
    return false;
}

} // namespace animatedwidgets
