#include "buttonspage.h"
#include "comboboxespage.h"
#include "inputspage.h"
#include "progresspage.h"
#include "selectionpage.h"
#include "sliderspage.h"
#include "switchespage.h"
#include "ui_gallery.h"

#include <QApplication>
#include <QTabBar>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QWidget gallery;
    Ui::Gallery ui;
    ui.setupUi(&gallery);

    const auto addPage = [&ui](QWidget* page, const QString& title) {
        ui.stack->addWidget(page);
        ui.tabs->addTab(title);
    };
    addPage(new ButtonsPage(&gallery), QObject::tr("Buttons"));
    addPage(new SelectionPage(&gallery), QObject::tr("Selection"));
    addPage(new SwitchesPage(&gallery), QObject::tr("Switches"));
    addPage(new ProgressPage(&gallery), QObject::tr("Progress"));
    addPage(new SlidersPage(&gallery), QObject::tr("Sliders"));
    addPage(new InputsPage(&gallery), QObject::tr("Inputs"));
    addPage(new ComboBoxesPage(&gallery), QObject::tr("Combo Boxes"));

    QObject::connect(ui.tabs, &QTabBar::currentChanged, ui.stack, &QStackedWidget::setCurrentIndex);
    ui.stack->setCurrentIndex(ui.tabs->currentIndex());
    gallery.show();
    return app.exec();
}
