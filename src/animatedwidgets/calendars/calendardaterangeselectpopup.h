#pragma once

#include "animatedwidgets/calendars/calendarstyle.h"

#include "animatedwidgets/popups/basepopupdlg.h"
#include <QDateTime>
#include <QSharedPointer>

namespace animatedwidgets {

class CalendarDateRangeSelectPopup : public BasePopupDlg {
    Q_OBJECT

public:
    /** @brief Returns the current calendar appearance configuration. */
    CalendarStyle calendarStyle() const;
    /** @brief Applies an appearance configuration to this control and its calendar children. */
    void setCalendarStyle(const CalendarStyle& style);

    explicit CalendarDateRangeSelectPopup(QWidget* parent = nullptr);

    void setCurrentRange(const QDate& start, const QDate& end) const;

signals:
    void dateRangeSelected(const QDate& start, const QDate& end);

private:
    CalendarStyle m_calendarStyle;

    QSharedPointer<struct CalendarSectionData> sectionData;
    class CalendarDateSelector* dateLeftSelector = nullptr;
    class CalendarDateSelector* dateRightSelector = nullptr;

private:
    void selectorRangeChanged(const QDate& start, const QDate& end);
    void selectorDisplayDateChanged(const QDate& date) const;
    void showYearSelector();
    void showMonthSelector();
};

} // namespace animatedwidgets
