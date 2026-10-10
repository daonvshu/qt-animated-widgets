#include "buttonspage.h"
#include "calendarspage.h"
#include "comboboxespage.h"
#include "inputspage.h"
#include "popupmenuspage.h"
#include "progresspage.h"
#include "selectionpage.h"
#include "sliderspage.h"
#include "switchespage.h"
#include "ui_gallery.h"

#include <QApplication>
#include <QTabBar>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

#ifdef Q_OS_WIN
    NONCLIENTMETRICSW metrics{};
    metrics.cbSize = sizeof(metrics);
    if (SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, metrics.cbSize, &metrics, 0)) {
        auto font = QApplication::font();
        font.setFamily(QString::fromWCharArray(metrics.lfMenuFont.lfFaceName));
        QApplication::setFont(font);
    }
#endif

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
    addPage(new CalendarsPage(&gallery), QObject::tr("Calendars"));
    addPage(new PopupMenusPage(&gallery), QObject::tr("Popup Menus"));

    QObject::connect(ui.tabs, &QTabBar::currentChanged, ui.stack, &QStackedWidget::setCurrentIndex);
    ui.stack->setCurrentIndex(ui.tabs->currentIndex());
    gallery.show();
    return app.exec();
}
