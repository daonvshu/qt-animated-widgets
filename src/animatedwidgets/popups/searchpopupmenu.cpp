#include "animatedwidgets/popups/searchpopupmenu.h"
#include "animatedwidgets/popups/popupmenurowtype.h"
#include "animatedwidgets/popups/defaultpopupmenumodel.h"
#include "animatedwidgets/popups/groupedpopupmenumodel.h"
#include "animatedwidgets/popups/popupmenufiltermodel.h"
#include "animatedwidgets/inputs/iconlineedit.h"

#include <QAbstractItemView>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QTimer>
#include <QLabel>
#include <QListView>

namespace animatedwidgets {

SearchPopupMenu::SearchPopupMenu(QWidget* parent)
    : PopupMenu(parent)
{
    m_filterModel = new PopupMenuFilterModel(this);
    SearchPopupMenu::setModel(defaultModel());

    m_searchEditor = new IconLineEdit(this);
    // Keep the native text layer transparent so it cannot cover LineEdit's animated frame.
    m_searchEditor->setStyleSheet(QStringLiteral("QLineEdit { background: transparent; border: none; }"));
    m_searchEditor->setLeadingIcon(QIcon(QStringLiteral(":/animatedwidgets/inputs/search.svg")));
    m_searchEditor->setPlaceholderText(tr("Search"));
    m_searchEditor->installEventFilter(this);
    auto* header = new QWidget(this);
    auto* headerLayout = new QVBoxLayout(header);
    m_searchHeaderLayout = headerLayout;
    headerLayout->setContentsMargins(0, 0, 0, m_searchSpacing);
    headerLayout->addWidget(m_searchEditor);
    setTopWidget(header);
    setFocusProxy(m_searchEditor);

    auto* emptyLabel = new QLabel(tr("No results"), this);
    emptyLabel->setAlignment(Qt::AlignCenter);
    emptyLabel->setMinimumHeight(64);
    setEmptyWidget(emptyLabel);

    connect(m_filterModel, &QAbstractItemModel::rowsInserted, this, &SearchPopupMenu::scheduleAdaptiveHeight);
    connect(m_filterModel, &QAbstractItemModel::rowsRemoved, this, &SearchPopupMenu::scheduleAdaptiveHeight);
    connect(m_filterModel, &QAbstractItemModel::modelReset, this, &SearchPopupMenu::scheduleAdaptiveHeight);
    connect(m_filterModel, &QAbstractItemModel::layoutChanged, this, &SearchPopupMenu::scheduleAdaptiveHeight);
    connect(m_filterModel, &QAbstractItemModel::dataChanged, this, &SearchPopupMenu::scheduleAdaptiveHeight);

    connect(m_searchEditor.data(), &QLineEdit::textChanged, this, [this](const QString& text) {
        m_filterModel->setSearchText(text);
        emit searchTextChanged(text);
    });
    connect(this, &PopupMenu::itemTriggered, this, [this](const QModelIndex& index) {
        const QPersistentModelIndex source = m_filterModel->mapToSource(index);
        QPointer<SearchPopupMenu> guard(this);
        if (auto* model = qobject_cast<DefaultPopupMenuModel*>(sourceModel()))
            model->triggerItem(source);
        else if (auto* model = qobject_cast<GroupedPopupMenuModel*>(sourceModel()))
            model->triggerItem(source);
        if (guard && source.isValid())
            emit sourceItemTriggered(source);
    });
}

IconLineEdit* SearchPopupMenu::searchEditor() const {
    return m_searchEditor;
}

PopupMenuFilterModel* SearchPopupMenu::filterModel() const {
    return m_filterModel;
}

QAbstractItemModel* SearchPopupMenu::sourceModel() const {
    return m_filterModel->sourceModel();
}

void SearchPopupMenu::setModel(QAbstractItemModel* model) {
    if (model == m_filterModel || (model == sourceModel() && this->model() == m_filterModel))
        return;
    m_filterModel->setSourceModel(model);
    PopupMenu::setModel(m_filterModel);
}

QString SearchPopupMenu::searchText() const {
    return m_searchEditor ? m_searchEditor->text() : QString();
}

void SearchPopupMenu::setPopupSize(const QSize& size) {
    m_adaptiveWidth = 0;
    updateEmptyState();
    PopupMenu::setPopupSize(size);
}

void SearchPopupMenu::setAdaptiveHeight(int width, int maximumVisibleItems) {
    m_adaptiveWidth = qMax(1, width);
    m_maximumVisibleItems = qMax(0, maximumVisibleItems);
    updateEmptyState();
    PopupMenu::setAdaptiveHeight(m_adaptiveWidth, m_maximumVisibleItems);
}

void SearchPopupMenu::scheduleAdaptiveHeight() {
    if (m_heightUpdatePending)
        return;

    // Coalesce proxy updates and measure only after the filtering operation has finished.
    m_heightUpdatePending = true;
    QTimer::singleShot(0, this, [this] {
        m_heightUpdatePending = false;
        updateEmptyState();
        if (m_adaptiveWidth > 0)
            setAdaptiveHeight(m_adaptiveWidth, m_maximumVisibleItems);
    });
}

QWidget* SearchPopupMenu::emptyWidget() const {
    return m_emptyWidget;
}

void SearchPopupMenu::setEmptyWidget(QWidget* widget) {
    if (widget == m_emptyWidget || widget == this || widget == view() || widget == topWidget()
        || widget == bottomWidget() || widget == m_searchEditor || (widget && widget->isAncestorOf(this)))
        return;

    if (m_emptyWidget) {
        layout()->removeWidget(m_emptyWidget);
        m_emptyWidget->hide();
        m_emptyWidget->deleteLater();
    }
    m_emptyWidget = widget;
    if (widget) {
        widget->setParent(this);
        auto* boxLayout = qobject_cast<QVBoxLayout*>(layout());
        boxLayout->insertWidget(boxLayout->indexOf(view()), widget, 1);
    }
    updateEmptyState();
    scheduleAdaptiveHeight();
}

void SearchPopupMenu::updateEmptyState() {
    bool hasItems = false;
    if (view() && model()) {
        const QModelIndex root = view()->rootIndex();
        const auto* list = qobject_cast<QListView*>(view());
        for (int row = 0; row < model()->rowCount(root); ++row) {
            if (list && list->isRowHidden(row))
                continue;
            const QModelIndex index = model()->index(row, list ? list->modelColumn() : 0, root);
            if (index.data(PopupMenuItem::TypeRole).toInt() == static_cast<int>(PopupMenuRowType::Option)
                && !index.data(PopupMenuItem::SeparatorRole).toBool()) {
                hasItems = true;
                break;
            }
        }
    }

    if (view())
        view()->setVisible(hasItems);
    if (m_emptyWidget)
        m_emptyWidget->setVisible(!hasItems);
}

void SearchPopupMenu::setSearchText(const QString& text) {
    if (m_searchEditor)
        m_searchEditor->setText(text);
}

int SearchPopupMenu::searchSpacing() const {
    return m_searchSpacing;
}

void SearchPopupMenu::setSearchSpacing(int spacing) {
    spacing = qMax(0, spacing);
    if (m_searchSpacing == spacing)
        return;

    m_searchSpacing = spacing;
    if (m_searchHeaderLayout) {
        QMargins margins = m_searchHeaderLayout->contentsMargins();
        margins.setBottom(spacing);
        m_searchHeaderLayout->setContentsMargins(margins);
    }
    scheduleAdaptiveHeight();
}

bool SearchPopupMenu::highlightEnabled() const {
    return m_filterModel->highlightEnabled();
}

void SearchPopupMenu::setHighlightEnabled(bool enabled) {
    m_filterModel->setHighlightEnabled(enabled);
}

QColor SearchPopupMenu::highlightColor() const {
    return m_filterModel->highlightColor();
}

void SearchPopupMenu::setHighlightColor(const QColor& color) {
    m_filterModel->setHighlightColor(color);
}

bool SearchPopupMenu::highlightBold() const {
    return m_filterModel->highlightBold();
}

void SearchPopupMenu::setHighlightBold(bool bold) {
    m_filterModel->setHighlightBold(bold);
}

bool SearchPopupMenu::eventFilter(QObject* watched, QEvent* event) {
    if (watched == m_searchEditor && event->type() == QEvent::KeyPress) {
        const auto* key = static_cast<QKeyEvent*>(event);
        if (key->key() == Qt::Key_Escape) {
            close();
            return true;
        }
        if (key->key() == Qt::Key_Down) {
            for (int row = 0; row < model()->rowCount(); ++row) {
                const QModelIndex index = model()->index(row, 0);
                if (index.flags().testFlag(Qt::ItemIsEnabled) && index.flags().testFlag(Qt::ItemIsSelectable)) {
                    view()->setCurrentIndex(index);
                    view()->setFocus();
                    break;
                }
            }
            return true;
        }
    }
    return PopupMenu::eventFilter(watched, event);
}

bool SearchPopupMenu::includeListInAdaptiveHeight() const {
    return view() && !view()->isHidden();
}

} // namespace animatedwidgets
