#include "animatedwidgets/calendars/calendardateitemdelegate.h"

#include <QPainter>
#include <QStyleOptionViewItem>

namespace animatedwidgets {

CalendarDateItemDelegate::CalendarDateItemDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
}

void CalendarDateItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    auto dateForItem = index.data(Qt::UserRole + 1).toDate();
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    QRect sectionBox(QPoint(), m_calendarStyle.dateBoxSize.expandedTo(QSize(1, 1)));
    sectionBox.moveCenter(option.rect.center());
    bool isCurrentMonth = dateForItem.year() == curDisplayDate.year() && dateForItem.month() == curDisplayDate.month();

    // Draw the hover state.
    if (option.state.testFlag(QStyle::State_Enabled) && option.state.testFlag(QStyle::State_MouseOver) && isCurrentMonth) {
        painter->save();
        painter->setPen(Qt::NoPen);
        painter->setBrush(m_calendarStyle.dateHoverBackground);
        painter->drawRoundedRect(sectionBox, qMax(0, m_calendarStyle.dateCornerRadius), qMax(0, m_calendarStyle.dateCornerRadius));
        painter->restore();
    }

    // Draw the selection background.
    painter->save();
    if (isCurrentMonth) {
        if (sectionData->rangeModeEnabled && sectionData->rangeValid()) {
            //Draw a complete range.
            if (sectionData->isSelectFrom(dateForItem) && sectionData->isSelectTo(dateForItem)) {
                drawSelectedBox(painter, sectionBox, option.rect, false, false);
            } else if (sectionData->isSelectFrom(dateForItem)) { //Draw the start boundary.
                drawSelectedBox(painter, sectionBox, option.rect, true, false);
            } else if (sectionData->isSelectTo(dateForItem)) { //Draw the end boundary.
                drawSelectedBox(painter, sectionBox, option.rect, false, true);
            } else if (sectionData->isInSelectRange(dateForItem)) {
                auto box = sectionBox;
                box.setLeft(option.rect.left());
                box.setRight(option.rect.right() - 1);
                painter->fillRect(box, m_calendarStyle.dateRangeBackground);
            }
        } else if (sectionData->rangeModeEnabled && (sectionData->isSelectFrom(dateForItem) || sectionData->isSelectTo(dateForItem))) {
            drawSelectedBox(painter, sectionBox, option.rect, false, false);
        } else {
            if (!sectionData->rangeModeEnabled && sectionData->isSelectedDate(dateForItem)) {
                drawSelectedBox(painter, sectionBox, option.rect, false, false);
            }
        }
    }
    painter->restore();

    // Outline today's date.
    painter->save();
    if (isCurrentMonth) {
        if (sectionData->isCurrentDate(dateForItem) && m_calendarStyle.todayBorderWidth > 0) {
            painter->setPen(QPen(m_calendarStyle.todayBorder, qMax(0, m_calendarStyle.todayBorderWidth)));
            painter->drawRoundedRect(QRectF(sectionBox).adjusted(m_calendarStyle.todayBorderWidth / 2.0, m_calendarStyle.todayBorderWidth / 2.0,
                                                       -m_calendarStyle.todayBorderWidth / 2.0, -m_calendarStyle.todayBorderWidth / 2.0),
                                     qMax(0, m_calendarStyle.dateCornerRadius), qMax(0, m_calendarStyle.dateCornerRadius));
        }
    }
    painter->restore();

    // Draw the day number.
    QColor textColor = option.state.testFlag(QStyle::State_Enabled) ? m_calendarStyle.content : m_calendarStyle.disabledContent;
    if (!isCurrentMonth) {
        textColor = m_calendarStyle.adjacentMonthContent;
    } else if (option.state.testFlag(QStyle::State_Enabled)) {
        if (sectionData->rangeModeEnabled) {
            if (sectionData->isSelectFrom(dateForItem) || sectionData->isSelectTo(dateForItem)) {
                textColor = m_calendarStyle.dateSelectedContent;
            }
        } else {
            if (sectionData->isSelectedDate(dateForItem)) {
                textColor = m_calendarStyle.dateSelectedContent;
            }
        }
    }
    painter->setFont(m_calendarStyle.itemFont);
    painter->setPen(textColor);
    painter->drawText(sectionBox, Qt::AlignCenter, QString::number(dateForItem.day()));
    painter->restore();
}

void CalendarDateItemDelegate::drawSelectedBox(QPainter* painter, const QRect& sectionBox, const QRect& dstRect, bool isFromBox, bool isToBox) const {
    const int boxRadius = qMax(0, m_calendarStyle.dateCornerRadius);

    painter->setPen(Qt::NoPen);
    if (!isFromBox && !isToBox) { //Single endpoint.
        //Draw the selected endpoint.
        painter->setBrush(m_calendarStyle.dateSelectedBackground);
        painter->drawRoundedRect(sectionBox, boxRadius, boxRadius);
    } else {
        //Draw the range background.
        painter->setBrush(m_calendarStyle.dateRangeBackground);
        painter->drawRoundedRect(sectionBox, boxRadius, boxRadius); //Draw rounded corners first.
        auto rangeRect = sectionBox;
        if (isFromBox) {
            rangeRect.setLeft(sectionBox.center().x());
            rangeRect.setRight(dstRect.right() - 1);
        } else {
            rangeRect.setLeft(dstRect.left());
            rangeRect.setRight(sectionBox.center().x());
        }
        painter->drawRect(rangeRect); //Extend the background across the cell.
        //Draw the selected endpoint.
        painter->setBrush(m_calendarStyle.dateSelectedBackground);
        painter->drawRoundedRect(sectionBox, boxRadius, boxRadius);
        if (isFromBox) {
            painter->drawRect(sectionBox.adjusted(sectionBox.width() / 2, 0, 0, 0)); //Flatten the inner corners.
        } else {
            painter->drawRect(sectionBox.adjusted(0, 0, -sectionBox.width() / 2, 0)); //Flatten the inner corners.
        }
    }
}

/** @brief Sets the appearance used for subsequent painting. */
void CalendarDateItemDelegate::setCalendarStyle(const CalendarStyle& style) {
    m_calendarStyle = style;
}

} // namespace animatedwidgets
