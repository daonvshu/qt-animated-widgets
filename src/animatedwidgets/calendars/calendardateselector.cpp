#include "animatedwidgets/calendars/calendardateselector.h"
#include "animatedwidgets/calendars/calendardateitemdelegate.h"
#include "animatedwidgets/utils/calendardateutils.h"
#include "animatedwidgets/utils/widgetresources.h"

#include <QBoxLayout>
#include <QHeaderView>
#include <QTableView>

#include "animatedwidgets/buttons/hovericonbutton.h"
#include "animatedwidgets/buttons/hoverbutton.h"

#include "animatedwidgets/utils/calendarstyleutils.h"

namespace animatedwidgets {

CalendarDateSelector::CalendarDateSelector(QWidget* parent)
    : QWidget(parent)
{
    WidgetResources::initializeCalendars();
    sectionData = QSharedPointer<CalendarSectionData>::create();
    m_weekdayLabels = QStringList({QStringLiteral("Mon"), QStringLiteral("Tue"), QStringLiteral("Wed"), QStringLiteral("Thu"),
                                   QStringLiteral("Fri"), QStringLiteral("Sat"), QStringLiteral("Sun")});
    m_yearLabelFormatter = [](const QDate& date) { return QString::number(date.year()); };
    m_monthLabelFormatter = [](const QDate& date) { return QString::number(date.month()); };

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
    //Add the date grid.
    vLayout->addWidget(createDateSelector());
    vLayout->setStretch(2, 1);

    setCurrentDisplayDate(curDisplayDate);
    setCalendarStyle(m_calendarStyle);
}

void CalendarDateSelector::setSectionData(const QSharedPointer<CalendarSectionData>& data) {
    if (!data)
        return;
    sectionData = data;
    dateItemDelegate->sectionData = data;
    refreshSelection();
}

void CalendarDateSelector::setWeekdayLabels(const QStringList& labels) {
    if (labels.size() != 7)
        return;
    m_weekdayLabels = labels;
    if (model)
        model->setHorizontalHeaderLabels(m_weekdayLabels);
}

QStringList CalendarDateSelector::weekdayLabels() const {
    return m_weekdayLabels;
}

void CalendarDateSelector::setYearLabelFormatter(const std::function<QString(const QDate&)>& formatter) {
    m_yearLabelFormatter = formatter ? formatter : [](const QDate& date) { return QString::number(date.year()); };
    updateCurrentDate();
}

void CalendarDateSelector::setMonthLabelFormatter(const std::function<QString(const QDate&)>& formatter) {
    m_monthLabelFormatter = formatter ? formatter : [](const QDate& date) { return QString::number(date.month()); };
    updateCurrentDate();
}

void CalendarDateSelector::setCurrentDisplayDate(const QDate& date) {
    if (!date.isValid())
        return;
    curDisplayDate = QDate(date.year(), date.month(), 1);
    dateItemDelegate->curDisplayDate = date;

    while (model->rowCount() > 0) {
        model->removeRow(0);
    }
    auto days = CalendarDateUtils::monthDays(date.year(), date.month());
    for (int i = 0; i < days.size(); i++) {
        int row = i / 7;
        int col = i % 7;
        auto item = new QStandardItem;
        item->setData(days[i], Qt::UserRole + 1);
        model->setItem(row, col, item);
    }
    updateCurrentDate();
    emit currentDisplayDateChanged(curDisplayDate);
}

QDate CalendarDateSelector::currentDisplayDate() const {
    return curDisplayDate;
}

QPoint CalendarDateSelector::yearGlobalPos() const {
    return dateYearLabel->mapToGlobal(QPoint(0, dateYearLabel->height()));
}

const QWidget* CalendarDateSelector::yearTarget() const {
    return dateYearLabel;
}

QPoint CalendarDateSelector::monthGlobalPos() const {
    return dateMonthLabel->mapToGlobal(QPoint(0, dateMonthLabel->height()));
}

const QWidget* CalendarDateSelector::monthTarget() const {
    return dateMonthLabel;
}

/**
 * @brief Creates the header.
 * @return
 */
QWidget* CalendarDateSelector::createTitle() {
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
        setCurrentDisplayDate(curDisplayDate.addYears(-1));
    });

    auto btnPrevMonth = new HoverIconButton(title);
    btnPrevMonth->setIcon(QIcon(QStringLiteral(":/animatedwidgets/calendars/monthleft.svg")));
    hBox->addWidget(btnPrevMonth);
    connect(btnPrevMonth, &QPushButton::clicked, this, [this] {
        setCurrentDisplayDate(curDisplayDate.addMonths(-1));
    });

    // date
    auto dateBox = new QWidget(title);
    auto hLayout = new QHBoxLayout(dateBox);
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->setSpacing(8);
    hLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    dateYearLabel = new HoverButton(title);
    dateYearLabel->setObjectName(QStringLiteral("calendarDateTitleButton"));
    dateYearLabel->setContentNormal(m_calendarStyle.content);
    dateYearLabel->setFont(m_calendarStyle.titleFont);
    connect(dateYearLabel, &HoverButton::clicked, this, &CalendarDateSelector::yearClicked);
    hLayout->addWidget(dateYearLabel);
    dateMonthLabel = new HoverButton(title);
    dateMonthLabel->setObjectName(QStringLiteral("calendarDateTitleButton"));
    dateMonthLabel->setContentNormal(m_calendarStyle.content);
    dateMonthLabel->setFont(m_calendarStyle.titleFont);
    connect(dateMonthLabel, &HoverButton::clicked, this, [this] {
        emit monthClicked(curDisplayDate.year());
    });
    hLayout->addWidget(dateMonthLabel);
    hLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));

    hBox->addWidget(dateBox);

    // right buttons
    auto btnNextMonth = new HoverIconButton(title);
    btnNextMonth->setIcon(QIcon(QStringLiteral(":/animatedwidgets/calendars/monthright.svg")));
    hBox->addWidget(btnNextMonth);
    connect(btnNextMonth, &QPushButton::clicked, this, [this] {
        setCurrentDisplayDate(curDisplayDate.addMonths(1));
    });

    auto btnNextYear = new HoverIconButton(title);
    btnNextYear->setIcon(QIcon(QStringLiteral(":/animatedwidgets/calendars/yearright.svg")));
    hBox->addWidget(btnNextYear);
    connect(btnNextYear, &QPushButton::clicked, this, [this] {
        setCurrentDisplayDate(curDisplayDate.addYears(1));
    });

    hBox->setStretch(2, 1);

    return title;
}

