#include "styleeditorwindow.h"

#include "document/styledocumentsession.h"
#include "dialogs/stylemessagepopup.h"
#include "navigation/styleworkspacerouter.h"
#include "pages/workspacepage.h"
#include "pages/thememanagerpage.h"
#include "plugins/stylepluginregistry.h"
#include "themes/stylethememanager.h"

#include <QApplication>
#include <QCloseEvent>
#include <QFileDialog>
#include <QSignalBlocker>

StyleEditorWindow::StyleEditorWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle(QStringLiteral("Animated Widgets Style Editor"));
    resize(1600, 920);

    createLayout();
}

void StyleEditorWindow::closeEvent(QCloseEvent* event) {
    if (activePopup) {
        event->ignore();
        activePopup->raise();
        activePopup->activateWindow();
        return;
    }
    if (closeApproved || !documentSession->document().modified) {
        event->accept();
        return;
    }

    event->ignore();
    confirmDiscardChanges([this] {
        closeApproved = true;
        close();
    });
}

void StyleEditorWindow::createLayout() {
    ui.setupUi(this);

    themeManager = new StyleThemeManager(this, ui.centralWidget);

    pluginRegistry = new StylePluginRegistry(this);
    pluginRegistry->load(QApplication::applicationDirPath() + QStringLiteral("/plugins/styleeditor"));

    documentSession = new StyleDocumentSession(this);
    connect(documentSession, &StyleDocumentSession::metadataChanged, this, &StyleEditorWindow::updateDocumentHeader);

    workspaceRouter = new StyleWorkspaceRouter(ui.workspaceStack, pluginRegistry, this);
    connect(workspaceRouter, &StyleWorkspaceRouter::pageCreated, this, &StyleEditorWindow::initializePage);
    connect(workspaceRouter, &StyleWorkspaceRouter::sectionChanged, this, [this](const QString& title, const QString& description) {
        ui.pageTitle->setText(title);
        ui.pageDescription->setText(description);
    });
    workspaceRouter->navigate(StyleWorkspaceRouter::Section::StyleEditor);

    updateDocumentHeader();
}

void StyleEditorWindow::on_styleEditorNavButton_clicked() {
    workspaceRouter->navigate(StyleWorkspaceRouter::Section::StyleEditor);
}

void StyleEditorWindow::on_componentLibraryNavButton_clicked() {
    workspaceRouter->navigate(StyleWorkspaceRouter::Section::ComponentLibrary);
}

void StyleEditorWindow::on_themeManagerNavButton_clicked() {
    workspaceRouter->navigate(StyleWorkspaceRouter::Section::ThemeManager);
}

void StyleEditorWindow::on_exportNavButton_clicked() {
    workspaceRouter->navigate(StyleWorkspaceRouter::Section::ExportProject);
}

void StyleEditorWindow::on_guidelineNavButton_clicked() {
    workspaceRouter->navigate(StyleWorkspaceRouter::Section::DesignGuidelines);
}

void StyleEditorWindow::on_helpNavButton_clicked() {
    workspaceRouter->navigate(StyleWorkspaceRouter::Section::HelpDocumentation);
}

void StyleEditorWindow::on_newDocumentButton_clicked() {
    confirmDiscardChanges([this] {
        documentSession->newDocument();
        reloadPages();
    });
}

void StyleEditorWindow::on_openDocumentButton_clicked() {
    confirmDiscardChanges([this] {
        const QString filePath = QFileDialog::getOpenFileName(this, QStringLiteral("Open style document"),
            documentSession->document().filePath, QStringLiteral("Style documents (*.json)"));
        if (filePath.isEmpty())
            return;

        QString errorMessage;
        if (!documentSession->open(filePath, errorMessage)) {
            showError(QStringLiteral("Open failed"), errorMessage);
            return;
        }
        reloadPages();
    });
}

void StyleEditorWindow::on_saveDocumentButton_clicked() {
    saveDocument();
}

void StyleEditorWindow::on_saveAsDocumentButton_clicked() {
    saveDocumentAs();
}

void StyleEditorWindow::on_documentNameEdit_textChanged(const QString& name) {
    if (documentSession)
        documentSession->setName(name);
}

