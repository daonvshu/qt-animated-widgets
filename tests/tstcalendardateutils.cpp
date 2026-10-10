#include "animatedwidgets/calendars/calendarsectiondata.h"
#include "animatedwidgets/utils/calendardateutils.h"

#include <QtTest>

using namespace animatedwidgets;

class CalendarDateUtilsTest : public QObject {
    Q_OBJECT

private slots:
    void leapMonthGrid();
    void mondayStartAndYearBoundary();
    void invalidMonth();
    void rangeBoundaries();
};

void CalendarDateUtilsTest::leapMonthGrid() {
    const auto days = CalendarDateUtils::monthDays(2024, 2);
    QCOMPARE(days.size(), 42);
    QCOMPARE(days.first(), QDate(2024, 1, 29));
    QCOMPARE(days.last(), QDate(2024, 3, 10));
    QVERIFY(days.contains(QDate(2024, 2, 29)));
    for (int index = 1; index < days.size(); ++index)
        QCOMPARE(days.at(index - 1).daysTo(days.at(index)), qint64(1));
}

void CalendarDateUtilsTest::mondayStartAndYearBoundary() {
    const auto monday = CalendarDateUtils::monthDays(2024, 1);
    QCOMPARE(monday.size(), 42);
    QCOMPARE(monday.first(), QDate(2024, 1, 1));

    const auto december = CalendarDateUtils::monthDays(2024, 12);
    QCOMPARE(december.first(), QDate(2024, 11, 25));
    QCOMPARE(december.last(), QDate(2025, 1, 5));
    QCOMPARE(december.first().dayOfWeek(), 1);
    QCOMPARE(december.last().dayOfWeek(), 7);
}

void CalendarDateUtilsTest::invalidMonth() {
    QVERIFY(CalendarDateUtils::monthDays(2024, 0).isEmpty());
    QVERIFY(CalendarDateUtils::monthDays(2024, 13).isEmpty());
    QVERIFY(CalendarDateUtils::monthDays(0, 1).isEmpty());
}

void CalendarDateUtilsTest::rangeBoundaries() {
    CalendarSectionData data;
    QVERIFY(!data.rangeValid());
    QVERIFY(!data.isInSelectRange(QDate(2024, 2, 29)));
    data.selectFromDate = QDate(2024, 2, 29);
    data.selectToDate = QDate(2024, 3, 2);
    QVERIFY(data.rangeValid());
    QVERIFY(data.isInSelectRange(data.selectFromDate));
    QVERIFY(data.isInSelectRange(data.selectToDate));
    QVERIFY(!data.isInSelectRange(QDate(2024, 2, 28)));
    QVERIFY(!data.isInSelectRange(QDate()));

    data.selectToDate = QDate(2024, 2, 28);
    QVERIFY(!data.rangeValid());
    QVERIFY(!data.isInSelectRange(data.selectFromDate));
}

QTEST_APPLESS_MAIN(CalendarDateUtilsTest)

#include "tstcalendardateutils.moc"
