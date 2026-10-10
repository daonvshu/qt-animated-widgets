#pragma once

#include "animatedwidgets/calendars/calendarstyle.h"

#include <QWidget>
#include <QBoxLayout>
#include <QLabel>
#include <QButtonGroup>

namespace animatedwidgets {

/**
 * @brief Year selection widget.
 */
class CalendarYearSelector : public QWidget {
    Q_OBJECT
    Q_PROPERTY(int selectedYear READ selectedYear WRITE setCurrentSelectedYear)

public:
    /** @brief Returns the current calendar appearance configuration. */
    CalendarStyle calendarStyle() const;
    /** @brief Applies an appearance configuration to this control and its calendar children. */
    void setCalendarStyle(const CalendarStyle& style);

    explicit CalendarYearSelector(QWidget* parent = nullptr);

    /** @brief Returns the selected year. */
    int selectedYear() const;
    void setCurrentSelectedYear(int year);

signals:
    void selectedYearChanged(int year);

private:
    CalendarStyle m_calendarStyle;

    QWidget* createTitle();
    QWidget* createYearSelector();

    void reloadYears() const;

private:
    int currentFirstYear = -1;
    int currentSelectedYear = -1;

    QLabel* yearRangeLabel = nullptr;
    QButtonGroup* yearButtonGroup = nullptr;
    QGridLayout* selectorLayout = nullptr;
};

} // namespace animatedwidgets

#ifndef ANIMATEDWIDGETS_DESIGNER_PLUGIN
using CalendarYearSelector = animatedwidgets::CalendarYearSelector;
#endif
