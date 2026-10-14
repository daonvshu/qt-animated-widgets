#include "workspacesectionpage.h"

WorkspaceSectionPage::WorkspaceSectionPage(const QVariant& data, QWidget* parent)
    : WorkspacePage(data, parent)
{
    createLayout();
}

void WorkspaceSectionPage::createLayout() {
    ui.setupUi(this);
}

QString WorkspaceSectionPage::title() const {
    return ui.sectionTitle->text();
}

QString WorkspaceSectionPage::description() const {
    return ui.sectionDescription->text();
}

void WorkspaceSectionPage::setSectionText(const QString& title, const QString& description) {
    ui.sectionTitle->setText(title);
    ui.sectionDescription->setText(description);
}
