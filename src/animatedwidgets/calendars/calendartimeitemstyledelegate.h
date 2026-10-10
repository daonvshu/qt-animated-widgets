#pragma once

#include "animatedwidgets/calendars/calendarstyle.h"

#include <QStyledItemDelegate>

namespace animatedwidgets {

class CalendarTimeItemStyleDelegate : public QStyledItemDelegate {
public:
    /** @brief Sets the appearance used for subsequent painting. */
    void setCalendarStyle(const CalendarStyle& style);

    using QStyledItemDelegate::QStyledItemDelegate;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex& index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex& index) const override;

private:
    CalendarStyle m_calendarStyle;
};

} // namespace animatedwidgets
