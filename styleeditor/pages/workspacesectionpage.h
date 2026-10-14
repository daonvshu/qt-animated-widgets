#pragma once

#include "ui_workspacesectionpage.h"

#include "workspacepage.h"

class WorkspaceSectionPage : public WorkspacePage {
    Q_OBJECT

public:
    explicit WorkspaceSectionPage(const QVariant& data, QWidget* parent = nullptr);

    QString title() const;
    QString description() const;

protected:
    void setSectionText(const QString& title, const QString& description);

private:
    void createLayout();

    Ui::WorkspaceSectionPage ui;
};
