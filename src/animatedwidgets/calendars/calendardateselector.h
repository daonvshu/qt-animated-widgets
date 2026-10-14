#pragma once

#include "animatedwidgets/namespacealias.h"

#include "animatedwidgets/calendars/calendarstyle.h"

#include <QWidget>
#include <QDateTime>
#include <QTableView>
#include <QStyledItemDelegate>
#include <QStandardItemModel>
#include <QSharedPointer>
#include <QPointer>

#include <functional>

#include "animatedwidgets/calendars/calendarsectiondata.h"

namespace animatedwidgets {

class HoverButton;

/**
 * @brief Date selection widget.
 */
class CalendarDateSelector : public QWidget {
    Q_OBJECT
    Q_PROPERTY(QDate selectedDate READ selectedDate WRITE setSelectedDate)
    Q_PROPERTY(QDate displayDate READ currentDisplayDate WRITE setCurrentDisplayDate NOTIFY currentDisplayDateChanged)
    Q_PROPERTY(bool rangeMode READ rangeMode WRITE setRangeMode)

public:
    /** @brief Returns the current calendar appearance configuration. */
    CalendarStyle calendarStyle() const;
    /** @brief Applies an appearance configuration to this control and its calendar children. */
    void setCalendarStyle(const CalendarStyle& style);

    explicit CalendarDateSelector(QWidget* parent = nullptr);

    void setSectionData(const QSharedPointer<CalendarSectionData>& data);

    /** @brief Sets the seven weekday header labels in Monday-first order. */
    void setWeekdayLabels(const QStringList& labels);
    /** @brief Returns the current weekday header labels. */
    QStringList weekdayLabels() const;
    /** @brief Sets the displayed year title formatter. */
    void setYearLabelFormatter(const std::function<QString(const QDate&)>& formatter);
    /** @brief Sets the displayed month title formatter. */
    void setMonthLabelFormatter(const std::function<QString(const QDate&)>& formatter);

    /** @brief Returns the selected date. */
    QDate selectedDate() const;
    /** @brief Sets the selected date without activating it. */
    void setSelectedDate(const QDate& date);

    /** @brief Returns whether range selection is enabled. */
    bool rangeMode() const;
    /** @brief Enables single-date or range selection. */
    void setRangeMode(bool enabled);
    /** @brief Sets an ordered date range, or clears it when both dates are invalid. */
    void setSelectedRange(const QDate& from, const QDate& to);
    /** @brief Repaints shared selection changes without rebuilding the month. */
    void refreshSelection();

    /**
     * @brief Sets the displayed month.
     * @param date
     */
    void setCurrentDisplayDate(const QDate& date);

    /**
     * @brief Returns the displayed month.
     * @return
     */
    QDate currentDisplayDate() const;

    /**
     * @brief Returns the year label's bottom-left screen position.
     * @return
     */
    QPoint yearGlobalPos() const;
    const QWidget* yearTarget() const;

    /**
     * @brief Returns the month label's bottom-left screen position.
     * @return
     */
    QPoint monthGlobalPos() const;
    const QWidget* monthTarget() const;

signals:
    void dateSelected(const QDate& date);
    void dateRangeSelected(const QDate& from, const QDate& to);
    void currentDisplayDateChanged(const QDate& date);
    void yearClicked();
    void monthClicked(int curYear);

private:
    CalendarStyle m_calendarStyle;

    QWidget* createTitle();
    QWidget* createDateSelector();
    void testCurrentSelected(QDate dateFromItem);
    void updateCurrentDate() const;

private:
    QSharedPointer<CalendarSectionData> sectionData;
    QDate curDisplayDate = QDate::currentDate();
    QStringList m_weekdayLabels;
    std::function<QString(const QDate&)> m_yearLabelFormatter;
    std::function<QString(const QDate&)> m_monthLabelFormatter;

    QStandardItemModel* model = nullptr;
    class CalendarDateItemDelegate* dateItemDelegate = nullptr;
    QTableView* dateTable = nullptr;
    HoverButton* dateYearLabel = nullptr;
    HoverButton* dateMonthLabel = nullptr;
};

} // namespace animatedwidgets

#ifndef ANIMATEDWIDGETS_DESIGNER_PLUGIN
using CalendarDateSelector = animatedwidgets::CalendarDateSelector;
#endif
