#pragma once

#include "stylepopupdialog.h"

#include "animatedwidgets/styleeditor/api/styleeditorplugin.h"

#include "ui_stylewidgetitempopup.h"

class StyleWidgetItemPopup : public StylePopupDialog {
    Q_OBJECT

public:
    explicit StyleWidgetItemPopup(const QString& category,
        const QList<animatedwidgets::styleeditor::StyleEditorWidgetType>& types, QWidget* parent = nullptr);

    void setItem(const QString& typeId, const QString& displayName);

signals:
    void itemAccepted(const QString& typeId, const QString& displayName);

private slots:
    void on_typeCombo_currentIndexChanged(int index);
    void on_nameEdit_textChanged();
    void on_nameEdit_returnPressed();
    void on_acceptButton_clicked();
    void on_cancelButton_clicked();

private:
    void createLayout();

    Ui::StyleWidgetItemPopup ui;

    QString m_defaultName;
    QString m_typeId;
    QString m_displayName;
};
