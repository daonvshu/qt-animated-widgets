#include "animatedwidgets/utils/calendardateutils.h"

namespace animatedwidgets {

QList<QDate> CalendarDateUtils::monthDays(int year, int month) {
    const QDate first(year, month, 1);
    if (!first.isValid())
        return {};

    QList<QDate> days;
    days.reserve(42);
    const QDate start = first.addDays(1 - first.dayOfWeek());
    for (int day = 0; day < 42; ++day)
        days.append(start.addDays(day));
    return days;
}

} // namespace animatedwidgets
