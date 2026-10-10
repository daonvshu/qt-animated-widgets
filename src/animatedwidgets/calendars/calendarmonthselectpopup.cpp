#include "animatedwidgets/calendars/calendarmonthselectpopup.h"

#include "animatedwidgets/calendars/calendarmonthselector.h"

#include <QBoxLayout>
#include <QDateTime>
#include <QTimer>

#include "animatedwidgets/utils/calendarstyleutils.h"

namespace animatedwidgets {

CalendarMonthSelectPopup::CalendarMonthSelectPopup(QWidget* parent)
    : BasePopupDlg(parent)
{
    setObjectName("CalendarMonthSelectPopup");

    auto vLayout = new QVBoxLayout(this);
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(0);

    monthSelector = new CalendarMonthSelector(this);
    connect(monthSelector, &CalendarMonthSelector::selectedMonthChanged, this, [this](int month) {
        QTimer::singleShot(1, this, &BasePopupDlg::close);
        emit monthSelected(month);
    });
    vLayout->addWidget(monthSelector);

    resizeContent(280, 41 + 272);
    showArrow(true);
    setCalendarStyle(m_calendarStyle);
}

void CalendarMonthSelectPopup::setCurrentDate(const QDate& date) const {
    monthSelector->setCurrentSelectedMonth(date.year(), date.month());
}

/** @brief Returns the current calendar appearance configuration. */
CalendarStyle CalendarMonthSelectPopup::calendarStyle() const {
    return m_calendarStyle;
}

/** @brief Applies an appearance configuration to this control and its calendar children. */
void CalendarMonthSelectPopup::setCalendarStyle(const CalendarStyle& style) {
    m_calendarStyle = style;
    setStyleSheet(style.popupStyleSheet(objectName()));
    shadowCornerRadius = qMax(0, style.popupCornerRadius);
    shadowColor = style.shadowColor;
    arrowFilledColor = style.popupBackground;
    setShadowRadius(style.shadowVisible ? qMax(0, style.shadowRadius) : 0);
    setShadowEdges(style.shadowVisible ? Qt::Edges(Qt::LeftEdge | Qt::RightEdge | Qt::TopEdge | Qt::BottomEdge) : Qt::Edges());
    showArrow(style.arrowVisible);
    monthSelector->setCalendarStyle(style);
    CalendarStyleUtils::applyChildren(this, style);
    update();
}

} // namespace animatedwidgets
