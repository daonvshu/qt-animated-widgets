#include "animatedwidgets/calendars/calendarmonthselector.h"

#include <QDateTime>
#include <QPushButton>
#include <QButtonGroup>
#include <QVariant>

#include "animatedwidgets/utils/calendarstyleutils.h"

namespace animatedwidgets {

CalendarMonthSelector::CalendarMonthSelector(QWidget* parent)
    : QWidget(parent)
{
    currentYear = QDate::currentDate().year();
    currentSelectedMonth = QDate::currentDate().month();

    auto vLayout = new QVBoxLayout(this);
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(0);
    //Add the header.
    vLayout->addWidget(createTitle());
    //Add a divider.
    auto vLine = new QFrame(this);
    vLine->setFixedHeight(1);
    vLine->setObjectName(QStringLiteral("calendarSeparator"));
    vLayout->addWidget(vLine);
    //Add the selector.
    vLayout->addWidget(createMonthSelector());
    vLayout->setStretch(2, 1);

    setCalendarStyle(m_calendarStyle);
}

void CalendarMonthSelector::setCurrentSelectedMonth(int year, int month) {
    if (!QDate(year, month, 1).isValid())
        return;
    currentSelectedMonth = month;
    currentYear = year;
    //update UI
    titleYearLabel->setText(tr("%1").arg(currentYear));
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 3; c++) {
            if (month == 1 + r * 3 + c) {
                auto btn = qobject_cast<QPushButton*>(selectorLayout->itemAtPosition(r, c)->widget());
                btn->setChecked(true);
                return;
            }
        }
    }
}

/** @brief Returns the displayed year. */
int CalendarMonthSelector::displayedYear() const {
    return currentYear;
}

/** @brief Returns the selected month. */
int CalendarMonthSelector::selectedMonth() const {
    return currentSelectedMonth;
}

/**
 * @brief Creates the header.
 * @return
 */
QWidget* CalendarMonthSelector::createTitle() {
    titleYearLabel = new QLabel(this);
    titleYearLabel->setFont(m_calendarStyle.titleFont);
    titleYearLabel->setText(tr("%1").arg(currentYear));
    titleYearLabel->setAlignment(Qt::AlignCenter);
    titleYearLabel->setMinimumHeight(40);
    return titleYearLabel;
}

/**
 * @brief Creates the month grid.
 * @return
 */
QWidget* CalendarMonthSelector::createMonthSelector() {
    auto monthSelectorBox = new QWidget(this);
    selectorLayout = new QGridLayout(monthSelectorBox);
    selectorLayout->setContentsMargins(26, 28, 26, 28);
    selectorLayout->setVerticalSpacing(40);
    selectorLayout->setHorizontalSpacing(24);

    auto buttonGroup = new QButtonGroup(monthSelectorBox);
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 3; c++) {
            int month = 1 + r * 3 + c;
            auto btn = new QPushButton(monthSelectorBox);
            btn->setStyleSheet(m_calendarStyle.buttonStyleSheet());
            btn->setCheckable(true);
            btn->setText(QString::number(month));
            btn->setChecked(month == currentSelectedMonth);
            btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            buttonGroup->addButton(btn);
            selectorLayout->addWidget(btn, r, c);
            connect(btn, &QPushButton::clicked, this, [this, month] {
                currentSelectedMonth = month;
                emit selectedMonthChanged(currentSelectedMonth);
            });
        }
    }

    return monthSelectorBox;
}

/** @brief Returns the current calendar appearance configuration. */
CalendarStyle CalendarMonthSelector::calendarStyle() const {
    return m_calendarStyle;
}

/** @brief Applies an appearance configuration to this control and its calendar children. */
void CalendarMonthSelector::setCalendarStyle(const CalendarStyle& style) {
    m_calendarStyle = style;
    setAttribute(Qt::WA_StyledBackground);
    setProperty("calendarSelector", true);
    setStyleSheet(style.selectorStyleSheet());
    CalendarStyleUtils::applyChildren(this, style);
    update();
}

} // namespace animatedwidgets
