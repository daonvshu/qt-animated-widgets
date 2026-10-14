#pragma once

#include "ui_styleeditorwindow.h"

#include <QMainWindow>
#include <QPointer>

#include <functional>

class QCloseEvent;

class StyleDocumentSession;
class StylePluginRegistry;
class StylePopupDialog;
class StyleWorkspaceRouter;
class WorkspacePage;

class StyleEditorWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit StyleEditorWindow(QWidget* parent = nullptr);

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void on_styleEditorNavButton_clicked();
    void on_componentLibraryNavButton_clicked();
    void on_themeManagerNavButton_clicked();
    void on_exportNavButton_clicked();
    void on_guidelineNavButton_clicked();
    void on_helpNavButton_clicked();

    void on_newDocumentButton_clicked();
    void on_openDocumentButton_clicked();
    void on_saveDocumentButton_clicked();
    void on_saveAsDocumentButton_clicked();

    void on_documentNameEdit_textChanged(const QString& name);
    void on_page_contentChanged();

private:
    void createLayout();
    void updateDocumentHeader();
    void initializePage(WorkspacePage* page);
    void reloadPages();
    void collectPageContent(WorkspacePage* page);
    void saveDocument(const std::function<void(bool)>& completed = {});
    void saveDocumentAs(const std::function<void(bool)>& completed = {});
    bool writeDocument(const QString& filePath);
    void confirmDiscardChanges(const std::function<void()>& continuation);
    void showPopup(StylePopupDialog* popup);
    void showError(const QString& title, const QString& message);

    Ui::StyleEditorWindow ui;

    StylePluginRegistry* pluginRegistry = nullptr;
    StyleDocumentSession* documentSession = nullptr;
    StyleWorkspaceRouter* workspaceRouter = nullptr;

    QPointer<StylePopupDialog> activePopup;
    bool closeApproved = false;
};
