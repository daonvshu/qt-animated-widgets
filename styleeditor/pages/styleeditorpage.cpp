#include "styleeditorpage.h"

#include "../document/styleeditorsession.h"
#include "../dialogs/stylemessagepopup.h"
#include "../dialogs/stylewidgetitempopup.h"
#include "../panels/stylepropertyinspector.h"
#include "../panels/stylewidgettreeadapter.h"
#include "../panels/stylewidgettreemodel.h"
#include "../plugins/stylepluginregistry.h"

#include "animatedwidgets/utils/smoothscrolling.h"

#include <QApplication>
#include <QClipboard>
#include <QIcon>
#include <QLabel>
#include <QPlainTextEdit>
#include <QSignalBlocker>

StyleEditorPage::StyleEditorPage(const QVariant& data, QWidget* parent)
    : WorkspacePage(data, parent)
    , pluginRegistry(static_cast<StylePluginRegistry*>(data.value<QObject*>()))
{
    Q_ASSERT(pluginRegistry);
    createLayout();
}

void StyleEditorPage::loadDocument(const StyleDocument& document) {
    const QSignalBlocker selectionBlocker(ui.widgetTree->selectionModel());
    propertyInspector->clear();
    ui.previewPanel->setPreview(nullptr);
    editorSession->setResourceSettings(document.resources);
    propertyInspector->setResourceSettings(document.resources);
    editorSession->loadItems(document.items);

    currentItem = -1;
    collapsedCategories.clear();
    refreshDocumentTree();
    refreshDocumentPreview();
}

void StyleEditorPage::collectDocument(StyleDocument& document) const {
    document.items = editorSession->collectItems();
    document.resources = editorSession->resourceSettings();
}

void StyleEditorPage::createLayout() {
    ui.setupUi(this);

    ui.codeEdit->setFont(QFont(QStringLiteral("Cascadia Code"), 10));

    propertyInspector = new StylePropertyInspector(ui.propertyList, ui.propertyList);
    animatedwidgets::SmoothScrolling::install(ui.propertyList);
    editorSession = new StyleEditorSession(*pluginRegistry, ui.previewPanel);

    treeModel = new StyleWidgetTreeModel(ui.widgetTree);
    auto* adapter = new StyleWidgetTreeAdapter;
    ui.widgetTree->setAdapter(adapter, true);
    ui.widgetTree->setModel(treeModel);
    ui.widgetTree->setUniformItemHeight(48);
    ui.widgetTree->setRowSpacing(4);
    ui.widgetTree->setIndentation(18);
    ui.widgetTree->setIndentLeafItems(false);
    ui.widgetTree->setRowGridLinesVisible(false);
    ui.widgetTree->setVisualStateBackgroundVisible(true);
    ui.widgetTree->setVisualStateCornerRadius(8);
    ui.widgetTree->setBranchIconSize(12);
    ui.widgetTree->setExpandedBranchIcon(QIcon(QStringLiteral(":/styleeditor/icons/chevrondown.svg")));
    ui.widgetTree->setCollapsedBranchIcon(QIcon(QStringLiteral(":/styleeditor/icons/chevronright.svg")));
    ui.widgetTree->setFrameShape(QFrame::NoFrame);
    ui.widgetTree->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui.widgetTree->setSelectionMode(viv::VirtualItemView::SelectionMode::SingleSelection);
    ui.widgetTree->setExpansionAnimationEnabled(true);
    ui.widgetTree->setExpansionAnimationDuration(300);
    animatedwidgets::SmoothScrolling::install(ui.widgetTree);

    connect(adapter, &StyleWidgetTreeAdapter::addRequested, this, &StyleEditorPage::on_tree_addRequested);
    connect(adapter, &StyleWidgetTreeAdapter::editRequested, this, &StyleEditorPage::on_tree_editRequested);
    connect(ui.widgetTree->selectionModel(), &QItemSelectionModel::currentChanged, this, &StyleEditorPage::on_tree_currentChanged);
    connect(ui.widgetTree, &viv::VirtualTreeView::collapsed, this, [this](const QModelIndex& index) {
        collapsedCategories.insert(index.data(StyleWidgetTreeModel::CategoryRole).toString());
    });
    connect(ui.widgetTree, &viv::VirtualTreeView::expanded, this, [this](const QModelIndex& index) {
        collapsedCategories.remove(index.data(StyleWidgetTreeModel::CategoryRole).toString());
    });

    connectEditorSignals();
    refreshDocumentTree();
}

