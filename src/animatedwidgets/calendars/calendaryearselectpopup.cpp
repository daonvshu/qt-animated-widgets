#include "animatedwidgets/calendars/calendaryearselectpopup.h"

#include "animatedwidgets/calendars/calendaryearselector.h"

#include <QBoxLayout>
#include <QDateTime>
#include <QTimer>

#include "animatedwidgets/utils/calendarstyleutils.h"

namespace animatedwidgets {

CalendarYearSelectPopup::CalendarYearSelectPopup(QWidget* parent)
    : BasePopupDlg(parent)
{
    setObjectName("CalendarYearSelectPopup");

    auto vLayout = new QVBoxLayout(this);
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(0);

    yearSelector = new CalendarYearSelector(this);
    connect(yearSelector, &CalendarYearSelector::selectedYearChanged, this, [this](int year) {
        QTimer::singleShot(1, this, &BasePopupDlg::close);
        emit yearSelected(year);
    });
    vLayout->addWidget(yearSelector);

    resizeContent(280, 41 + 272);
    showArrow(true);
    setCalendarStyle(m_calendarStyle);
}

void CalendarYearSelectPopup::setCurrentYear(int year) const {
    yearSelector->setCurrentSelectedYear(year);
}

/** @brief Returns the current calendar appearance configuration. */
CalendarStyle CalendarYearSelectPopup::calendarStyle() const {
    return m_calendarStyle;
}

/** @brief Applies an appearance configuration to this control and its calendar children. */
void CalendarYearSelectPopup::setCalendarStyle(const CalendarStyle& style) {
    m_calendarStyle = style;
    setStyleSheet(style.popupStyleSheet(objectName()));
    shadowCornerRadius = qMax(0, style.popupCornerRadius);
    shadowColor = style.shadowColor;
    arrowFilledColor = style.popupBackground;
    setShadowRadius(style.shadowVisible ? qMax(0, style.shadowRadius) : 0);
    setShadowEdges(style.shadowVisible ? Qt::Edges(Qt::LeftEdge | Qt::RightEdge | Qt::TopEdge | Qt::BottomEdge) : Qt::Edges());
    showArrow(style.arrowVisible);
    yearSelector->setCalendarStyle(style);
    CalendarStyleUtils::applyChildren(this, style);
    update();
}

} // namespace animatedwidgets
