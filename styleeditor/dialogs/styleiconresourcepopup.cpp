#include "styleiconresourcepopup.h"

#include "../panels/styleicontreeadapter.h"
#include "../panels/styleicontreemodel.h"
#include "../utils/styleresourceutils.h"
#include "animatedwidgets/popups/popupmenu.h"
#include "animatedwidgets/popups/popupmenuitemdelegate.h"
#include "animatedwidgets/popups/popupmenuliststyle.h"
#include "animatedwidgets/popups/popupmenustyle.h"
#include "animatedwidgets/utils/smoothscrolling.h"

#include <QAbstractItemView>
#include <QDir>
#include <QFileDialog>
#include <QFontMetrics>
#include <QIcon>
#include <QItemSelectionModel>
#include <QScrollBar>
#include <QSignalBlocker>
#include <QStyleOptionViewItem>
#include <QTimer>

StyleIconResourcePopup::StyleIconResourcePopup(const StyleResourceSettings& settings, const QString& reference, QWidget* parent)
    : StylePopupDialog(parent)
    , m_settings(settings)
    , m_reference(reference)
{
    createLayout();
    reloadResources();

    connect(this, &StylePopupDialog::finished, this, [this](int result) {
        if (result == 1)
            emit iconAccepted(m_reference);
    });
}

void StyleIconResourcePopup::createLayout() {
    ui.setupUi(this);
    setWindowTitle(QStringLiteral("Choose icon"));
    setFocusProxy(ui.searchEdit);

    m_treeModel = new StyleIconTreeModel(ui.iconTree);
    ui.iconTree->setAdapter(new StyleIconTreeAdapter, true);
    ui.iconTree->setModel(m_treeModel);
    ui.iconTree->setUniformItemHeight(36);
    ui.iconTree->setRowSpacing(2);
    ui.iconTree->setIndentation(18);
    ui.iconTree->setIndentLeafItems(false);
    ui.iconTree->setRowGridLinesVisible(false);
    ui.iconTree->setVisualStateBackgroundVisible(true);
    ui.iconTree->setVisualStateCornerRadius(4);
    ui.iconTree->setHoverBackgroundColor(QColor(QStringLiteral("#f2f3f5")));
    ui.iconTree->setSelectedBackgroundColor(QColor(QStringLiteral("#e3f3ff")));
    ui.iconTree->setBranchIconColor(QColor(QStringLiteral("#7c879a")));
    ui.iconTree->setBranchIconSize(12);
    ui.iconTree->setExpandedBranchIcon(QIcon(QStringLiteral(":/styleeditor/icons/chevrondown.svg")));
    ui.iconTree->setCollapsedBranchIcon(QIcon(QStringLiteral(":/styleeditor/icons/chevronright.svg")));
    ui.iconTree->setFrameShape(QFrame::NoFrame);
    ui.iconTree->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui.iconTree->setSelectionMode(viv::VirtualItemView::SelectionMode::SingleSelection);
    ui.iconTree->setExpansionAnimationEnabled(true);
    ui.iconTree->setExpansionAnimationDuration(300);
    animatedwidgets::SmoothScrolling::install(ui.iconTree);

    connect(ui.iconTree->selectionModel(), &QItemSelectionModel::currentChanged, this, &StyleIconResourcePopup::updateSelection);
    connect(ui.qrcCombo, &animatedwidgets::ComboBox::popupAboutToShow, this, &StyleIconResourcePopup::configureQrcPopup);
}

void StyleIconResourcePopup::configureQrcPopup(animatedwidgets::PopupMenu* popup) {
    auto* view = popup->view();
    view->setTextElideMode(Qt::ElideLeft);
    const auto* delegate = qobject_cast<animatedwidgets::PopupMenuItemDelegate*>(popup->itemDelegate());
    QStyleOptionViewItem option;
    option.initFrom(view);
    option.font = view->font();
    option.fontMetrics = QFontMetrics(option.font);

    int contentWidth = 0;
    for (int row = 0; row < ui.qrcCombo->count(); ++row) {
        const QModelIndex index = ui.qrcCombo->model()->index(row, ui.qrcCombo->modelColumn(), ui.qrcCombo->rootModelIndex());
        contentWidth = qMax(contentWidth, popup->itemDelegate()->sizeHint(option, index).width());
    }
    if (delegate && delegate->drawCheckedIcon && (!delegate->checkedIcon.isNull() || !delegate->uncheckedIcon.isNull()))
        contentWidth += delegate->checkedIconSize.width() + delegate->checkedIconSpacing;

    const QMargins margins = popup->listStyle()->margins() + popup->popupStyle()->margins() + view->contentsMargins();
    int popupWidth = contentWidth + margins.left() + margins.right();
    if (ui.qrcCombo->count() > ui.qrcCombo->maxVisibleItems())
        popupWidth += view->verticalScrollBar()->sizeHint().width();
    ui.qrcCombo->setPopupWidth(qBound(ui.qrcCombo->width(), popupWidth, 420));
}

const StyleResourceSettings& StyleIconResourcePopup::settings() const {
    return m_settings;
}

