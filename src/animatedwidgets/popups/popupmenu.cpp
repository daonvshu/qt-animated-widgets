#include "animatedwidgets/popups/popupmenu.h"

#include "animatedwidgets/popups/defaultpopupmenustyle.h"
#include "animatedwidgets/popups/defaultpopupmenuliststyle.h"
#include "animatedwidgets/popups/popupmenuitemdelegate.h"
#include "animatedwidgets/popups/defaultpopupmenumodel.h"

#include <QAbstractItemDelegate>
#include <QAbstractItemModel>
#include <QAbstractItemView>
#include <QEvent>
#include <QKeyEvent>
#include <QListView>
#include <QScrollBar>
#include <QVBoxLayout>

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
    PopupMenu::setModel(m_defaultModel);
}

PopupMenu::~PopupMenu()
{
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
    if (m_view) {
        m_view->removeEventFilter(this);
        disconnect(m_view, nullptr, this, nullptr);
        layout()->removeWidget(m_view);
        m_view->hide();
        m_view->deleteLater();
    }
    m_view = view;
    m_view->setParent(this);
    m_view->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_view->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_view->installEventFilter(this);
    setFocusProxy(m_view);
    if (auto* boxLayout = qobject_cast<QVBoxLayout*>(layout()))
        boxLayout->insertWidget(m_topWidget ? 1 : 0, m_view, 1);
    if (previousModel)
        m_view->setModel(previousModel);
    if (m_delegate)
        m_view->setItemDelegate(m_delegate);
    connectViewSignals();
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
    if (previousDelegate && previousDelegate->parent() == this)
        previousDelegate->deleteLater();
    applyStyle();
}

PopupMenuStyle* PopupMenu::popupStyle() const {
    return m_style;
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
    return m_listStyle;
}

void PopupMenu::setListStyle(PopupMenuListStyle* style) {
    if (!style || style == m_listStyle)
        return;
    delete m_listStyle;
    m_listStyle = style;
    applyStyle();
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
    disconnect(m_modelTriggerConnection);
    if (m_view)
        m_view->setModel(model);
    if (auto* menuModel = qobject_cast<DefaultPopupMenuModel*>(model))
        m_modelTriggerConnection = connect(this, &PopupMenu::itemTriggered,
                                           menuModel, &DefaultPopupMenuModel::triggerItem);
}

DefaultPopupMenuModel* PopupMenu::defaultModel() const {
    return m_defaultModel;
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

void PopupMenu::setAutoClose(bool enabled) {
    m_autoClose = enabled;
}

void PopupMenu::setPopupSize(const QSize& size) {
    setContentFixedSize(size.width(), size.height());
}

void PopupMenu::setAdaptiveHeight(int width, int maximumVisibleItems) {
    if (!m_view)
        return;

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
        int visibleItems = 0;
        for (int row = 0; row < itemModel->rowCount(root); ++row) {
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
    const QMargins bodyMargins = m_style ? m_style->margins() : QMargins();
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
    if (watched == m_view && event->type() == QEvent::KeyPress) {
        const auto* key = static_cast<QKeyEvent*>(event);
        if (key->key() == Qt::Key_Escape) {
            close();
            return true;
        }
        if (key->key() == Qt::Key_Return || key->key() == Qt::Key_Enter || key->key() == Qt::Key_Space) {
            const QModelIndex index = m_view->currentIndex();
            if (index.isValid() && index.flags().testFlag(Qt::ItemIsEnabled)
                && index.flags().testFlag(Qt::ItemIsSelectable)) {
                QPointer<PopupMenu> guard(this);
                emit itemTriggered(index);
                if (guard && m_autoClose)
                    close();
            }
            return true;
        }
    }
    return BasePopupDlg::eventFilter(watched, event);
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
    if (!m_style)
        return;
    m_style->applyPopup(this);
    if (m_listStyle) {
        m_listStyle->apply(m_view);
        if (auto* boxLayout = qobject_cast<QVBoxLayout*>(layout()))
            boxLayout->setContentsMargins(m_listStyle->margins());
    }

    if (m_view) {
        m_view->verticalScrollBar()->setStyleSheet(m_scrollBarStyleSheet);
        m_view->horizontalScrollBar()->setStyleSheet(m_scrollBarStyleSheet);
    }
}

void PopupMenu::connectViewSignals() {
    if (!m_view)
        return;
    connect(m_view, &QAbstractItemView::clicked, this, [this](const QModelIndex& index) {
        if (!index.isValid() || !index.flags().testFlag(Qt::ItemIsEnabled)
            || !index.flags().testFlag(Qt::ItemIsSelectable))
            return;
        QPointer<PopupMenu> guard(this);
        emit itemTriggered(index);
        if (guard && m_autoClose)
            close();
    });
}

} // namespace animatedwidgets
