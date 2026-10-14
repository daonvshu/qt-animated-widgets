#pragma once

#include "workspacepage.h"

#include "ui_styleeditorpage.h"

#include <QPointer>
#include <QSet>

class StyleEditorSession;
class StylePluginRegistry;
class StylePropertyInspector;
class StyleWidgetItemPopup;
class StyleWidgetTreeModel;

class StyleEditorPage : public WorkspacePage {
    Q_OBJECT

public:
    Q_INVOKABLE explicit StyleEditorPage(const QVariant& data, QWidget* parent = nullptr);

    void loadDocument(const StyleDocument& document) override;
    void collectDocument(StyleDocument& document) const override;

private slots:
    void on_stateCombo_currentIndexChanged(int index);

    void on_resetAllButton_clicked();
    void on_copyButton_clicked();

    void on_documentSearchEdit_textChanged();

private:
    void createLayout();
    void connectEditorSignals();
    void on_tree_currentChanged(const QModelIndex& index);
    void on_tree_addRequested(const QString& category);
    void on_tree_editRequested(const QString& itemId);

    void refreshDocumentTree();
    void refreshDocumentPreview();
    void rebuildProperties();
    void rebuildCode();
    void showItemPopup(const QString& category, const QString& itemId = QString());
    void applyItemChanges(const QString& itemId, const QString& typeId, const QString& displayName);

    Ui::StyleEditorPage ui;

    StylePluginRegistry* pluginRegistry = nullptr;
    StyleEditorSession* editorSession = nullptr;
    StylePropertyInspector* propertyInspector = nullptr;
    StyleWidgetTreeModel* treeModel = nullptr;

    QPointer<StyleWidgetItemPopup> itemPopup;
    QSet<QString> collapsedCategories;
    int currentItem = -1;
};

Q_DECLARE_METATYPE(StyleEditorPage*)
