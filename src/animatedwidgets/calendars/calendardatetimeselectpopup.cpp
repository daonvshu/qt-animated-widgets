#include "animatedwidgets/calendars/calendardatetimeselectpopup.h"

#include "animatedwidgets/calendars/calendarmonthselectpopup.h"
#include "animatedwidgets/calendars/calendaryearselectpopup.h"

#include "animatedwidgets/calendars/calendardateselector.h"
#include "animatedwidgets/calendars/calendartimeselector.h"

#include "animatedwidgets/buttons/filledbutton.h"

#include <QBoxLayout>

#include "animatedwidgets/utils/calendarstyleutils.h"

namespace animatedwidgets {

CalendarDateTimeSelectPopup::CalendarDateTimeSelectPopup(QWidget* parent)
    : BasePopupDlg(parent)
{
    m_dateTimeLabelFormatter = [this](const QDateTime& value) {
        QString text;
        if (!dateFormat.isEmpty())
            text += value.date().toString(dateFormat) + QStringLiteral(" ");
        if (!timeFormat.isEmpty())
            text += value.time().toString(timeFormat);
        return text;
    };
    sectionData = QSharedPointer<CalendarSectionData>::create();

    setObjectName("CalendarDateTimeSelectPopup");

    auto vLayout = new QVBoxLayout(this);
    vLayout->setSpacing(0);
    vLayout->setContentsMargins(0, 0, 0, 0);

    {
        auto hLayout = new QHBoxLayout;
        hLayout->setSpacing(0);
        hLayout->setContentsMargins(0, 0, 0, 0);

        dateSelector = new CalendarDateSelector(this);
        dateSelector->setSectionData(sectionData);
        dateSelector->setCurrentDisplayDate(QDate::currentDate());
        dateSelector->setMinimumWidth(280);
        connect(dateSelector, &CalendarDateSelector::dateSelected, this, [this](const QDate&) {
            updateDateTimeLabel();
        });
        connect(dateSelector, &CalendarDateSelector::yearClicked, this, &CalendarDateTimeSelectPopup::showYearSelector);
        connect(dateSelector, &CalendarDateSelector::monthClicked, this, &CalendarDateTimeSelectPopup::showMonthSelector);
        hLayout->addWidget(dateSelector);

        auto hLine = new QFrame(this);
        hLine->setFixedWidth(1);
        hLine->setObjectName(QStringLiteral("calendarSeparator"));
        hLine->setProperty("calendarVerticalSeparator", true);
        hLayout->addWidget(hLine);

        timeSelector = new CalendarTimeSelector(this);
        timeSelector->setCurrentTime(sectionData->selectedTime);
        connect(timeSelector, &CalendarTimeSelector::timeChanged, this, [this](const QTime& time) {
            sectionData->selectedTime = time;
            updateDateTimeLabel();
        });
        hLayout->addWidget(timeSelector);

        vLayout->addLayout(hLayout);
    }

    auto vLine = new QFrame(this);
    vLine->setFixedHeight(1);
    vLine->setObjectName(QStringLiteral("calendarSeparator"));
    vLayout->addWidget(vLine);

    {
        auto buttonBox = new QWidget(this);
        buttonBox->setStyleSheet("background: transparent;");
        buttonBox->setMinimumHeight(42);
        auto layout = new QHBoxLayout(buttonBox);
        layout->setSpacing(0);
        layout->setContentsMargins(12, 0, 12, 0);

        dateTimeLabel = new QLabel(this);
        dateTimeLabel->setObjectName(QStringLiteral("calendarValueLabel"));
        dateTimeLabel->setFont(m_calendarStyle.itemFont);
        layout->addWidget(dateTimeLabel);

        layout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
        auto confirmBtn = new FilledButton(this);
        connect(confirmBtn, &FilledButton::clicked, this, [this] {
            QPointer<CalendarDateTimeSelectPopup> guard(this);
            emit datetimeSelected(QDateTime(sectionData->selectedDate, sectionData->selectedTime));
            if (guard)
                close();
        });
        confirmBtn->setText(tr("Apply"));
        confirmBtn->setMinimumSize(46, 24);
        layout->addWidget(confirmBtn);

        vLayout->addWidget(buttonBox);
    }

    vLayout->setStretch(0, 1);

    constexpr int dateSelectorWidth = 280;
    constexpr int dateSelectorHeight = 40 + 1 + 226;
    constexpr int timeSelectorWidth = 192;
    constexpr int actionHeight = 42;
    resizeContent(dateSelectorWidth + 1 + timeSelectorWidth, dateSelectorHeight + 1 + actionHeight);
    showArrow(true);
    updateDateTimeLabel();
    setCalendarStyle(m_calendarStyle);
}

void CalendarDateTimeSelectPopup::setSelectedDatetime(const QDateTime& datetime) const {
    if (!datetime.isValid())
        return;
    sectionData->selectedDate = datetime.date();
    sectionData->selectedTime = datetime.time();
    dateSelector->setCurrentDisplayDate(datetime.date());
    timeSelector->setCurrentTime(datetime.time());
    updateDateTimeLabel();
}

void CalendarDateTimeSelectPopup::setDateFormat(const QString& format) {
    dateFormat = format;
    updateDateTimeLabel();
}

void CalendarDateTimeSelectPopup::setTimeFormat(const QString& format) {
    timeFormat = format;
    timeSelector->setTimeFormat(format);
    int col = 3;
    if (!format.contains("H") && !format.contains("h")) {
        col--;
    }
    if (!format.contains("m")) {
        col--;
    }
    if (!format.contains("s")) {
        col--;
    }
    resizeContent(280 + 1 + 64 * col, 40 + 2 + 226 + 42);
    updateDateTimeLabel();
}

void CalendarDateTimeSelectPopup::setDateTimeLabelFormatter(const std::function<QString(const QDateTime&)>& formatter) {
    m_dateTimeLabelFormatter = formatter ? formatter : [this](const QDateTime& value) {
        QString text;
        if (!dateFormat.isEmpty())
            text += value.date().toString(dateFormat) + QStringLiteral(" ");
        if (!timeFormat.isEmpty())
            text += value.time().toString(timeFormat);
        return text;
    };
    updateDateTimeLabel();
}

void CalendarDateTimeSelectPopup::showYearSelector() {
    auto popup = new CalendarYearSelectPopup(this);
    popup->setCalendarStyle(m_calendarStyle);
    popup->setCurrentYear(dateSelector->currentDisplayDate().year());
    popup->moveToTargetBottom(dateSelector->yearTarget(), Qt::AlignLeft, 0);
    popup->show();
    connect(popup, &CalendarYearSelectPopup::yearSelected, this, [this](int year) {
        dateSelector->setCurrentDisplayDate(QDate(year, dateSelector->currentDisplayDate().month(), 1));
    });
}

void CalendarDateTimeSelectPopup::showMonthSelector() {
    auto popup = new CalendarMonthSelectPopup(this);
    popup->setCalendarStyle(m_calendarStyle);
    popup->setCurrentDate(dateSelector->currentDisplayDate());
    popup->moveToTargetBottom(dateSelector->monthTarget(), Qt::AlignLeft, 0);
    popup->show();
    connect(popup, &CalendarMonthSelectPopup::monthSelected, this, [this](int month) {
        dateSelector->setCurrentDisplayDate(QDate(dateSelector->currentDisplayDate().year(), month, 1));
    });
}

void CalendarDateTimeSelectPopup::updateDateTimeLabel() const {
    dateTimeLabel->setText(m_dateTimeLabelFormatter(QDateTime(sectionData->selectedDate, sectionData->selectedTime)));
}

/** @brief Returns the current calendar appearance configuration. */
CalendarStyle CalendarDateTimeSelectPopup::calendarStyle() const {
    return m_calendarStyle;
}

/** @brief Applies an appearance configuration to this control and its calendar children. */
void CalendarDateTimeSelectPopup::setCalendarStyle(const CalendarStyle& style) {
    m_calendarStyle = style;
    setStyleSheet(style.popupStyleSheet(objectName()));
    shadowCornerRadius = qMax(0, style.popupCornerRadius);
    shadowColor = style.shadowColor;
    arrowFilledColor = style.popupBackground;
    setShadowRadius(style.shadowVisible ? qMax(0, style.shadowRadius) : 0);
    setShadowEdges(style.shadowVisible ? Qt::Edges(Qt::LeftEdge | Qt::RightEdge | Qt::TopEdge | Qt::BottomEdge) : Qt::Edges());
    showArrow(style.arrowVisible);
    dateSelector->setCalendarStyle(style);
    timeSelector->setCalendarStyle(style);
    dateTimeLabel->setFont(style.itemFont);
    CalendarStyleUtils::applyChildren(this, style);
    update();
}

} // namespace animatedwidgets
