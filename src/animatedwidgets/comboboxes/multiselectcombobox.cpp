#include "animatedwidgets/comboboxes/multiselectcombobox.h"

#include "animatedwidgets/buttons/hovericonbutton.h"
#include "animatedwidgets/inputs/lineedit.h"
#include "animatedwidgets/popups/defaultpopupmenuliststyle.h"
#include "animatedwidgets/popups/defaultpopupmenumodel.h"
#include "animatedwidgets/popups/popupdismissfilter.h"
#include "animatedwidgets/popups/popupmenu.h"
#include "animatedwidgets/popups/popupmenufiltermodel.h"
#include "animatedwidgets/tags/tagwidget.h"
#include "animatedwidgets/utils/widgetresources.h"
#include "animatedwidgets/utils/smoothscrolling.h"

#include <QAbstractItemView>
#include <QApplication>
#include <QHBoxLayout>
#include <QInputMethodEvent>
#include <QKeyEvent>
#include <QLabel>
#include <QMouseEvent>
#include <QScrollArea>
#include <QScrollBar>
#include <QTimer>
#include <QWheelEvent>
#include <utility>

namespace animatedwidgets {

MultiSelectComboBox::MultiSelectComboBox(QWidget* parent)
    : CompositeInputBase(parent)
{
    WidgetResources::initializeComboBoxes();
    WidgetResources::initializeInputs();
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_model = new DefaultPopupMenuModel(this);
    m_model->setSelectionMode(DefaultPopupMenuModel::MultiSelection);
    m_filterModel = new PopupMenuFilterModel(this);
    m_filterModel->setSourceModel(m_model);

    m_body = new QWidget(this);
    m_bodyLayout = new QHBoxLayout(m_body);
    m_bodyLayout->setSpacing(4);
    m_scrollArea = new QScrollArea(m_body);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setStyleSheet(QStringLiteral("QScrollArea { background: transparent; border: none; }"));
    m_scrollArea->viewport()->setAutoFillBackground(false);
    m_content = new QWidget;
    m_content->setFocusPolicy(Qt::ClickFocus);
    m_contentLayout = new QHBoxLayout(m_content);
    m_contentLayout->setContentsMargins(0, 0, 0, 0);
    m_contentLayout->setSpacing(m_tagSpacing);
    m_contentLayout->setSizeConstraint(QLayout::SetMinimumSize);
    m_scrollArea->setWidget(m_content);
    m_content->setAutoFillBackground(false);

    m_editor = new LineEdit(m_content);
    m_editor->setBorderVisible(false);
    m_editor->setContentPadding(0);
    m_editor->setAnimationDuration(0);
    m_editor->setBackgroundNormal(Qt::transparent);
    m_editor->setBackgroundHover(Qt::transparent);
    m_editor->setBackgroundFocus(Qt::transparent);
    m_editor->setBackgroundError(Qt::transparent);
    m_editor->setBackgroundDisabled(Qt::transparent);
    m_editor->setStyleSheet(QStringLiteral("QLineEdit { background: transparent; border: none; }"));
    m_placeholder = new QLabel(m_content);
    m_placeholder->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_placeholder->setForegroundRole(QPalette::PlaceholderText);
    m_contentLayout->addWidget(m_editor);
    m_contentLayout->addWidget(m_placeholder);
    m_contentLayout->addStretch();

    m_clearButton = new HoverIconButton(m_body);
    m_arrowButton = new HoverIconButton(m_body);
    for (auto* button : {m_clearButton, m_arrowButton}) {
        button->setFocusPolicy(Qt::NoFocus);
        button->setFixedSize(24, 24);
        button->setIconSize(QSize(14, 14));
        button->setContentNormal(QColor(QStringLiteral("#64748b")));
        button->setContentHover(QColor(QStringLiteral("#3478f6")));
        button->setBackgroundHover(QColor(QStringLiteral("#e9effb")));
        button->setAnimateContentColor(true);
        button->setCursor(Qt::PointingHandCursor);
    }
    m_clearButton->setIcon(QIcon(QStringLiteral(":/animatedwidgets/inputs/clear.svg")));
    m_clearButton->setAccessibleName(tr("Clear selection"));
    m_arrowButton->setIcon(QIcon(QStringLiteral(":/animatedwidgets/comboboxes/dropdown.svg")));
    m_arrowButton->setAccessibleName(tr("Show options"));
    m_bodyLayout->addWidget(m_scrollArea, 1);
    m_bodyLayout->addWidget(m_clearButton);
    m_bodyLayout->addWidget(m_arrowButton);
    setEditorWidget(m_body);
    setFocusProxy(m_editor);

    for (auto* widget : {static_cast<QWidget*>(m_editor), m_content, m_scrollArea->viewport()})
        widget->installEventFilter(this);
    SmoothScrolling::install(m_scrollArea);
    connect(m_clearButton, &QPushButton::clicked, this, &MultiSelectComboBox::clearSelection);
    connect(m_arrowButton, &QPushButton::clicked, this, [this] {
        if (m_popup)
            hidePopup();
        else
            showPopup();
    });
    connect(m_model, &DefaultPopupMenuModel::selectedIndexesChanged, this, [this](const QList<int>& indexes) {
        refreshTags();
        emit selectedIndexesChanged(indexes);
    });
    connect(m_model, &QAbstractItemModel::modelReset, this, &MultiSelectComboBox::refreshTags);
    connect(m_model, &QAbstractItemModel::dataChanged, this, [this](const QModelIndex&, const QModelIndex&, const QVector<int>& roles) {
        if (roles.size() != 1 || roles.first() != Qt::CheckStateRole)
            refreshTags();
    });
    connect(m_editor, &QLineEdit::textChanged, this, [this](const QString& text) {
        m_filterModel->setSearchText(m_searchable ? text : QString());
        if (m_popup)
            m_popup->view()->setCurrentIndex(QModelIndex());
        updateContentLayout();
        emit searchTextChanged(text);
    });
    connect(m_editor, &QLineEdit::textEdited, this, [this] { showPopup(); });
    connect(m_filterModel, &QAbstractItemModel::modelReset, this, &MultiSelectComboBox::schedulePopupLayout);
    connect(m_filterModel, &QAbstractItemModel::layoutChanged, this, &MultiSelectComboBox::schedulePopupLayout);
    connect(m_filterModel, &QAbstractItemModel::rowsInserted, this, &MultiSelectComboBox::schedulePopupLayout);
    connect(m_filterModel, &QAbstractItemModel::rowsRemoved, this, &MultiSelectComboBox::schedulePopupLayout);
    connect(qApp, &QApplication::focusChanged, this, [this](QWidget*, QWidget* current) {
        if (!current || !isAncestorOf(current))
            hidePopup();
    });
    setPlaceholderText(tr("Select options"));
    updateContentLayout();
}

MultiSelectComboBox::~MultiSelectComboBox() {
    disconnect(qApp, nullptr, this, nullptr);

    // QWidget destroys children after derived members, so detach their callbacks first.
    const auto descendants = findChildren<QObject*>();
    for (auto* child : descendants) {
        child->removeEventFilter(this);
        disconnect(child, nullptr, this, nullptr);
    }

    // Closing popups may still be alive after m_popup was cleared for their hide animation.
    const auto popups = findChildren<PopupMenu*>(QString(), Qt::FindDirectChildrenOnly);
    for (auto* popup : popups)
        delete popup;
}

void MultiSelectComboBox::setStrings(const QStringList& strings) {
    m_model->setStrings(strings);
}

void MultiSelectComboBox::setItems(const QList<PopupMenuItem>& items) {
    m_model->setItems(items);
}

DefaultPopupMenuModel* MultiSelectComboBox::model() const {
    return m_model;
}

PopupMenuFilterModel* MultiSelectComboBox::filterModel() const {
    return m_filterModel;
}

LineEdit* MultiSelectComboBox::editor() const {
    return m_editor;
}

HoverIconButton* MultiSelectComboBox::clearButton() const {
    return m_clearButton;
}

HoverIconButton* MultiSelectComboBox::arrowButton() const {
    return m_arrowButton;
}

PopupMenu* MultiSelectComboBox::currentPopup() const {
    return m_popup;
}

QList<int> MultiSelectComboBox::selectedIndexes() const {
    return m_model->selectedIndexes();
}

void MultiSelectComboBox::setSelectedIndexes(const QList<int>& indexes) {
    m_model->setSelectedIndexes(indexes);
}

bool MultiSelectComboBox::isSearchable() const {
    return m_searchable;
}

void MultiSelectComboBox::setSearchable(bool searchable) {
    if (m_searchable == searchable)
        return;
    QPointer<MultiSelectComboBox> guard(this);
    hidePopup();
    if (!guard)
        return;
    m_searchable = searchable;
    setFocusProxy(searchable ? static_cast<QWidget*>(m_editor) : m_content);
    updateContentLayout();
}

QString MultiSelectComboBox::placeholderText() const {
    return m_editor->placeholderText();
}

void MultiSelectComboBox::setPlaceholderText(const QString& text) {
    m_editor->setPlaceholderText(text);
    m_placeholder->setText(text);
    updateContentLayout();
}

QString MultiSelectComboBox::searchText() const {
    return m_editor->text();
}

void MultiSelectComboBox::setSearchText(const QString& text) {
    m_editor->setText(m_searchable ? text : QString());
}

bool MultiSelectComboBox::iconHint() const {
    return m_iconHint;
}

void MultiSelectComboBox::setIconHint(bool enabled) {
    m_iconHint = enabled;
    m_tagStyle.iconHint = enabled;
    m_clearButton->setIconHint(enabled);
    m_arrowButton->setIconHint(enabled);
    if (m_popup)
        m_popup->setIconHint(enabled);
    refreshTags();
}

int MultiSelectComboBox::contentPadding() const {
    return m_contentPadding;
}

void MultiSelectComboBox::setContentPadding(int padding) {
    m_contentPadding = qMax(0, padding);
    updateContentLayout();
}

int MultiSelectComboBox::tagSpacing() const {
    return m_tagSpacing;
}

void MultiSelectComboBox::setTagSpacing(int spacing) {
    m_tagSpacing = qMax(0, spacing);
    m_contentLayout->setSpacing(m_tagSpacing);
    ensureEditorVisible();
}

TagStyle MultiSelectComboBox::tagStyle() const {
    return m_tagStyle;
}

void MultiSelectComboBox::setTagStyle(const TagStyle& style) {
    m_tagStyle = style;
    refreshTags();
}

void MultiSelectComboBox::setTagStyleFactory(TagStyleFactory factory) {
    m_tagStyleFactory = std::move(factory);
    refreshTags();
}

void MultiSelectComboBox::refreshTags() {
    for (auto* tag : m_tags) {
        m_contentLayout->removeWidget(tag);
        tag->hide();
        tag->deleteLater();
    }
    m_tags.clear();

    for (int itemIndex : selectedIndexes()) {
        const QModelIndex index = m_model->indexForItem(itemIndex);
        auto* tag = new TagWidget(m_content);
        tag->setText(index.data(Qt::DisplayRole).toString());
        tag->setIcon(qvariant_cast<QIcon>(index.data(Qt::DecorationRole)));
        tag->setTagStyle(m_tagStyleFactory ? m_tagStyleFactory(index) : m_tagStyle);
        tag->setEnabled(isEnabled() && index.flags().testFlag(Qt::ItemIsEnabled));
        tag->installEventFilter(this);
        connect(tag, &TagWidget::closeRequested, this, [this, itemIndex] { removeTag(itemIndex); });
        m_contentLayout->insertWidget(static_cast<int>(m_tags.size()), tag, 0, Qt::AlignVCenter);
        m_tags.append(tag);
        tag->show();
    }
    updateContentLayout();
}

QColor MultiSelectComboBox::tagBackground() const {
    return m_tagStyle.background;
}

void MultiSelectComboBox::setTagBackground(const QColor& color) {
    m_tagStyle.background = color;
    refreshTags();
}

QColor MultiSelectComboBox::tagBorder() const {
    return m_tagStyle.border;
}

void MultiSelectComboBox::setTagBorder(const QColor& color) {
    m_tagStyle.border = color;
    refreshTags();
}

QColor MultiSelectComboBox::tagContent() const {
    return m_tagStyle.content;
}

void MultiSelectComboBox::setTagContent(const QColor& color) {
    m_tagStyle.content = color;
    refreshTags();
}

int MultiSelectComboBox::tagCornerRadius() const {
    return m_tagStyle.cornerRadius;
}

void MultiSelectComboBox::setTagCornerRadius(int radius) {
    m_tagStyle.cornerRadius = qMax(0, radius);
    refreshTags();
}

int MultiSelectComboBox::tagBorderWidth() const {
    return m_tagStyle.borderWidth;
}

void MultiSelectComboBox::setTagBorderWidth(int width) {
    m_tagStyle.borderWidth = qMax(0, width);
    refreshTags();
}

int MultiSelectComboBox::tagContentPadding() const {
    return m_tagStyle.contentPadding;
}

void MultiSelectComboBox::setTagContentPadding(int padding) {
    m_tagStyle.contentPadding = qMax(0, padding);
    refreshTags();
}

QSize MultiSelectComboBox::tagIconSize() const {
    return m_tagStyle.iconSize;
}

void MultiSelectComboBox::setTagIconSize(const QSize& size) {
    m_tagStyle.iconSize = QSize(qMax(1, size.width()), qMax(1, size.height()));
    refreshTags();
}

int MultiSelectComboBox::tagIconSpacing() const {
    return m_tagStyle.iconSpacing;
}

void MultiSelectComboBox::setTagIconSpacing(int spacing) {
    m_tagStyle.iconSpacing = qMax(0, spacing);
    refreshTags();
}

bool MultiSelectComboBox::tagCloseButtonVisible() const {
    return m_tagStyle.closeButtonVisible;
}

void MultiSelectComboBox::setTagCloseButtonVisible(bool visible) {
    m_tagStyle.closeButtonVisible = visible;
    refreshTags();
}

QIcon MultiSelectComboBox::tagCloseIcon() const {
    return m_tagStyle.closeIcon;
}

void MultiSelectComboBox::setTagCloseIcon(const QIcon& icon) {
    m_tagStyle.closeIcon = icon;
    refreshTags();
}

QSize MultiSelectComboBox::tagCloseIconSize() const {
    return m_tagStyle.closeIconSize;
}

void MultiSelectComboBox::setTagCloseIconSize(const QSize& size) {
    m_tagStyle.closeIconSize = QSize(qMax(1, size.width()), qMax(1, size.height()));
    refreshTags();
}

int MultiSelectComboBox::tagCloseButtonSize() const {
    return m_tagStyle.closeButtonSize;
}

void MultiSelectComboBox::setTagCloseButtonSize(int size) {
    m_tagStyle.closeButtonSize = qMax(1, size);
    refreshTags();
}

bool MultiSelectComboBox::clearButtonVisible() const {
    return m_clearButtonVisible;
}

void MultiSelectComboBox::setClearButtonVisible(bool visible) {
    m_clearButtonVisible = visible;
    updateContentLayout();
}

bool MultiSelectComboBox::arrowVisible() const {
    return m_arrowVisible;
}

void MultiSelectComboBox::setArrowVisible(bool visible) {
    m_arrowVisible = visible;
    updateContentLayout();
}

QIcon MultiSelectComboBox::arrowIcon() const {
    return m_arrowButton->icon();
}

void MultiSelectComboBox::setArrowIcon(const QIcon& icon) {
    m_arrowButton->setIcon(icon);
}

QSize MultiSelectComboBox::arrowIconSize() const {
    return m_arrowButton->iconSize();
}

void MultiSelectComboBox::setArrowIconSize(const QSize& size) {
    const QSize bounded(qMax(1, size.width()), qMax(1, size.height()));
    m_arrowButton->setIconSize(bounded);
    m_arrowButton->setFixedSize(bounded + QSize(10, 10));
    updateContentLayout();
}

int MultiSelectComboBox::popupWidth() const {
    return m_popupWidth;
}

void MultiSelectComboBox::setPopupWidth(int width) {
    m_popupWidth = qMax(0, width);
    schedulePopupLayout();
}

int MultiSelectComboBox::maxVisibleItems() const {
    return m_maxVisibleItems;
}

void MultiSelectComboBox::setMaxVisibleItems(int count) {
    m_maxVisibleItems = qMax(0, count);
    schedulePopupLayout();
}

QString MultiSelectComboBox::popupScrollBarStyleSheet() const {
    return m_scrollBarStyleSheet;
}

void MultiSelectComboBox::setPopupScrollBarStyleSheet(const QString& styleSheet) {
    m_scrollBarStyleSheet = styleSheet;
    if (m_popup)
        m_popup->setScrollBarStyleSheet(styleSheet);
}

QSize MultiSelectComboBox::sizeHint() const {
    return QSize(280, qMax(36, CompositeInputBase::sizeHint().height()));
}

QSize MultiSelectComboBox::minimumSizeHint() const {
    return QSize(120, sizeHint().height());
}

void MultiSelectComboBox::clearSelection() {
    m_model->setSelectedIndexes({});
}

void MultiSelectComboBox::removeTag(int itemIndex) {
    QList<int> selected = selectedIndexes();
    selected.removeAll(itemIndex);
    setSelectedIndexes(selected);
}

void MultiSelectComboBox::updateContentLayout() {
    m_bodyLayout->setContentsMargins(m_contentPadding, 3, m_contentPadding, 3);
    m_editor->setVisible(m_searchable);
    m_placeholder->setVisible(!m_searchable && m_tags.isEmpty());
    m_clearButton->setVisible(m_clearButtonVisible && !m_tags.isEmpty());
    m_arrowButton->setVisible(m_arrowVisible);

    const QString text = m_editor->text().isEmpty() && m_tags.isEmpty() ? placeholderText() : m_editor->text();
    m_editor->setFixedWidth(qMax(64, m_editor->fontMetrics().horizontalAdvance(text) + 16));
    m_editor->setFixedHeight(qMax(24, m_editor->QLineEdit::sizeHint().height()));
    int height = qMax(m_editor->height(), qMax(m_clearButton->height(), m_arrowButton->height()));
    for (const auto* tag : m_tags)
        height = qMax(height, tag->sizeHint().height());
    m_scrollArea->setFixedHeight(height);
    updateGeometry();
    ensureEditorVisible();
}

void MultiSelectComboBox::ensureEditorVisible() {
    QTimer::singleShot(0, this, [this] {
        m_contentLayout->activate();
        if (m_searchable)
            m_scrollArea->ensureWidgetVisible(m_editor, 4, 0);
        else if (!m_tags.isEmpty())
            m_scrollArea->ensureWidgetVisible(m_tags.last(), 4, 0);
    });
}

void MultiSelectComboBox::showPopup() {
    if (!isEnabled() || m_popup || m_model->rowCount() == 0)
        return;
    (m_searchable ? static_cast<QWidget*>(m_editor) : m_content)->setFocus(Qt::MouseFocusReason);

    auto* popup = new PopupMenu(this);
    m_popup = popup;
    popup->setWindowFlags((popup->windowFlags() & ~Qt::WindowFlags(Qt::WindowType_Mask)) | Qt::Tool | Qt::WindowDoesNotAcceptFocus);
    popup->setAttribute(Qt::WA_ShowWithoutActivating);
    popup->setFocusProxy(nullptr);
    popup->setFocusPolicy(Qt::NoFocus);
    popup->view()->setFocusPolicy(Qt::NoFocus);
    popup->setAutoClose(false);
    popup->setModel(m_filterModel);
    auto* style = new DefaultPopupMenuListStyle;
    style->checkedIcon = QIcon(QStringLiteral(":/animatedwidgets/comboboxes/checked.svg"));
    style->checkedIconAlignment = Qt::AlignRight;
    popup->setListStyle(style);
    popup->setIconHint(m_iconHint);
    if (!m_scrollBarStyleSheet.isNull())
        popup->setScrollBarStyleSheet(m_scrollBarStyleSheet);

    m_emptyLabel = new QLabel(tr("No results"), popup);
    m_emptyLabel->setAlignment(Qt::AlignCenter);
    m_emptyLabel->setMinimumHeight(64);
    popup->setBottomWidget(m_emptyLabel);
    auto* dismiss = new PopupDismissFilter(popup, this);
    connect(dismiss, &PopupDismissFilter::dismissRequested, this, &MultiSelectComboBox::hidePopup);
    connect(popup, &BasePopupDlg::closeRequested, dismiss, &PopupDismissFilter::stop);
    connect(popup, &BasePopupDlg::closeRequested, this, [this, popup] {
        if (m_popup == popup) {
            m_popup = nullptr;
            m_emptyLabel = nullptr;
            m_editor->clear();
        }
    });
    connect(popup, &PopupMenu::itemTriggered, this, [this](const QModelIndex& index) { m_model->triggerItem(m_filterModel->mapToSource(index)); });

    QPointer<MultiSelectComboBox> guard(this);
    emit popupAboutToShow(popup);
    if (!guard || m_popup != popup)
        return;
    updatePopupLayout();
    popup->moveToTargetBottom(this);
    popup->show();
    popup->view()->setCurrentIndex(QModelIndex());
}

void MultiSelectComboBox::hidePopup() {
    if (!m_popup)
        return;
    PopupMenu* popup = m_popup;
    m_popup = nullptr;
    m_emptyLabel = nullptr;
    popup->close();
    m_editor->clear();
}

void MultiSelectComboBox::schedulePopupLayout() {
    if (!m_popup || m_popupLayoutPending)
        return;
    m_popupLayoutPending = true;
    QTimer::singleShot(0, this, [this] {
        m_popupLayoutPending = false;
        updatePopupLayout();
    });
}

void MultiSelectComboBox::updatePopupLayout() {
    if (!m_popup)
        return;
    const bool empty = m_filterModel->rowCount() == 0;
    m_popup->view()->setVisible(!empty);
    if (m_emptyLabel)
        m_emptyLabel->setVisible(empty);
    m_popup->setAdaptiveHeight(m_popupWidth > 0 ? m_popupWidth : width(), m_maxVisibleItems);
}

void MultiSelectComboBox::movePopupSelection(int direction) {
    if (!m_popup)
        return;
    const int count = m_filterModel->rowCount();
    int row = m_popup->view()->currentIndex().row();
    if (row < 0)
        row = direction > 0 ? -1 : count;
    for (int i = 0; i < count; ++i) {
        row = (row + direction + count) % count;
        const QModelIndex index = m_filterModel->index(row, 0);
        if (index.flags().testFlag(Qt::ItemIsEnabled) && index.flags().testFlag(Qt::ItemIsSelectable)) {
            m_popup->view()->setCurrentIndex(index);
            m_popup->view()->scrollTo(index);
            return;
        }
    }
}

bool MultiSelectComboBox::event(QEvent* event) {
    const bool handled = CompositeInputBase::event(event);
    if (m_editor && (event->type() == QEvent::FontChange || event->type() == QEvent::EnabledChange)) {
        if (!isEnabled())
            hidePopup();
        refreshTags();
    }
    return handled;
}

bool MultiSelectComboBox::eventFilter(QObject* watched, QEvent* event) {
    if (event->type() == QEvent::InputMethod && watched == m_editor)
        m_preediting = !static_cast<QInputMethodEvent*>(event)->preeditString().isEmpty();
    if (event->type() == QEvent::Wheel && watched == m_scrollArea->viewport()) {
        const auto* wheel = static_cast<QWheelEvent*>(event);
        if (wheel->modifiers().testFlag(Qt::ControlModifier))
            return false;
        const QPoint delta = wheel->pixelDelta().isNull() ? wheel->angleDelta() / 3 : wheel->pixelDelta();
        auto* bar = m_scrollArea->horizontalScrollBar();
        if (bar->maximum() == 0)
            return false;
        const int target = qBound(bar->minimum(), bar->value() - (delta.x() ? delta.x() : delta.y()), bar->maximum());
        if (target == bar->value())
            return false;
        bar->setValue(target);
        return true;
    }
    if (event->type() == QEvent::MouseButtonRelease && static_cast<QMouseEvent*>(event)->button() == Qt::LeftButton) {
        QTimer::singleShot(0, this, [this] { showPopup(); });
    }
    if (event->type() == QEvent::KeyPress) {
        const auto* key = static_cast<QKeyEvent*>(event);
        if (m_preediting)
            return CompositeInputBase::eventFilter(watched, event);
        if (key->key() == Qt::Key_Escape && m_popup) {
            hidePopup();
            return true;
        }
        if (key->key() == Qt::Key_Down || key->key() == Qt::Key_Up) {
            QPointer<MultiSelectComboBox> guard(this);
            showPopup();
            if (guard)
                movePopupSelection(key->key() == Qt::Key_Down ? 1 : -1);
            return true;
        }
        if ((key->key() == Qt::Key_Return || key->key() == Qt::Key_Enter || (!m_searchable && key->key() == Qt::Key_Space)) && !m_preediting) {
            if (!m_popup) {
                showPopup();
            } else {
                const QModelIndex index = m_popup->view()->currentIndex();
                if (!index.isValid())
                    movePopupSelection(1);
                m_model->triggerItem(m_filterModel->mapToSource(m_popup->view()->currentIndex()));
            }
            return true;
        }
        if (key->key() == Qt::Key_Backspace && m_editor->text().isEmpty() && !m_preediting && key->modifiers() == Qt::NoModifier) {
            const QList<int> selected = selectedIndexes();
            if (!selected.isEmpty())
                removeTag(selected.last());
            return true;
        }
    }
    if (event->type() == QEvent::Resize && watched == m_scrollArea->viewport())
        ensureEditorVisible();
    if (event->type() == QEvent::FocusOut && watched == m_editor)
        m_preediting = false;
    return CompositeInputBase::eventFilter(watched, event);
}

void MultiSelectComboBox::mousePressEvent(QMouseEvent* event) {
    CompositeInputBase::mousePressEvent(event);
    if (event->button() == Qt::LeftButton)
        showPopup();
}

} // namespace animatedwidgets
