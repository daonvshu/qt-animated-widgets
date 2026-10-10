#pragma once

#include "animatedwidgets/calendars/calendarstyle.h"

#include "animatedwidgets/popups/basepopupdlg.h"
#include <QDateTime>

namespace animatedwidgets {

class CalendarYearSelectPopup : public BasePopupDlg {
    Q_OBJECT

public:
    /** @brief Returns the current calendar appearance configuration. */
    CalendarStyle calendarStyle() const;
    /** @brief Applies an appearance configuration to this control and its calendar children. */
    void setCalendarStyle(const CalendarStyle& style);

    explicit CalendarYearSelectPopup(QWidget* parent = nullptr);

    void setCurrentYear(int year) const;

signals:
    void yearSelected(int year);

private:
    CalendarStyle m_calendarStyle;

    class CalendarYearSelector* yearSelector = nullptr;
};

} // namespace animatedwidgets