void StyleEditorWindow::updateDocumentHeader() {
    const StyleDocument& document = documentSession->document();
    {
        const QSignalBlocker blocker(ui.documentNameEdit);
        ui.documentNameEdit->setText(document.name);
    }

    const QString path = document.filePath.isEmpty() ? QStringLiteral("Temporary document") : document.filePath;
    ui.documentPathLabel->setText(path + (document.modified ? QStringLiteral("  ·  Modified") : QString()));
    setWindowTitle(QStringLiteral("%1 - Animated Widgets Style Editor").arg(document.name));
}

void StyleEditorWindow::initializePage(WorkspacePage* page) {
    StyleThemeManager::prepareWidget(page);
    if (auto* themePage = qobject_cast<ThemeManagerPage*>(page)) {
        themePage->setTheme(themeManager->theme());
        connect(themePage, &ThemeManagerPage::themeRequested, themeManager, &StyleThemeManager::setTheme);
        connect(themeManager, &StyleThemeManager::themeChanged, themePage, &ThemeManagerPage::setTheme);
    }

    {
        const QSignalBlocker blocker(page);
        page->loadDocument(documentSession->document());
    }
    connect(page, &WorkspacePage::contentChanged, this, &StyleEditorWindow::on_page_contentChanged);
    connect(page, &WorkspacePage::dialogRequested, this, &StyleEditorWindow::showPopup);
}

void StyleEditorWindow::reloadPages() {
    for (WorkspacePage* page : workspaceRouter->pages()) {
        const QSignalBlocker blocker(page);
        page->loadDocument(documentSession->document());
    }
}

void StyleEditorWindow::on_page_contentChanged() {
    collectPageContent(qobject_cast<WorkspacePage*>(sender()));
}

void StyleEditorWindow::collectPageContent(WorkspacePage* page) {
    if (!page)
        return;

    StyleDocument content = documentSession->document();
    page->collectDocument(content);
    documentSession->updateContent(content);
}

void StyleEditorWindow::saveDocument(const std::function<void(bool)>& completed) {
    const QString filePath = documentSession->document().filePath;
    if (filePath.isEmpty()) {
        saveDocumentAs(completed);
        return;
    }

    const bool saved = writeDocument(filePath);
    if (completed)
        completed(saved);
}

void StyleEditorWindow::saveDocumentAs(const std::function<void(bool)>& completed) {
    QFileDialog dialog(this, QStringLiteral("Save style document"), documentSession->document().filePath,
        QStringLiteral("Style documents (*.json)"));
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setDefaultSuffix(QStringLiteral("json"));
    if (dialog.exec() != QDialog::Accepted || dialog.selectedFiles().isEmpty()) {
        if (completed)
            completed(false);
        return;
    }

    const bool saved = writeDocument(dialog.selectedFiles().first());
    if (completed)
        completed(saved);
}

bool StyleEditorWindow::writeDocument(const QString& filePath) {
    collectPageContent(workspaceRouter->currentPage());

    QString errorMessage;
    if (documentSession->save(filePath, errorMessage))
        return true;
    showError(QStringLiteral("Save failed"), errorMessage);
    return false;
}

void StyleEditorWindow::confirmDiscardChanges(const std::function<void()>& continuation) {
    if (!documentSession->document().modified) {
        continuation();
        return;
    }

    auto* popup = new StyleMessagePopup(QStringLiteral("Unsaved changes"),
        QStringLiteral("Save changes to the current document?"), this);
    popup->setPrimaryAction(StyleMessagePopup::Action::Save, QStringLiteral("Save"));
    popup->setSecondaryAction(StyleMessagePopup::Action::Discard, QStringLiteral("Discard"));
    popup->setCancelVisible(true);
    connect(popup, &StyleMessagePopup::actionSelected, this, [this, popup, continuation](StyleMessagePopup::Action action) {
        if (activePopup == popup)
            activePopup = nullptr;
        if (action == StyleMessagePopup::Action::Discard) {
            continuation();
        } else if (action == StyleMessagePopup::Action::Save) {
            saveDocument([continuation](bool saved) {
                if (saved)
                    continuation();
            });
        }
    });
    showPopup(popup);
}

void StyleEditorWindow::showPopup(StylePopupDialog* popup) {
    activePopup = popup;
    connect(popup, &StylePopupDialog::finished, this, [this, popup] {
        if (activePopup == popup)
            activePopup = nullptr;
    });
    popup->showDialog();
}

void StyleEditorWindow::showError(const QString& title, const QString& message) {
    showPopup(new StyleMessagePopup(title, message, this));
}