/**
 * @brief Creates the date grid.
 * @return
 */
QWidget* CalendarDateSelector::createDateSelector() {
    auto table = dateTable = new QTableView(this);
    table->setMouseTracking(true);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setMinimumHeight(226);
    table->verticalHeader()->setVisible(false);
    table->verticalHeader()->setMinimumSectionSize(24);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->horizontalHeader()->setHighlightSections(false);
    table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    table->setShowGrid(false);
    table->setSelectionMode(QAbstractItemView::NoSelection);
    table->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    table->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    table->setStyleSheet(m_calendarStyle.dateStyleSheet());

    table->setModel(model = new QStandardItemModel(this));
    table->setItemDelegate(dateItemDelegate = new CalendarDateItemDelegate(this));
    dateItemDelegate->sectionData = sectionData;

    model->setHorizontalHeaderLabels(m_weekdayLabels);
    for (int i = 0; i < 7; ++i) {
        table->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    }

    connect(table, &QTableView::clicked, this, [this, table](const QModelIndex& index) {
        auto dateFromItem = index.data(Qt::UserRole + 1).toDate();
        QPointer<CalendarDateSelector> guard(this);
        testCurrentSelected(dateFromItem);
        if (guard)
            table->viewport()->update();
    });
    return table;
}

/**
 * @brief Applies a date click to the selection.
 * @param dateFromItem
 */