void StyleIconResourcePopup::on_qrcCombo_activated(int index) {
    if (index < 0)
        return;
    const QString source = ui.qrcCombo->itemData(index).toString();
    QTimer::singleShot(0, this, [this, source] {
        if (source.isEmpty()) {
            refreshQrcChoices();
            chooseQrcFile();
        } else {
            m_settings.source = source;
            reloadResources();
        }
    });
}

void StyleIconResourcePopup::chooseQrcFile() {
    const QString initial = m_settings.source.isEmpty() ? m_settings.projectDirectory
        : QDir::cleanPath(QDir(m_settings.projectDirectory).absoluteFilePath(m_settings.source));
    const QString path = QFileDialog::getOpenFileName(this, QStringLiteral("Choose Qt resource file"), initial, QStringLiteral("Qt resources (*.qrc)"));
    if (path.isEmpty())
        return;

    QString error;
    if (!StyleResourceUtils::selectQrc(m_settings, path, error)) {
        ui.statusLabel->setText(error);
        return;
    }
    reloadResources();
}

void StyleIconResourcePopup::refreshQrcChoices() {
    const QSignalBlocker blocker(ui.qrcCombo);
    m_settings.qrcFiles = StyleResourceUtils::qrcSources(m_settings);
    ui.qrcCombo->clear();
    ui.qrcCombo->addItem(QStringLiteral("Choose QRC file..."));
    for (const QString& source : m_settings.qrcFiles) {
        const QString path = QDir::toNativeSeparators(QDir::cleanPath(QDir(m_settings.projectDirectory).absoluteFilePath(source)));
        ui.qrcCombo->addItem(path, source);
        ui.qrcCombo->setItemData(ui.qrcCombo->count() - 1, path, Qt::ToolTipRole);
    }
    const int current = ui.qrcCombo->findData(m_settings.source);
    ui.qrcCombo->setCurrentIndex(current >= 0 ? current : 0);
    ui.qrcCombo->setToolTip(ui.qrcCombo->currentText());
}

void StyleIconResourcePopup::reloadResources() {
    refreshQrcChoices();
    ui.sourcePathLabel->setText(m_settings.source.isEmpty() ? QStringLiteral("Choose a QRC file")
        : QDir::toNativeSeparators(QDir::cleanPath(QDir(m_settings.projectDirectory).absoluteFilePath(m_settings.source))));
    if (m_settings.qrcFiles.contains(m_settings.source)) {
        m_entries = StyleResourceUtils::scan(m_settings, m_error);
    } else {
        m_entries.clear();
        m_error = QStringLiteral("Choose a QRC file.");
    }
    refreshTree();
}

void StyleIconResourcePopup::refreshTree() {
    const QSignalBlocker blocker(ui.iconTree->selectionModel());
    const QString selected = ui.iconTree->currentIndex().data(StyleIconTreeModel::ReferenceRole).toString();
    m_treeModel->rebuild(m_entries, ui.searchEdit->text());
    ui.iconTree->setExpansionAnimationEnabled(false);
    expandCategories();
    ui.iconTree->setExpansionAnimationEnabled(true);
    const QModelIndex current = m_treeModel->indexForReference(selected.isEmpty() ? m_reference : selected);
    ui.iconTree->setCurrentIndex(current);
    if (current.isValid())
        ui.iconTree->scrollTo(current);

    ui.statusLabel->setText(m_error.isEmpty()
        ? QStringLiteral("%1 icons. Generated paths use :/ QRC references.").arg(m_treeModel->iconCount()) : m_error);
    updateSelection();
}

void StyleIconResourcePopup::expandCategories(const QModelIndex& parent) {
    for (int row = 0; row < m_treeModel->rowCount(parent); ++row) {
        const QModelIndex index = m_treeModel->index(row, 0, parent);
        if (m_treeModel->hasChildren(index)) {
            ui.iconTree->setExpanded(index, true);
            expandCategories(index);
        }
    }
}

void StyleIconResourcePopup::on_searchEdit_textChanged() {
    refreshTree();
}

void StyleIconResourcePopup::updateSelection() {
    const QString reference = ui.iconTree->currentIndex().data(StyleIconTreeModel::ReferenceRole).toString();
    ui.acceptButton->setEnabled(!reference.isEmpty());
    ui.selectionLabel->setText(reference.isEmpty() ? QStringLiteral("Select an icon, or clear the current icon.") : reference);
}

void StyleIconResourcePopup::on_iconTree_activated(const QModelIndex& index) {
    const QString reference = index.data(StyleIconTreeModel::ReferenceRole).toString();
    if (reference.isEmpty())
        return;
    m_reference = reference;
    finish(1);
}

void StyleIconResourcePopup::on_acceptButton_clicked() {
    if (!ui.acceptButton->isEnabled())
        return;
    m_reference = ui.iconTree->currentIndex().data(StyleIconTreeModel::ReferenceRole).toString();
    finish(1);
}

void StyleIconResourcePopup::on_clearButton_clicked() {
    m_reference.clear();
    finish(1);
}

void StyleIconResourcePopup::on_cancelButton_clicked() {
    finish(0);
}
