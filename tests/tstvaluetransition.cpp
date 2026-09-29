#include "animatedwidgets/core/valuetransition.h"

#include <QtTest>

class ValueTransitionTest : public QObject {
    Q_OBJECT
private slots:
    void snapsWhenImmediate() {
        animatedwidgets::ValueTransition transition;
        transition.setDuration(200);
        transition.setTarget(1.0, true);
        QCOMPARE(transition.current().toDouble(), 1.0);
        QVERIFY(!transition.isRunning());
    }

    void snapsWhenDurationIsZero() {
        animatedwidgets::ValueTransition transition;
        transition.setDuration(0);
        transition.setTarget(2.5);
        QCOMPARE(transition.current().toDouble(), 2.5);
        QVERIFY(!transition.isRunning());
    }

    void appliesFirstTargetWithoutPreviousValue() {
        animatedwidgets::ValueTransition transition;
        transition.setDuration(200);
        transition.setTarget(1.0);
        QCOMPARE(transition.current().toDouble(), 1.0);
        QVERIFY(!transition.isRunning());
    }

    void animatesFromEstablishedValue() {
        animatedwidgets::ValueTransition transition;
        transition.setDuration(120);
        transition.setTarget(0.0, true);
        transition.setTarget(1.0);
        QVERIFY(transition.isRunning());
        QTRY_COMPARE(transition.current().toDouble(), 1.0);
        QVERIFY(!transition.isRunning());
    }

    void redirectsFromCurrentValue() {
        animatedwidgets::ValueTransition transition;
        transition.setDuration(400);
        transition.setTarget(0.0, true);
        transition.setTarget(1.0);
        QTRY_VERIFY(transition.current().toDouble() > 0.2);

        const double before = transition.current().toDouble();
        transition.setTarget(0.0);
        const double after = transition.current().toDouble();
        QVERIFY(after <= before);
        QVERIFY(after > 0.5 * before);
        QTRY_COMPARE(transition.current().toDouble(), 0.0);
    }

    void keepsRunningTowardsSameTarget() {
        animatedwidgets::ValueTransition transition;
        transition.setDuration(400);
        transition.setTarget(0.0, true);
        transition.setTarget(1.0);
        QTRY_VERIFY(transition.current().toDouble() > 0.2);

        const double before = transition.current().toDouble();
        transition.setTarget(1.0);
        QVERIFY(transition.isRunning());
        QVERIFY(transition.current().toDouble() >= before);
        QTRY_COMPARE(transition.current().toDouble(), 1.0);
    }
};

QTEST_GUILESS_MAIN(ValueTransitionTest)
#include "tstvaluetransition.moc"
