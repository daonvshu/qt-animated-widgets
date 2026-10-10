#pragma once

#include "animatedwidgets/calendars/calendarstyle.h"

#include "animatedwidgets/popups/basepopupdlg.h"
#include <QDateTime>

namespace animatedwidgets {

class CalendarMonthSelectPopup : public BasePopupDlg {
    Q_OBJECT

public:
    /** @brief Returns the current calendar appearance configuration. */
    CalendarStyle calendarStyle() const;
    /** @brief Applies an appearance configuration to this control and its calendar children. */
    void setCalendarStyle(const CalendarStyle& style);

    explicit CalendarMonthSelectPopup(QWidget* parent = nullptr);

    void setCurrentDate(const QDate& date) const;

signals:
    void monthSelected(int month);

private:
    CalendarStyle m_calendarStyle;

    class CalendarMonthSelector* monthSelector = nullptr;
};

} // namespace animatedwidgets
