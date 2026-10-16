#include "thememanagerpage.h"

#include <QSignalBlocker>

ThemeManagerPage::ThemeManagerPage(const QVariant& data, QWidget* parent)
    : WorkspacePage(data, parent)
{
    createLayout();
}

void ThemeManagerPage::createLayout() {
    ui.setupUi(this);
    ui.themeCombo->addItem(QStringLiteral("Light"), QStringLiteral("light"));
    ui.themeCombo->addItem(QStringLiteral("Dark"), QStringLiteral("dark"));
}

void ThemeManagerPage::setTheme(const QString& theme) {
    const QSignalBlocker blocker(ui.themeCombo);
    const int index = ui.themeCombo->findData(theme);
    if (index >= 0)
        ui.themeCombo->setCurrentIndex(index);
}

void ThemeManagerPage::on_themeCombo_activated(int index) {
    if (index >= 0)
        emit themeRequested(ui.themeCombo->itemData(index).toString());
}
