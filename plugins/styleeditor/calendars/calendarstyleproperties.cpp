#include "calendarstyleproperties.h"

#include "animatedwidgets/calendars/calendardaterangeselectpopup.h"
#include "animatedwidgets/calendars/calendardateselector.h"
#include "animatedwidgets/calendars/calendardateselectpopup.h"
#include "animatedwidgets/calendars/calendardatetimeselectpopup.h"
#include "animatedwidgets/calendars/calendarmonthselector.h"
#include "animatedwidgets/calendars/calendarmonthselectpopup.h"
#include "animatedwidgets/calendars/calendartimeselector.h"
#include "animatedwidgets/calendars/calendaryearselector.h"
#include "animatedwidgets/calendars/calendaryearselectpopup.h"

#include "../utils/stylepluginproperties.h"

using animatedwidgets::styleeditor::StylePropertyBuilder;

void CalendarStyleProperties::calendarDateSelector(StylePropertyBuilder& builder) {
    appearance<animatedwidgets::CalendarDateSelector>(builder);
    navigation<animatedwidgets::CalendarDateSelector>(builder);
    dateCells<animatedwidgets::CalendarDateSelector>(builder);
    builder.add(QStringLiteral("rangeMode"), QStringLiteral("Range selection"));
}

void CalendarStyleProperties::calendarMonthSelector(StylePropertyBuilder& builder) {
    appearance<animatedwidgets::CalendarMonthSelector>(builder);
    navigation<animatedwidgets::CalendarMonthSelector>(builder);
}

void CalendarStyleProperties::calendarYearSelector(StylePropertyBuilder& builder) {
    appearance<animatedwidgets::CalendarYearSelector>(builder);
    navigation<animatedwidgets::CalendarYearSelector>(builder);
}

void CalendarStyleProperties::calendarTimeSelector(StylePropertyBuilder& builder) {
    appearance<animatedwidgets::CalendarTimeSelector>(builder);
    timeCells<animatedwidgets::CalendarTimeSelector>(builder);
    builder.add(QStringLiteral("timeFormat"), QStringLiteral("Time format"));
}

void CalendarStyleProperties::calendarDateSelectPopup(StylePropertyBuilder& builder) {
    appearance<animatedwidgets::CalendarDateSelectPopup>(builder);
    navigation<animatedwidgets::CalendarDateSelectPopup>(builder);
    dateCells<animatedwidgets::CalendarDateSelectPopup>(builder);
    popup<animatedwidgets::CalendarDateSelectPopup>(builder);
}

void CalendarStyleProperties::calendarDateTimeSelectPopup(StylePropertyBuilder& builder) {
    appearance<animatedwidgets::CalendarDateTimeSelectPopup>(builder);
    navigation<animatedwidgets::CalendarDateTimeSelectPopup>(builder);
    dateCells<animatedwidgets::CalendarDateTimeSelectPopup>(builder);
    timeCells<animatedwidgets::CalendarDateTimeSelectPopup>(builder);
    popup<animatedwidgets::CalendarDateTimeSelectPopup>(builder);
}

void CalendarStyleProperties::calendarDateRangeSelectPopup(StylePropertyBuilder& builder) {
    appearance<animatedwidgets::CalendarDateRangeSelectPopup>(builder);
    navigation<animatedwidgets::CalendarDateRangeSelectPopup>(builder);
    dateCells<animatedwidgets::CalendarDateRangeSelectPopup>(builder);
    popup<animatedwidgets::CalendarDateRangeSelectPopup>(builder);
}

void CalendarStyleProperties::calendarMonthSelectPopup(StylePropertyBuilder& builder) {
    appearance<animatedwidgets::CalendarMonthSelectPopup>(builder);
    navigation<animatedwidgets::CalendarMonthSelectPopup>(builder);
    popup<animatedwidgets::CalendarMonthSelectPopup>(builder);
}

void CalendarStyleProperties::calendarYearSelectPopup(StylePropertyBuilder& builder) {
    appearance<animatedwidgets::CalendarYearSelectPopup>(builder);
    navigation<animatedwidgets::CalendarYearSelectPopup>(builder);
    popup<animatedwidgets::CalendarYearSelectPopup>(builder);
}
