#include "calendarstyleplugin.h"

#include "calendarstyleproperties.h"
#include "../utils/styleplugintypeutils.h"
#include "animatedwidgets/calendars/calendardateselector.h"
#include "animatedwidgets/calendars/calendarmonthselector.h"
#include "animatedwidgets/calendars/calendaryearselector.h"
#include "animatedwidgets/calendars/calendartimeselector.h"
#include "animatedwidgets/calendars/calendardateselectpopup.h"
#include "animatedwidgets/calendars/calendardatetimeselectpopup.h"
#include "animatedwidgets/calendars/calendardaterangeselectpopup.h"
#include "animatedwidgets/calendars/calendarmonthselectpopup.h"
#include "animatedwidgets/calendars/calendaryearselectpopup.h"

using animatedwidgets::styleeditor::StyleEditorWidgetType;
using animatedwidgets::styleeditor::StyleWidgetTypeOptions;

QString CalendarStylePlugin::pluginId() const {
    return QStringLiteral("animatedwidgets.calendars");
}

QString CalendarStylePlugin::pluginName() const {
    return QStringLiteral("Animated Widgets Calendars");
}

QList<StyleEditorWidgetType> CalendarStylePlugin::widgetTypes() const {
    StyleWidgetTypeOptions calendarDateSelector;
    calendarDateSelector.id = QStringLiteral("calendardateselector");
    calendarDateSelector.name = QStringLiteral("Calendar Date Selector");
    calendarDateSelector.category = QStringLiteral("Calendars");
    calendarDateSelector.categoryOrder = 900.0;
    calendarDateSelector.order = 100.0;
    calendarDateSelector.suggestedSize = QSize(320, 320);
    calendarDateSelector.defineProperties = &CalendarStyleProperties::calendarDateSelector;

    StyleWidgetTypeOptions calendarMonthSelector;
    calendarMonthSelector.id = QStringLiteral("calendarmonthselector");
    calendarMonthSelector.name = QStringLiteral("Calendar Month Selector");
    calendarMonthSelector.category = QStringLiteral("Calendars");
    calendarMonthSelector.categoryOrder = 900.0;
    calendarMonthSelector.order = 200.0;
    calendarMonthSelector.suggestedSize = QSize(320, 320);
    calendarMonthSelector.defineProperties = &CalendarStyleProperties::calendarMonthSelector;

    StyleWidgetTypeOptions calendarYearSelector;
    calendarYearSelector.id = QStringLiteral("calendaryearselector");
    calendarYearSelector.name = QStringLiteral("Calendar Year Selector");
    calendarYearSelector.category = QStringLiteral("Calendars");
    calendarYearSelector.categoryOrder = 900.0;
    calendarYearSelector.order = 300.0;
    calendarYearSelector.suggestedSize = QSize(320, 320);
    calendarYearSelector.defineProperties = &CalendarStyleProperties::calendarYearSelector;

    StyleWidgetTypeOptions calendarTimeSelector;
    calendarTimeSelector.id = QStringLiteral("calendartimeselector");
    calendarTimeSelector.name = QStringLiteral("Calendar Time Selector");
    calendarTimeSelector.category = QStringLiteral("Calendars");
    calendarTimeSelector.categoryOrder = 900.0;
    calendarTimeSelector.order = 400.0;
    calendarTimeSelector.suggestedSize = QSize(320, 320);
    calendarTimeSelector.defineProperties = &CalendarStyleProperties::calendarTimeSelector;

    StyleWidgetTypeOptions calendarDateSelectPopup;
    calendarDateSelectPopup.id = QStringLiteral("calendardateselectpopup");
    calendarDateSelectPopup.name = QStringLiteral("Calendar Date Select Popup");
    calendarDateSelectPopup.category = QStringLiteral("Calendars");
    calendarDateSelectPopup.categoryOrder = 900.0;
    calendarDateSelectPopup.order = 500.0;
    calendarDateSelectPopup.suggestedSize = QSize(320, 320);
    calendarDateSelectPopup.defineProperties = &CalendarStyleProperties::calendarDateSelectPopup;

    StyleWidgetTypeOptions calendarDateTimeSelectPopup;
    calendarDateTimeSelectPopup.id = QStringLiteral("calendardatetimeselectpopup");
    calendarDateTimeSelectPopup.name = QStringLiteral("Calendar Date Time Select Popup");
    calendarDateTimeSelectPopup.category = QStringLiteral("Calendars");
    calendarDateTimeSelectPopup.categoryOrder = 900.0;
    calendarDateTimeSelectPopup.order = 600.0;
    calendarDateTimeSelectPopup.suggestedSize = QSize(580, 340);
    calendarDateTimeSelectPopup.defineProperties = &CalendarStyleProperties::calendarDateTimeSelectPopup;

    StyleWidgetTypeOptions calendarDateRangeSelectPopup;
    calendarDateRangeSelectPopup.id = QStringLiteral("calendardaterangeselectpopup");
    calendarDateRangeSelectPopup.name = QStringLiteral("Calendar Date Range Select Popup");
    calendarDateRangeSelectPopup.category = QStringLiteral("Calendars");
    calendarDateRangeSelectPopup.categoryOrder = 900.0;
    calendarDateRangeSelectPopup.order = 700.0;
    calendarDateRangeSelectPopup.suggestedSize = QSize(650, 340);
    calendarDateRangeSelectPopup.defineProperties = &CalendarStyleProperties::calendarDateRangeSelectPopup;

    StyleWidgetTypeOptions calendarMonthSelectPopup;
    calendarMonthSelectPopup.id = QStringLiteral("calendarmonthselectpopup");
    calendarMonthSelectPopup.name = QStringLiteral("Calendar Month Select Popup");
    calendarMonthSelectPopup.category = QStringLiteral("Calendars");
    calendarMonthSelectPopup.categoryOrder = 900.0;
    calendarMonthSelectPopup.order = 800.0;
    calendarMonthSelectPopup.suggestedSize = QSize(320, 320);
    calendarMonthSelectPopup.defineProperties = &CalendarStyleProperties::calendarMonthSelectPopup;

    StyleWidgetTypeOptions calendarYearSelectPopup;
    calendarYearSelectPopup.id = QStringLiteral("calendaryearselectpopup");
    calendarYearSelectPopup.name = QStringLiteral("Calendar Year Select Popup");
    calendarYearSelectPopup.category = QStringLiteral("Calendars");
    calendarYearSelectPopup.categoryOrder = 900.0;
    calendarYearSelectPopup.order = 900.0;
    calendarYearSelectPopup.suggestedSize = QSize(320, 320);
    calendarYearSelectPopup.defineProperties = &CalendarStyleProperties::calendarYearSelectPopup;

    return {
        StylePluginTypeUtils::create<animatedwidgets::CalendarDateSelector>(calendarDateSelector),
        StylePluginTypeUtils::create<animatedwidgets::CalendarMonthSelector>(calendarMonthSelector),
        StylePluginTypeUtils::create<animatedwidgets::CalendarYearSelector>(calendarYearSelector),
        StylePluginTypeUtils::create<animatedwidgets::CalendarTimeSelector>(calendarTimeSelector),
        StylePluginTypeUtils::create<animatedwidgets::CalendarDateSelectPopup>(calendarDateSelectPopup),
        StylePluginTypeUtils::create<animatedwidgets::CalendarDateTimeSelectPopup>(calendarDateTimeSelectPopup),
        StylePluginTypeUtils::create<animatedwidgets::CalendarDateRangeSelectPopup>(calendarDateRangeSelectPopup),
        StylePluginTypeUtils::create<animatedwidgets::CalendarMonthSelectPopup>(calendarMonthSelectPopup),
        StylePluginTypeUtils::create<animatedwidgets::CalendarYearSelectPopup>(calendarYearSelectPopup)
    };
}
