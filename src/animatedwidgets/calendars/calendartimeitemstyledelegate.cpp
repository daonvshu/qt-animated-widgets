#include "animatedwidgets/calendars/calendartimeitemstyledelegate.h"

#include <QPainter>
#include <QStyleOptionViewItem>

namespace animatedwidgets {

void CalendarTimeItemStyleDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    //draw background
    painter->save();

    painter->setPen(Qt::NoPen);
    if (option.state.testFlag(QStyle::State_Selected)) {
        painter->setBrush(m_calendarStyle.timeSelectedBackground);
    } else if (option.state.testFlag(QStyle::State_MouseOver)) {
        painter->setBrush(m_calendarStyle.timeHoverBackground);
    } else {
        painter->setBrush(m_calendarStyle.timeBackground);
    }
    const QRectF backgroundRect = QRectF(option.rect).marginsRemoved(QMarginsF(m_calendarStyle.timeContentPadding));
    painter->drawRoundedRect(backgroundRect, qMax(0, m_calendarStyle.timeCornerRadius), qMax(0, m_calendarStyle.timeCornerRadius));

    painter->restore();

    painter->save();

    painter->setFont(m_calendarStyle.itemFont);
    painter->setPen(!option.state.testFlag(QStyle::State_Enabled) ? m_calendarStyle.disabledContent
        : option.state.testFlag(QStyle::State_Selected) ? m_calendarStyle.timeSelectedContent : m_calendarStyle.content);
    painter->drawText(backgroundRect, Qt::AlignCenter, index.data(Qt::DisplayRole).toString());

    painter->restore();
    painter->restore();
}

QSize CalendarTimeItemStyleDelegate::sizeHint(const QStyleOptionViewItem&, const QModelIndex&) const {
    return {0, qMax(1, m_calendarStyle.timeItemHeight)};
}

/** @brief Sets the appearance used for subsequent painting. */
void CalendarTimeItemStyleDelegate::setCalendarStyle(const CalendarStyle& style) {
    m_calendarStyle = style;
}

} // namespace animatedwidgets