void StyleEditorPage::connectEditorSignals() {
    connect(ui.previewPanel, &StylePreviewPanel::dialogRequested, this, &WorkspacePage::dialogRequested);
    connect(ui.previewPanel, &StylePreviewPanel::settingsEdited, this, [this](const StylePreviewSettings& settings) {
        editorSession->setPreviewSettings(currentItem, settings);
    });
    connect(propertyInspector, &StylePropertyInspector::dialogRequested, this, &WorkspacePage::dialogRequested);
    connect(propertyInspector, &StylePropertyInspector::resourceSettingsEdited, this, [this](const StyleResourceSettings& settings) {
        editorSession->setResourceSettings(settings);
        refreshDocumentPreview();
    });
    connect(editorSession, &StyleEditorSession::contentChanged, this, &WorkspacePage::contentChanged);
    connect(editorSession, &StyleEditorSession::propertyValuesChanged, this, [this](int index) {
        if (index == currentItem) {
            ui.codeEdit->clear();
            refreshDocumentPreview();
        }
    });
    connect(ui.previewPanel, &StylePreviewPanel::previewUpdated, this,
            [this](const QString& itemId, const QList<animatedwidgets::styleeditor::StyleProperty>& properties, const QString& code) {
        const int index = editorSession->indexOfItem(itemId);
        if (index < 0 || index != currentItem)
            return;
        const bool initialize = editorSession->items().at(index).properties.isEmpty();
        editorSession->updatePreviewResult(index, properties, code);
        if (initialize)
            rebuildProperties();
        rebuildCode();
    });
    connect(propertyInspector, &StylePropertyInspector::valueEdited, this, [this](const QString& name, const QVariant& value) {
        editorSession->setPropertyValue(currentItem, name, value);
    });
}

void StyleEditorPage::on_tree_currentChanged(const QModelIndex& index) {
    if (index.isValid() && !index.parent().isValid())
        return;

    currentItem = index.isValid() ? index.data(StyleWidgetTreeModel::ItemIndexRole).toInt() : -1;
    const QList<StyleEditorItem>& items = editorSession->items();
    if (currentItem < 0 || currentItem >= items.size()) {
        currentItem = -1;
        ui.selectedControlLabel->setText(QStringLiteral("Current item: None"));
        propertyInspector->clear();
        ui.codeEdit->clear();
        refreshDocumentPreview();
        return;
    }

    ui.selectedControlLabel->setText(QStringLiteral("Current item: %1").arg(items.at(currentItem).displayName));
    refreshDocumentPreview();
    rebuildProperties();
    rebuildCode();
}

void StyleEditorPage::refreshDocumentTree() {
    const QList<StyleEditorItem>& items = editorSession->items();
    const QString selectedId = currentItem >= 0 && currentItem < items.size() ? items.at(currentItem).id : QString();
    const QString filter = ui.documentSearchEdit->text().trimmed();
    const QSignalBlocker selectionBlocker(ui.widgetTree->selectionModel());
    const QSignalBlocker treeBlocker(ui.widgetTree);
    treeModel->rebuild(pluginRegistry->widgetTypes(), editorSession->collectItems(), filter);
    ui.documentCountLabel->setText(QString::number(items.size()));

    QModelIndex selected = treeModel->indexForItem(selectedId);
    for (int row = 0; row < treeModel->rowCount(); ++row) {
        const QModelIndex category = treeModel->index(row, 0);
        const bool expanded = !filter.isEmpty() || !collapsedCategories.contains(category.data(StyleWidgetTreeModel::CategoryRole).toString());
        ui.widgetTree->setExpanded(category, expanded);
        if (!selected.isValid() && expanded && treeModel->rowCount(category) > 0)
            selected = treeModel->index(0, 0, category);
    }

    ui.widgetTree->setCurrentIndex(selected);
    on_tree_currentChanged(selected);
}

