#include "animatedwidgets/calendars/calendaryearselector.h"
#include "animatedwidgets/utils/widgetresources.h"

#include <QDateTime>
#include <QPushButton>

#include "animatedwidgets/buttons/hovericonbutton.h"

#include "animatedwidgets/utils/calendarstyleutils.h"

namespace animatedwidgets {

CalendarYearSelector::CalendarYearSelector(QWidget* parent)
    : QWidget(parent)
{
    WidgetResources::initializeCalendars();
    setCurrentSelectedYear(QDate::currentDate().year());

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
    //Add the year grid.
    vLayout->addWidget(createYearSelector());
    vLayout->setStretch(2, 1);


    reloadYears();
    setCalendarStyle(m_calendarStyle);
}

void CalendarYearSelector::setCurrentSelectedYear(int year) {
    if (year < 1 || year > 9999)
        return;
    currentSelectedYear = year;
    currentFirstYear = qMax(1, year / 10 * 10);
    reloadYears();
}

/** @brief Returns the selected year. */
int CalendarYearSelector::selectedYear() const {
    return currentSelectedYear;
}

/**
 * @brief Creates the header.
 * @return
 */
QWidget* CalendarYearSelector::createTitle() {
    auto title = new QWidget(this);
    title->setMinimumHeight(40);
    auto hBox = new QHBoxLayout(title);
    hBox->setContentsMargins(11, 0, 11, 0);
    hBox->setSpacing(0);

    // left buttons
    auto btnPrevYear = new HoverIconButton(title);
    btnPrevYear->setIcon(QIcon(QStringLiteral(":/animatedwidgets/calendars/yearleft.svg")));
    hBox->addWidget(btnPrevYear);
    connect(btnPrevYear, &QPushButton::clicked, this, [this] {
        currentFirstYear = qMax(1, currentFirstYear - 10);
        reloadYears();
    });

    // date
    yearRangeLabel = new QLabel(title);
    yearRangeLabel->setFont(m_calendarStyle.titleFont);
    yearRangeLabel->setText(tr("%1 - %2").arg(currentFirstYear).arg(currentFirstYear + 9));
    yearRangeLabel->setAlignment(Qt::AlignCenter);
    hBox->addWidget(yearRangeLabel);

    // right buttons
    auto btnNextYear = new HoverIconButton(title);
    btnNextYear->setIcon(QIcon(QStringLiteral(":/animatedwidgets/calendars/yearright.svg")));
    hBox->addWidget(btnNextYear);
    connect(btnNextYear, &QPushButton::clicked, this, [this] {
        currentFirstYear = qMin(9990, currentFirstYear + 10);
        reloadYears();
    });

    hBox->setStretch(1, 1);

    return title;
}

/**
 * @brief Creates the year grid.
 * @return
 */
QWidget* CalendarYearSelector::createYearSelector() {
    auto yearSelectorBox = new QWidget(this);
    selectorLayout = new QGridLayout(yearSelectorBox);
    selectorLayout->setContentsMargins(26, 28, 26, 28);
    selectorLayout->setVerticalSpacing(40);
    selectorLayout->setHorizontalSpacing(24);

    yearButtonGroup = new QButtonGroup(yearSelectorBox);
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 3; c++) {
            int year = currentFirstYear - 1 + r * 3 + c;
            auto btn = new QPushButton(yearSelectorBox);
            btn->setStyleSheet(m_calendarStyle.buttonStyleSheet());
            btn->setCheckable(true);
            btn->setText(QString::number(year));
            btn->setEnabled(year >= currentFirstYear && year <= currentFirstYear + 9 && year <= 9999);
            btn->setChecked(year == currentSelectedYear);
            btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            yearButtonGroup->addButton(btn);
            selectorLayout->addWidget(btn, r, c);
            connect(btn, &QPushButton::clicked, this, [this, btn] {
                currentSelectedYear = btn->text().toInt();
                emit selectedYearChanged(currentSelectedYear);
            });
        }
    }

    return yearSelectorBox;
}

/**
 * @brief Refreshes the displayed decade.
 */
void CalendarYearSelector::reloadYears() const {
    if (selectorLayout == nullptr) {
        return;
    }

    yearButtonGroup->setExclusive(false);
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 3; c++) {
            int year = currentFirstYear - 1 + r * 3 + c;
            auto btn = qobject_cast<QPushButton*>(selectorLayout->itemAtPosition(r, c)->widget());
            btn->setText(QString::number(year));
            btn->setChecked(year == currentSelectedYear);
            btn->setEnabled(year >= currentFirstYear && year <= currentFirstYear + 9 && year <= 9999);
        }
    }
    yearButtonGroup->setExclusive(true);
    yearRangeLabel->setText(tr("%1 - %2").arg(currentFirstYear).arg(currentFirstYear + 9));
}

/** @brief Returns the current calendar appearance configuration. */
CalendarStyle CalendarYearSelector::calendarStyle() const {
    return m_calendarStyle;
}

/** @brief Applies an appearance configuration to this control and its calendar children. */
void CalendarYearSelector::setCalendarStyle(const CalendarStyle& style) {
    m_calendarStyle = style;
    setAttribute(Qt::WA_StyledBackground);
    setProperty("calendarSelector", true);
    setStyleSheet(style.selectorStyleSheet());
    CalendarStyleUtils::applyChildren(this, style);
    update();
}

} // namespace animatedwidgets
