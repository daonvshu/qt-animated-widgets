#pragma once

#include "animatedwidgets/calendars/calendarstyle.h"

#include "animatedwidgets/popups/basepopupdlg.h"
#include <QDateTime>
#include <QSharedPointer>

namespace animatedwidgets {

class CalendarDateSelectPopup : public BasePopupDlg {
    Q_OBJECT

public:
    /** @brief Returns the current calendar appearance configuration. */
    CalendarStyle calendarStyle() const;
    /** @brief Applies an appearance configuration to this control and its calendar children. */
    void setCalendarStyle(const CalendarStyle& style);

    explicit CalendarDateSelectPopup(QWidget* parent = nullptr);

    void setSelectedDate(const QDate& date) const;

signals:
    void dateSelected(const QDate& date);

private:
    CalendarStyle m_calendarStyle;

    QSharedPointer<struct CalendarSectionData> sectionData;
    class CalendarDateSelector* dateSelector = nullptr;

private:
    void showYearSelector();
    void showMonthSelector();
};

} // namespace animatedwidgets
