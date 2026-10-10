#pragma once

#include <QDate>
#include <QList>

namespace animatedwidgets {

class CalendarDateUtils {
public:
    // Returns a Monday-first six-week grid, including adjacent-month dates.
    static QList<QDate> monthDays(int year, int month);
};

} // namespace animatedwidgets
