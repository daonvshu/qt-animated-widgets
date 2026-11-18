#pragma once

#include "animatedwidgets/calendars/calendarstyle.h"

#include "animatedwidgets/calendars/calendarsectiondata.h"

#include <QRectF>
#include <QSharedPointer>
#include <QStyledItemDelegate>

namespace animatedwidgets {

/**
 * @brief Date-cell drawing delegate.
 */
class CalendarDateItemDelegate : public QStyledItemDelegate {
public:
    /** @brief Sets the appearance used for subsequent painting. */
    void setCalendarStyle(const CalendarStyle& style);

    explicit CalendarDateItemDelegate(QObject* parent = nullptr);

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    QSharedPointer<CalendarSectionData> sectionData = QSharedPointer<CalendarSectionData>::create();
    QDate curDisplayDate = QDate::currentDate();

    friend class CalendarDateSelector;

private:
    void drawSelectedBox(QPainter* painter, const QRectF& sectionBox, const QRectF& dstRect, bool isFromBox, bool isToBox) const;

private:
    CalendarStyle m_calendarStyle;
};

} // namespace animatedwidgets
