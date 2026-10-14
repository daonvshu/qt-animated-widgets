#pragma once

#include "animatedwidgets/namespacealias.h"

#include "animatedwidgets/calendars/calendarstyle.h"

#include <QWidget>
#include <QLabel>
#include <QBoxLayout>

namespace animatedwidgets {

/**
 * @brief Month selection widget.
 */
class CalendarMonthSelector : public QWidget {
    Q_OBJECT

public:
    /** @brief Returns the current calendar appearance configuration. */
    CalendarStyle calendarStyle() const;
    /** @brief Applies an appearance configuration to this control and its calendar children. */
    void setCalendarStyle(const CalendarStyle& style);

    explicit CalendarMonthSelector(QWidget* parent = nullptr);

    /** @brief Returns the displayed year. */
    int displayedYear() const;
    /** @brief Returns the selected month. */
    int selectedMonth() const;
    void setCurrentSelectedMonth(int year, int month);

signals:
    void selectedMonthChanged(int month);

private:
    CalendarStyle m_calendarStyle;

    QWidget* createTitle();
    QWidget* createMonthSelector();

private:
    int currentYear = -1;
    int currentSelectedMonth = -1;

    QLabel* titleYearLabel = nullptr;
    QGridLayout* selectorLayout = nullptr;
};

} // namespace animatedwidgets

#ifndef ANIMATEDWIDGETS_DESIGNER_PLUGIN
using CalendarMonthSelector = animatedwidgets::CalendarMonthSelector;
#endif
