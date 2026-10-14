#pragma once

#include "stylepopupdialog.h"

#include "ui_stylemessagepopup.h"

class StyleMessagePopup : public StylePopupDialog {
    Q_OBJECT

public:
    enum class Action { Cancel, Accept, Save, Discard };
    Q_ENUM(Action)

    explicit StyleMessagePopup(const QString& title, const QString& message, QWidget* parent = nullptr);

    void setPrimaryAction(Action action, const QString& text);
    void setSecondaryAction(Action action, const QString& text);
    void setCancelVisible(bool visible);

signals:
    void actionSelected(StyleMessagePopup::Action action);

protected:
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void on_primaryButton_clicked();
    void on_secondaryButton_clicked();
    void on_cancelButton_clicked();

private:
    void createLayout();

    Ui::StyleMessagePopup ui;

    Action m_primaryAction = Action::Accept;
    Action m_secondaryAction = Action::Discard;
};