void StyleEditorPage::refreshDocumentPreview() {
    const QList<StyleEditorItem>& items = editorSession->items();
    if (currentItem >= 0 && currentItem < items.size()) {
        const StyleEditorItem& item = items.at(currentItem);
        ui.previewPanel->setPreview(&item, editorSession->resourceSettings(), ui.stateCombo->currentIndex());
        return;
    }
    ui.previewPanel->setPreview(nullptr);
}

void StyleEditorPage::on_tree_addRequested(const QString& category) {
    showItemPopup(category);
}

void StyleEditorPage::on_tree_editRequested(const QString& itemId) {
    const int index = editorSession->indexOfItem(itemId);
    if (index < 0)
        return;

    const auto* type = pluginRegistry->findType(editorSession->items().at(index).widgetTypeId);
    showItemPopup(type ? StyleWidgetTreeModel::categoryName(*type) : QString(), itemId);
}

void StyleEditorPage::showItemPopup(const QString& category, const QString& itemId) {
    if (itemPopup)
        return;

    QList<animatedwidgets::styleeditor::StyleEditorWidgetType> types;
    for (const auto& type : pluginRegistry->widgetTypes()) {
        if (category.isEmpty() || StyleWidgetTreeModel::categoryName(type) == category)
            types.append(type);
    }
    if (types.isEmpty())
        return;

    auto* popup = new StyleWidgetItemPopup(category.isEmpty() ? QStringLiteral("Widget styles") : category, types, window());
    itemPopup = popup;
    const int index = editorSession->indexOfItem(itemId);
    if (index >= 0) {
        const StyleEditorItem& item = editorSession->items().at(index);
        popup->setItem(item.widgetTypeId, item.displayName);
    }

    connect(popup, &StyleWidgetItemPopup::itemAccepted, this, [this, itemId](const QString& typeId, const QString& displayName) {
        applyItemChanges(itemId, typeId, displayName);
    });
    emit dialogRequested(popup);
}

void StyleEditorPage::applyItemChanges(const QString& itemId, const QString& typeId, const QString& displayName) {
    propertyInspector->clear();
    int index = -1;
    if (itemId.isEmpty()) {
        index = editorSession->addItem(typeId, displayName);
    } else {
        index = editorSession->indexOfItem(itemId);
        if (!editorSession->updateItem(index, typeId, displayName))
            index = -1;
    }
    if (index < 0) {
        rebuildProperties();
        emit dialogRequested(new StyleMessagePopup(QStringLiteral("Widget style unavailable"),
            QStringLiteral("The selected widget style could not be created. The current document has been preserved."), window()));
        return;
    }

    currentItem = index;

    {
        const QSignalBlocker blocker(ui.documentSearchEdit);
        ui.documentSearchEdit->clear();
    }
    if (const auto* type = pluginRegistry->findType(typeId))
        collapsedCategories.remove(StyleWidgetTreeModel::categoryName(*type));
    refreshDocumentTree();
    ui.widgetTree->scrollTo(treeModel->indexForItem(editorSession->items().at(index).id));
}

void StyleEditorPage::rebuildProperties() {
    const QList<StyleEditorItem>& items = editorSession->items();
    if (currentItem < 0 || currentItem >= items.size()) {
        propertyInspector->clear();
        return;
    }
    propertyInspector->setProperties(items.at(currentItem).properties);
}

void StyleEditorPage::on_stateCombo_currentIndexChanged(int) {
    refreshDocumentPreview();
}

void StyleEditorPage::rebuildCode() {
    const QList<StyleEditorItem>& items = editorSession->items();
    if (currentItem < 0 || currentItem >= items.size())
        return;
    ui.codeEdit->setPlainText(items.at(currentItem).generatedCode);
}

void StyleEditorPage::on_resetAllButton_clicked() {
    editorSession->resetProperties(currentItem);
    rebuildProperties();
}

void StyleEditorPage::on_copyButton_clicked() {
    QApplication::clipboard()->setText(ui.codeEdit->toPlainText());
}

void StyleEditorPage::on_documentSearchEdit_textChanged() {
    refreshDocumentTree();
}