void CalendarDateSelector::testCurrentSelected(QDate dateFromItem) {
    if (!dateFromItem.isValid())
        return;
    if (dateFromItem.year() == curDisplayDate.year() && dateFromItem.month() == curDisplayDate.month()) {
        if (sectionData->rangeModeEnabled) {
            // Handle range selection.
            if (sectionData->selectFromDate.isNull() && sectionData->selectToDate.isNull()) {
                // First click starts a range.
                sectionData->selectFromDate = dateFromItem;
                emit dateRangeSelected(sectionData->selectFromDate, sectionData->selectToDate);
            } else if (!sectionData->selectFromDate.isNull() && sectionData->selectToDate.isNull()) {
                // Second click completes and orders the range.
                if (dateFromItem < sectionData->selectFromDate) {
                    qSwap(sectionData->selectFromDate, dateFromItem);
                }
                sectionData->selectToDate = dateFromItem;
                emit dateRangeSelected(sectionData->selectFromDate, sectionData->selectToDate);
            } else {
                // A new click restarts the range.
                sectionData->selectFromDate = dateFromItem;
                sectionData->selectToDate = QDate(); // Clear the end date.
                emit dateRangeSelected(sectionData->selectFromDate, sectionData->selectToDate);
            }
        } else {
            sectionData->selectedDate = dateFromItem;
            emit dateSelected(dateFromItem);
        }
    }
}

/**
 * @brief Updates the displayed month labels.
 */
void CalendarDateSelector::updateCurrentDate() const {
    dateYearLabel->setText(m_yearLabelFormatter(curDisplayDate));
    dateMonthLabel->setText(m_monthLabelFormatter(curDisplayDate));
}

/** @brief Returns the selected date. */
QDate CalendarDateSelector::selectedDate() const {
    return sectionData->selectedDate;
}

/** @brief Sets the selected date without activating it. */
void CalendarDateSelector::setSelectedDate(const QDate& date) {
    sectionData->selectedDate = date;
    if (date.isValid())
        setCurrentDisplayDate(date);
    refreshSelection();
}

/** @brief Returns whether range selection is enabled. */
bool CalendarDateSelector::rangeMode() const {
    return sectionData->rangeModeEnabled;
}

/** @brief Enables single-date or range selection. */
void CalendarDateSelector::setRangeMode(bool enabled) {
    sectionData->rangeModeEnabled = enabled;
    refreshSelection();
}

/** @brief Sets an ordered date range, or clears it when both dates are invalid. */
void CalendarDateSelector::setSelectedRange(const QDate& from, const QDate& to) {
    if (from.isValid() != to.isValid() || (from.isValid() && from > to))
        return;
    sectionData->selectFromDate = from;
    sectionData->selectToDate = to;
    if (from.isValid())
        setCurrentDisplayDate(from);
    refreshSelection();
}

/** @brief Repaints shared selection changes without rebuilding the month. */
void CalendarDateSelector::refreshSelection() {
    dateTable->viewport()->update();
}

/** @brief Returns the current calendar appearance configuration. */
CalendarStyle CalendarDateSelector::calendarStyle() const {
    return m_calendarStyle;
}

/** @brief Applies an appearance configuration to this control and its calendar children. */
void CalendarDateSelector::setCalendarStyle(const CalendarStyle& style) {
    m_calendarStyle = style;
    setAttribute(Qt::WA_StyledBackground);
    setProperty("calendarSelector", true);
    setStyleSheet(style.selectorStyleSheet());
    dateItemDelegate->setCalendarStyle(style);
    dateTable->setFont(style.itemFont);
    dateTable->horizontalHeader()->setFont(style.titleFont);
    dateTable->setStyleSheet(style.dateStyleSheet());
    dateTable->viewport()->update();
    CalendarStyleUtils::applyChildren(this, style);
    update();
}

} // namespace animatedwidgets
