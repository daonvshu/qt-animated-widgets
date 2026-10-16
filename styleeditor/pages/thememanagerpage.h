#pragma once

#include "workspacepage.h"
#include "ui_thememanagerpage.h"

class ThemeManagerPage : public WorkspacePage {
    Q_OBJECT

public:
    Q_INVOKABLE explicit ThemeManagerPage(const QVariant& data, QWidget* parent = nullptr);

    void setTheme(const QString& theme);

signals:
    void themeRequested(const QString& theme);

private slots:
    void on_themeCombo_activated(int index);

private:
    void createLayout();

    Ui::ThemeManagerPage ui;
};

Q_DECLARE_METATYPE(ThemeManagerPage*)
