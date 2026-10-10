#include "animatedwidgets/calendars/calendartimeselector.h"
#include "animatedwidgets/calendars/calendartimeitemstyledelegate.h"
#include "animatedwidgets/utils/smoothscrolling.h"

#include <QBoxLayout>
#include <QStandardItemModel>
#include <QScrollBar>

#include "animatedwidgets/utils/calendarstyleutils.h"

namespace animatedwidgets {

CalendarTimeSelector::CalendarTimeSelector(QWidget* parent)
    : QWidget(parent)
{
    m_timeLabelFormatter = [this](const QTime& time) { return time.toString(timeFormat); };
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
    vLayout->addWidget(createTimeSelector());
    vLayout->setStretch(2, 1);

    setCalendarStyle(m_calendarStyle);
}

/**
 * @brief Sets the selected time.
 * @param time
 */
void CalendarTimeSelector::setCurrentTime(const QTime& time) {
    if (!time.isValid() || currentSelectedTime == time)
        return;
    currentSelectedTime = time;
    updateTimeLabel();

    hourSl->setCurrentIndex(hourSl->model()->index(time.hour(), 0));
    minuteSl->setCurrentIndex(minuteSl->model()->index(time.minute(), 0));
    secondSl->setCurrentIndex(secondSl->model()->index(time.second(), 0));
    emit timeChanged(time);
}

/**
 * @brief Sets the display format.
 * @param format
 */
void CalendarTimeSelector::setTimeFormat(const QString& format) {
    timeFormat = format;
    hourSl->setVisible(format.contains("H") || format.contains("h"));
    minuteSl->setVisible(format.contains("m"));
    secondSl->setVisible(format.contains("s"));
    updateTimeLabel();
}

void CalendarTimeSelector::setTimeLabelFormatter(const std::function<QString(const QTime&)>& formatter) {
    m_timeLabelFormatter = formatter ? formatter : [this](const QTime& time) { return time.toString(timeFormat); };
    updateTimeLabel();
}

/**
 * @brief Creates the header.
 * @return
 */
QWidget* CalendarTimeSelector::createTitle() {
    timeLabel = new QLabel("00:00:00", this);
    timeLabel->setFont(m_calendarStyle.titleFont);
    timeLabel->setAlignment(Qt::AlignCenter);
    timeLabel->setMinimumHeight(40);
    return timeLabel;
}

/**
 * @brief Creates the time lists.
 * @return
 */
QWidget* CalendarTimeSelector::createTimeSelector() {
    auto timeSelectorBox = new QWidget(this);

    auto hLayout = new QHBoxLayout(timeSelectorBox);
    hLayout->setContentsMargins(0, 2, 0, 0);
    hLayout->setSpacing(0);

    hourSl = new QListView(timeSelectorBox);
    hourSl->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    hourSl->setMouseTracking(true);
    hourSl->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    SmoothScrolling::install(hourSl);
    hourSl->setEditTriggers(QAbstractItemView::NoEditTriggers);
    hourSl->setItemDelegate(new CalendarTimeItemStyleDelegate(hourSl));
    auto model = new QStandardItemModel(this);
    hourSl->setModel(model);
    for (int i = 0; i < 24; i++) {
        auto item = new QStandardItem(QString("%1").arg(i, 2, 10, QLatin1Char('0')));
        model->appendRow(item);
    }
    hourSl->setCurrentIndex(hourSl->model()->index(currentSelectedTime.hour(), 0));
    connect(hourSl, &QListView::clicked, this, [this](const QModelIndex& index) {
        currentSelectedTime.setHMS(index.data().toInt(), currentSelectedTime.minute(), currentSelectedTime.second());
        updateTimeLabel();
        emit timeChanged(currentSelectedTime);
    });
    hLayout->addWidget(hourSl);

    minuteSl = new QListView(timeSelectorBox);
    minuteSl->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    minuteSl->setMouseTracking(true);
    minuteSl->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    SmoothScrolling::install(minuteSl);
    minuteSl->setEditTriggers(QAbstractItemView::NoEditTriggers);
    minuteSl->setItemDelegate(new CalendarTimeItemStyleDelegate(minuteSl));
    model = new QStandardItemModel(this);
    minuteSl->setModel(model);
    for (int i = 0; i < 60; i++) {
        auto item = new QStandardItem(QString("%1").arg(i, 2, 10, QLatin1Char('0')));
        model->appendRow(item);
    }
    minuteSl->setCurrentIndex(minuteSl->model()->index(currentSelectedTime.minute(), 0));
    connect(minuteSl, &QListView::clicked, this, [this](const QModelIndex& index) {
        currentSelectedTime.setHMS(currentSelectedTime.hour(), index.data().toInt(), currentSelectedTime.second());
        updateTimeLabel();
        emit timeChanged(currentSelectedTime);
    });
    hLayout->addWidget(minuteSl);

    secondSl = new QListView(timeSelectorBox);
    secondSl->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    secondSl->setMouseTracking(true);
    secondSl->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    SmoothScrolling::install(secondSl);
    secondSl->setEditTriggers(QAbstractItemView::NoEditTriggers);
    secondSl->setItemDelegate(new CalendarTimeItemStyleDelegate(secondSl));
    model = new QStandardItemModel(this);
    secondSl->setModel(model);
    for (int i = 0; i < 60; i++) {
        auto item = new QStandardItem(QString("%1").arg(i, 2, 10, QLatin1Char('0')));
        model->appendRow(item);
    }
    secondSl->setCurrentIndex(secondSl->model()->index(currentSelectedTime.second(), 0));
    connect(secondSl, &QListView::clicked, this, [this](const QModelIndex& index) {
        currentSelectedTime.setHMS(currentSelectedTime.hour(), currentSelectedTime.minute(), index.data().toInt());
        updateTimeLabel();
        emit timeChanged(currentSelectedTime);
    });
    hLayout->addWidget(secondSl);

    return timeSelectorBox;
}

void CalendarTimeSelector::updateTimeLabel() const {
    timeLabel->setText(m_timeLabelFormatter(currentSelectedTime));
}

/** @brief Returns the currently selected time. */
QTime CalendarTimeSelector::currentTime() const {
    return currentSelectedTime;
}

/** @brief Returns the display format used by the time selector. */
QString CalendarTimeSelector::displayTimeFormat() const {
    return timeFormat;
}

/** @brief Returns the current calendar appearance configuration. */
CalendarStyle CalendarTimeSelector::calendarStyle() const {
    return m_calendarStyle;
}

/** @brief Applies an appearance configuration to this control and its calendar children. */
void CalendarTimeSelector::setCalendarStyle(const CalendarStyle& style) {
    m_calendarStyle = style;
    setAttribute(Qt::WA_StyledBackground);
    setProperty("calendarSelector", true);
    setStyleSheet(style.selectorStyleSheet());
    for (auto* list : {hourSl, minuteSl, secondSl}) {
        list->setFont(style.itemFont);
        list->setStyleSheet(style.timeStyleSheet());
        static_cast<CalendarTimeItemStyleDelegate*>(list->itemDelegate())->setCalendarStyle(style);
        list->doItemsLayout();
        list->viewport()->update();
    }
    CalendarStyleUtils::applyChildren(this, style);
    update();
}

} // namespace animatedwidgets
