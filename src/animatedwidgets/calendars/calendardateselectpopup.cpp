#include "animatedwidgets/calendars/calendardateselectpopup.h"

#include "animatedwidgets/calendars/calendarmonthselectpopup.h"
#include "animatedwidgets/calendars/calendaryearselectpopup.h"

#include "animatedwidgets/calendars/calendardateselector.h"

#include "animatedwidgets/buttons/hoverbutton.h"

#include <QBoxLayout>

#include "animatedwidgets/utils/calendarstyleutils.h"

namespace animatedwidgets {

CalendarDateSelectPopup::CalendarDateSelectPopup(QWidget* parent)
    : BasePopupDlg(parent)
{
    sectionData = QSharedPointer<CalendarSectionData>::create();

    setObjectName("CalendarDateSelectPopup");

    auto vLayout = new QVBoxLayout(this);
    vLayout->setSpacing(0);
    vLayout->setContentsMargins(0, 0, 0, 0);

    dateSelector = new CalendarDateSelector(this);
    dateSelector->setSectionData(sectionData);
    dateSelector->setCurrentDisplayDate(QDate::currentDate());
    connect(dateSelector, &CalendarDateSelector::dateSelected, this, &CalendarDateSelectPopup::dateSelected);
    connect(dateSelector, &CalendarDateSelector::yearClicked, this, &CalendarDateSelectPopup::showYearSelector);
    connect(dateSelector, &CalendarDateSelector::monthClicked, this, &CalendarDateSelectPopup::showMonthSelector);
    vLayout->addWidget(dateSelector);

    auto vLine = new QFrame(this);
    vLine->setFixedHeight(1);
    vLine->setObjectName(QStringLiteral("calendarSeparator"));
    vLayout->addWidget(vLine);

    auto todayBtn = new HoverButton(this);
    todayBtn->setText(tr("Today"));
    todayBtn->setMinimumHeight(42);
    connect(todayBtn, &HoverButton::clicked, this, [this] {
        sectionData->selectedDate = QDate::currentDate();
        dateSelector->setCurrentDisplayDate(sectionData->selectedDate);
        emit dateSelected(sectionData->selectedDate);
    });
    vLayout->addWidget(todayBtn);

    vLayout->setStretch(0, 1);

    constexpr int dateSelectorWidth = 280;
    constexpr int dateSelectorHeight = 40 + 1 + 226;
    constexpr int actionHeight = 42;
    resizeContent(dateSelectorWidth, dateSelectorHeight + 1 + actionHeight);
    showArrow(true);
    setCalendarStyle(m_calendarStyle);
}

void CalendarDateSelectPopup::setSelectedDate(const QDate& date) const {
    if (!date.isValid()) {
        return;
    }
    sectionData->selectedDate = date;
    dateSelector->setCurrentDisplayDate(date);
}

void CalendarDateSelectPopup::showYearSelector() {
    auto popup = new CalendarYearSelectPopup(this);
    popup->setCalendarStyle(m_calendarStyle);
    popup->setCurrentYear(dateSelector->currentDisplayDate().year());
    popup->moveToTargetBottom(dateSelector->yearTarget(), Qt::AlignLeft, 0);
    popup->show();
    connect(popup, &CalendarYearSelectPopup::yearSelected, this, [this](int year) {
        dateSelector->setCurrentDisplayDate(QDate(year, dateSelector->currentDisplayDate().month(), 1));
    });
}

void CalendarDateSelectPopup::showMonthSelector() {
    auto popup = new CalendarMonthSelectPopup(this);
    popup->setCalendarStyle(m_calendarStyle);
    popup->setCurrentDate(dateSelector->currentDisplayDate());
    popup->moveToTargetBottom(dateSelector->monthTarget(), Qt::AlignLeft, 0);
    popup->show();
    connect(popup, &CalendarMonthSelectPopup::monthSelected, this, [this](int month) {
        dateSelector->setCurrentDisplayDate(QDate(dateSelector->currentDisplayDate().year(), month, 1));
    });
}

/** @brief Returns the current calendar appearance configuration. */
CalendarStyle CalendarDateSelectPopup::calendarStyle() const {
    return m_calendarStyle;
}

/** @brief Applies an appearance configuration to this control and its calendar children. */
void CalendarDateSelectPopup::setCalendarStyle(const CalendarStyle& style) {
    m_calendarStyle = style;
    setStyleSheet(style.popupStyleSheet(objectName()));
    shadowCornerRadius = qMax(0, style.popupCornerRadius);
    shadowColor = style.shadowColor;
    arrowFilledColor = style.popupBackground;
    setShadowRadius(style.shadowVisible ? qMax(0, style.shadowRadius) : 0);
    setShadowEdges(style.shadowVisible ? Qt::Edges(Qt::LeftEdge | Qt::RightEdge | Qt::TopEdge | Qt::BottomEdge) : Qt::Edges());
    showArrow(style.arrowVisible);
    dateSelector->setCalendarStyle(style);
    CalendarStyleUtils::applyChildren(this, style);
    update();
}

} // namespace animatedwidgets
