#pragma once

#include "animatedwidgets/calendars/calendarstyle.h"

#include <QWidget>
#include <QDateTime>
#include <QListView>
#include <QLabel>
#include <QStyledItemDelegate>

#include <functional>

namespace animatedwidgets {

/**
 * @brief Time selection widget.
 */
class CalendarTimeSelector : public QWidget {
    Q_OBJECT
    Q_PROPERTY(QTime time READ currentTime WRITE setCurrentTime NOTIFY timeChanged)
    Q_PROPERTY(QString timeFormat READ displayTimeFormat WRITE setTimeFormat)

public:
    /** @brief Returns the current calendar appearance configuration. */
    CalendarStyle calendarStyle() const;
    /** @brief Applies an appearance configuration to this control and its calendar children. */
    void setCalendarStyle(const CalendarStyle& style);

    explicit CalendarTimeSelector(QWidget* parent = nullptr);

    /** @brief Returns the currently selected time. */
    QTime currentTime() const;
    void setCurrentTime(const QTime& time);

    void setTimeFormat(const QString& format);
    /** @brief Sets the formatter used for the selected time title. */
    void setTimeLabelFormatter(const std::function<QString(const QTime&)>& formatter);
    /** @brief Returns the display format used by the time selector. */
    QString displayTimeFormat() const;

signals:
    void timeChanged(const QTime& time);

private:
    CalendarStyle m_calendarStyle;

    QWidget* createTitle();
    QWidget* createTimeSelector();

    void updateTimeLabel() const;

private:
    QTime currentSelectedTime{ 0, 0, 0 };
    std::function<QString(const QTime&)> m_timeLabelFormatter;
    QLabel* timeLabel = nullptr;
    QListView* hourSl = nullptr;
    QListView* minuteSl = nullptr;
    QListView* secondSl = nullptr;
    QString timeFormat = "HH:mm:ss";
};

} // namespace animatedwidgets

#ifndef ANIMATEDWIDGETS_DESIGNER_PLUGIN
using CalendarTimeSelector = animatedwidgets::CalendarTimeSelector;
#endif
