#pragma once

#include "stylepopupdialog.h"
#include "../document/styleresourceentry.h"
#include "../document/styleresourcesettings.h"
#include "ui_styleiconresourcepopup.h"

class StyleIconTreeModel;

namespace animatedwidgets {
class PopupMenu;
}

class StyleIconResourcePopup : public StylePopupDialog {
    Q_OBJECT

public:
    explicit StyleIconResourcePopup(const StyleResourceSettings& settings, const QString& reference, QWidget* parent = nullptr);
    const StyleResourceSettings& settings() const;

signals:
    void iconAccepted(const QString& reference);

private slots:
    void on_qrcCombo_activated(int index);
    void on_searchEdit_textChanged();
    void on_iconTree_activated(const QModelIndex& index);
    void on_acceptButton_clicked();
    void on_clearButton_clicked();
    void on_cancelButton_clicked();

private:
    void createLayout();
    void configureQrcPopup(animatedwidgets::PopupMenu* popup);
    void chooseQrcFile();
    void refreshQrcChoices();
    void reloadResources();
    void refreshTree();
    void updateSelection();
    void expandCategories(const QModelIndex& parent = QModelIndex());

    Ui::StyleIconResourcePopup ui;
    StyleIconTreeModel* m_treeModel = nullptr;
    StyleResourceSettings m_settings;
    QList<StyleResourceEntry> m_entries;
    QString m_reference;
    QString m_error;
};
