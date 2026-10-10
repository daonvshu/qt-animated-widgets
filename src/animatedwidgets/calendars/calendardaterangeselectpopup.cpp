#include "animatedwidgets/calendars/calendardaterangeselectpopup.h"

#include "animatedwidgets/calendars/calendarmonthselectpopup.h"
#include "animatedwidgets/calendars/calendaryearselectpopup.h"

#include "animatedwidgets/calendars/calendardateselector.h"

#include <QBoxLayout>

#include "animatedwidgets/utils/calendarstyleutils.h"

namespace animatedwidgets {

CalendarDateRangeSelectPopup::CalendarDateRangeSelectPopup(QWidget* parent)
    : BasePopupDlg(parent)
{
    sectionData = QSharedPointer<CalendarSectionData>::create();
    sectionData->rangeModeEnabled = true;

    setObjectName("CalendarDateRangeSelectPopup");

    auto hLayout = new QHBoxLayout(this);
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->setSpacing(0);

    dateLeftSelector = new CalendarDateSelector(this);
    dateLeftSelector->setSectionData(sectionData);
    dateLeftSelector->setCurrentDisplayDate(QDate::currentDate());
    connect(dateLeftSelector, &CalendarDateSelector::dateRangeSelected, this, &CalendarDateRangeSelectPopup::selectorRangeChanged);
    connect(dateLeftSelector, &CalendarDateSelector::currentDisplayDateChanged, this, &CalendarDateRangeSelectPopup::selectorDisplayDateChanged);
    connect(dateLeftSelector, &CalendarDateSelector::yearClicked, this, &CalendarDateRangeSelectPopup::showYearSelector);
    connect(dateLeftSelector, &CalendarDateSelector::monthClicked, this, &CalendarDateRangeSelectPopup::showMonthSelector);
    hLayout->addWidget(dateLeftSelector);

    dateRightSelector = new CalendarDateSelector(this);
    dateRightSelector->setSectionData(sectionData);
    dateRightSelector->setCurrentDisplayDate(QDate::currentDate().addMonths(1));
    connect(dateRightSelector, &CalendarDateSelector::dateRangeSelected, this, &CalendarDateRangeSelectPopup::selectorRangeChanged);
    connect(dateRightSelector, &CalendarDateSelector::currentDisplayDateChanged, this, &CalendarDateRangeSelectPopup::selectorDisplayDateChanged);
    connect(dateRightSelector, &CalendarDateSelector::yearClicked, this, &CalendarDateRangeSelectPopup::showYearSelector);
    connect(dateRightSelector, &CalendarDateSelector::monthClicked, this, &CalendarDateRangeSelectPopup::showMonthSelector);
    hLayout->addWidget(dateRightSelector);

    resizeContent(280 * 2, 40 + 1 + 226);
    showArrow(true);
    setCalendarStyle(m_calendarStyle);
}

void CalendarDateRangeSelectPopup::setCurrentRange(const QDate& start, const QDate& end) const {
    if (!start.isValid() || !end.isValid()) {
        return;
    }
    if (start > end) {
        return;
    }
    sectionData->selectFromDate = start;
    sectionData->selectToDate = end;
    dateLeftSelector->setCurrentDisplayDate(start);
    if (start.year() == end.year() && start.month() == end.month()) {
        dateRightSelector->setCurrentDisplayDate(end.addMonths(1)); //Show the next month.
    } else {
        dateRightSelector->setCurrentDisplayDate(end);
    }
}

void CalendarDateRangeSelectPopup::selectorRangeChanged(const QDate& start, const QDate& end) {
    dateLeftSelector->refreshSelection();
    dateRightSelector->refreshSelection();
    if (start.isValid() && end.isValid()) {
        emit dateRangeSelected(start, end);
    }
}

void CalendarDateRangeSelectPopup::selectorDisplayDateChanged(const QDate& date) const {
    if (sender() == dateLeftSelector) {
        if (dateRightSelector->currentDisplayDate() <= date) {
            dateRightSelector->setCurrentDisplayDate(date.addMonths(1));
        }
    } else {
        if (dateLeftSelector->currentDisplayDate() >= date) {
            dateLeftSelector->setCurrentDisplayDate(date.addMonths(-1));
        }
    }
}

void CalendarDateRangeSelectPopup::showYearSelector() {
    auto dateSelector = qobject_cast<CalendarDateSelector*>(sender());
    if (!dateSelector)
        return;
    auto popup = new CalendarYearSelectPopup(this);
    popup->setCalendarStyle(m_calendarStyle);
    popup->setCurrentYear(dateSelector->currentDisplayDate().year());
    popup->moveToTargetBottom(dateSelector->yearTarget(), Qt::AlignLeft, 0);
    popup->show();
    connect(popup, &CalendarYearSelectPopup::yearSelected, this, [dateSelector](int year) {
        dateSelector->setCurrentDisplayDate(QDate(year, dateSelector->currentDisplayDate().month(), 1));
    });
}

void CalendarDateRangeSelectPopup::showMonthSelector() {
    auto dateSelector = qobject_cast<CalendarDateSelector*>(sender());
    if (!dateSelector)
        return;
    auto popup = new CalendarMonthSelectPopup(this);
    popup->setCalendarStyle(m_calendarStyle);
    popup->setCurrentDate(dateSelector->currentDisplayDate());
    popup->moveToTargetBottom(dateSelector->monthTarget(), Qt::AlignLeft, 0);
    popup->show();
    connect(popup, &CalendarMonthSelectPopup::monthSelected, this, [dateSelector](int month) {
        dateSelector->setCurrentDisplayDate(QDate(dateSelector->currentDisplayDate().year(), month, 1));
    });
}

/** @brief Returns the current calendar appearance configuration. */
CalendarStyle CalendarDateRangeSelectPopup::calendarStyle() const {
    return m_calendarStyle;
}

/** @brief Applies an appearance configuration to this control and its calendar children. */
void CalendarDateRangeSelectPopup::setCalendarStyle(const CalendarStyle& style) {
    m_calendarStyle = style;
    setStyleSheet(style.popupStyleSheet(objectName()));
    shadowCornerRadius = qMax(0, style.popupCornerRadius);
    shadowColor = style.shadowColor;
    arrowFilledColor = style.popupBackground;
    setShadowRadius(style.shadowVisible ? qMax(0, style.shadowRadius) : 0);
    setShadowEdges(style.shadowVisible ? Qt::Edges(Qt::LeftEdge | Qt::RightEdge | Qt::TopEdge | Qt::BottomEdge) : Qt::Edges());
    showArrow(style.arrowVisible);
    dateLeftSelector->setCalendarStyle(style);
    dateRightSelector->setCalendarStyle(style);
    CalendarStyleUtils::applyChildren(this, style);
    update();
}

} // namespace animatedwidgets
