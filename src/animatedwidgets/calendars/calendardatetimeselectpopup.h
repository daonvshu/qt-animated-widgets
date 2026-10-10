#pragma once

#include "animatedwidgets/calendars/calendarstyle.h"

#include "animatedwidgets/popups/basepopupdlg.h"
#include <QDateTime>
#include <QSharedPointer>
#include <QLabel>

#include <functional>

namespace animatedwidgets {

class CalendarDateTimeSelectPopup : public BasePopupDlg {
    Q_OBJECT

public:
    /** @brief Returns the current calendar appearance configuration. */
    CalendarStyle calendarStyle() const;
    /** @brief Applies an appearance configuration to this control and its calendar children. */
    void setCalendarStyle(const CalendarStyle& style);

    explicit CalendarDateTimeSelectPopup(QWidget* parent = nullptr);

    void setSelectedDatetime(const QDateTime& datetime) const;

    void setDateFormat(const QString& format);

    void setTimeFormat(const QString& format);
    /** @brief Sets a formatter for the selected date/time summary label. */
    void setDateTimeLabelFormatter(const std::function<QString(const QDateTime&)>& formatter);

signals:
    void datetimeSelected(const QDateTime& datetime);

private:
    CalendarStyle m_calendarStyle;

    QSharedPointer<struct CalendarSectionData> sectionData;
    class CalendarDateSelector* dateSelector = nullptr;
    class CalendarTimeSelector* timeSelector = nullptr;
    QLabel* dateTimeLabel = nullptr;

    QString dateFormat = QStringLiteral("yyyy-MM-dd");
    QString timeFormat = QStringLiteral("HH:mm:ss");
    std::function<QString(const QDateTime&)> m_dateTimeLabelFormatter;

private:
    void showYearSelector();
    void showMonthSelector();
    void updateDateTimeLabel() const;
};

} // namespace animatedwidgets
