#pragma once

#include <QDateTime>

namespace animatedwidgets {

struct CalendarSectionData {
    QDate curDate = QDate::currentDate();
    QDate selectedDate = QDate::currentDate();
    QTime selectedTime = QTime::currentTime();
    QDate selectFromDate;
    QDate selectToDate;
    bool rangeModeEnabled = false;

    bool isCurrentDate(const QDate& date) const {
        return date == curDate;
    }

    bool isSelectedDate(const QDate& date) const {
        return date == selectedDate;
    }

    bool isSelectFrom(const QDate& date) const {
        return date == selectFromDate;
    }

    bool isSelectTo(const QDate& date) const {
        return date == selectToDate;
    }

    bool isInSelectRange(const QDate& date) const {
        if (!date.isValid() || !rangeValid())
            return false;
        return date >= selectFromDate && date <= selectToDate;
    }

    bool rangeValid() const {
        return selectFromDate.isValid() && selectToDate.isValid() && selectFromDate <= selectToDate;
    }
};

} // namespace animatedwidgets
