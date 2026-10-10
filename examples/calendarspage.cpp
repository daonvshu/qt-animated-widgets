#include "calendarspage.h"
#include "ui_calendarspage.h"

#include "animatedwidgets/calendars/calendardaterangeselectpopup.h"
#include "animatedwidgets/calendars/calendardateselectpopup.h"
#include "animatedwidgets/calendars/calendardatetimeselectpopup.h"
#include "animatedwidgets/calendars/calendarmonthselectpopup.h"
#include "animatedwidgets/calendars/calendaryearselectpopup.h"

#include <QDateTime>

CalendarsPage::CalendarsPage(QWidget* parent)
    : QWidget(parent)
{
    Ui::CalendarsPage ui;
    ui.setupUi(this);

    setupSelectors(ui);
    setupPopups(ui);
}

void CalendarsPage::setupSelectors(const Ui::CalendarsPage& ui) {
    auto rangeStyle = ui.rangeSelector->calendarStyle();
    rangeStyle.dateSelectedBackground = QColor(QStringLiteral("#16856b"));
    rangeStyle.dateRangeBackground = QColor(QStringLiteral("#dff5eb"));
    rangeStyle.todayBorder = rangeStyle.dateSelectedBackground;
    rangeStyle.buttonSelectedBackground = rangeStyle.dateSelectedBackground;
    rangeStyle.navigationHoverContent = rangeStyle.dateSelectedBackground;
    ui.rangeSelector->setCalendarStyle(rangeStyle);

    ui.rangeSelector->setRangeMode(true);
    ui.rangeSelector->setSelectedRange(QDate::currentDate(), QDate::currentDate().addDays(4));
    ui.timeSelector->setCurrentTime(QTime(9, 30));

    connect(ui.dateSelector, &animatedwidgets::CalendarDateSelector::dateSelected, this,
            [label = ui.resultLabel](const QDate& date) {
        label->setText(QStringLiteral("Date: %1").arg(date.toString(Qt::ISODate)));
    });
    connect(ui.rangeSelector, &animatedwidgets::CalendarDateSelector::dateRangeSelected, this,
            [label = ui.resultLabel](const QDate& from, const QDate& to) {
        label->setText(QStringLiteral("Range: %1 / %2").arg(from.toString(Qt::ISODate), to.toString(Qt::ISODate)));
    });
    connect(ui.yearSelector, &animatedwidgets::CalendarYearSelector::selectedYearChanged, this,
            [label = ui.resultLabel](int year) {
        label->setText(QStringLiteral("Year: %1").arg(year));
    });
    connect(ui.monthSelector, &animatedwidgets::CalendarMonthSelector::selectedMonthChanged, this,
            [label = ui.resultLabel](int month) {
        label->setText(QStringLiteral("Month: %1").arg(month));
    });
    connect(ui.timeSelector, &animatedwidgets::CalendarTimeSelector::timeChanged, this,
            [label = ui.resultLabel](const QTime& time) {
        label->setText(QStringLiteral("Time: %1").arg(time.toString(Qt::ISODate)));
    });

    for (auto* selector : {ui.dateSelector, ui.rangeSelector}) {
        connect(selector, &animatedwidgets::CalendarDateSelector::yearClicked, this, [selector] {
            auto* popup = new animatedwidgets::CalendarYearSelectPopup(selector);
            popup->setCalendarStyle(selector->calendarStyle());
            popup->setCurrentYear(selector->currentDisplayDate().year());
            connect(popup, &animatedwidgets::CalendarYearSelectPopup::yearSelected, selector, [selector](int year) {
                selector->setCurrentDisplayDate(QDate(year, selector->currentDisplayDate().month(), 1));
            });
            popup->moveToTargetBottom(selector->yearTarget());
            popup->show();
        });
        connect(selector, &animatedwidgets::CalendarDateSelector::monthClicked, this, [selector] {
            auto* popup = new animatedwidgets::CalendarMonthSelectPopup(selector);
            popup->setCalendarStyle(selector->calendarStyle());
            popup->setCurrentDate(selector->currentDisplayDate());
            connect(popup, &animatedwidgets::CalendarMonthSelectPopup::monthSelected, selector, [selector](int month) {
                selector->setCurrentDisplayDate(QDate(selector->currentDisplayDate().year(), month, 1));
            });
            popup->moveToTargetBottom(selector->monthTarget());
            popup->show();
        });
    }

    connect(ui.disabledCheck, &QCheckBox::toggled, this, [ui](bool disabled) {
        ui.selectorsTab->setEnabled(!disabled);
        ui.datePopupButton->setEnabled(!disabled);
        ui.rangePopupButton->setEnabled(!disabled);
        ui.dateTimePopupButton->setEnabled(!disabled);
    });
    connect(ui.secondsCheck, &QCheckBox::toggled, this, [selector = ui.timeSelector](bool visible) {
        selector->setTimeFormat(visible ? QStringLiteral("HH:mm:ss") : QStringLiteral("HH:mm"));
    });
}

void CalendarsPage::setupPopups(const Ui::CalendarsPage& ui) {
    connect(ui.datePopupButton, &QPushButton::clicked, this, [this, ui] {
        auto* popup = new animatedwidgets::CalendarDateSelectPopup(this);
        popup->setCalendarStyle(ui.dateSelector->calendarStyle());
        popup->setSelectedDate(ui.dateSelector->selectedDate());
        connect(popup, &animatedwidgets::CalendarDateSelectPopup::dateSelected, this, [ui, popup](const QDate& date) {
            ui.dateSelector->setSelectedDate(date);
            ui.resultLabel->setText(QStringLiteral("Date: %1").arg(date.toString(Qt::ISODate)));
            popup->close();
        });
        popup->moveToTargetBottom(ui.datePopupButton);
        popup->show();
    });
    connect(ui.rangePopupButton, &QPushButton::clicked, this, [this, ui] {
        auto* popup = new animatedwidgets::CalendarDateRangeSelectPopup(this);
        popup->setCalendarStyle(ui.rangeSelector->calendarStyle());
        popup->setCurrentRange(QDate::currentDate(), QDate::currentDate().addDays(4));
        connect(popup, &animatedwidgets::CalendarDateRangeSelectPopup::dateRangeSelected, this,
                [ui, popup](const QDate& from, const QDate& to) {
            ui.rangeSelector->setSelectedRange(from, to);
            ui.resultLabel->setText(QStringLiteral("Range: %1 / %2").arg(from.toString(Qt::ISODate), to.toString(Qt::ISODate)));
            popup->close();
        });
        popup->moveToTargetBottom(ui.rangePopupButton);
        popup->show();
    });
    connect(ui.dateTimePopupButton, &QPushButton::clicked, this, [this, ui] {
        auto* popup = new animatedwidgets::CalendarDateTimeSelectPopup(this);
        popup->setCalendarStyle(ui.dateSelector->calendarStyle());
        popup->setSelectedDatetime(QDateTime(ui.dateSelector->selectedDate(), ui.timeSelector->currentTime()));
        popup->setTimeFormat(ui.secondsCheck->isChecked() ? QStringLiteral("HH:mm:ss") : QStringLiteral("HH:mm"));
        connect(popup, &animatedwidgets::CalendarDateTimeSelectPopup::datetimeSelected, this, [ui](const QDateTime& value) {
            ui.dateSelector->setSelectedDate(value.date());
            ui.timeSelector->setCurrentTime(value.time());
            ui.resultLabel->setText(QStringLiteral("Date/time: %1").arg(value.toString(Qt::ISODate)));
        });
        popup->moveToTargetBottom(ui.dateTimePopupButton);
        popup->show();
    });
}
