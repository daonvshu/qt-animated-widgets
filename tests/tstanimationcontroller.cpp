#include "animatedwidgets/core/animationcontroller.h"

#include <QCoreApplication>
#include <QSignalSpy>
#include <QtTest>

class AnimationControllerTest : public QObject {
    Q_OBJECT
private slots:
    void animatesProperty() {
        QObject target;
        target.setProperty("value", 0.0);
        animatedwidgets::AnimationController controller;
        auto spec = controller.spec();
        spec.duration = 0;
        controller.setSpec(spec);
        QSignalSpy spy(&controller, &animatedwidgets::AnimationController::finished);
        controller.animate(&target, "value", 1.0);
        QCOMPARE(target.property("value").toReal(), 1.0);
        QVERIFY(spy.count() >= 1);
    }

    void transitionsPropertyOverTime() {
        QObject target;
        target.setProperty("value", 0.0);
        animatedwidgets::AnimationController controller;
        auto spec = controller.spec();
        spec.duration = 120;
        controller.setSpec(spec);

        controller.animate(&target, "value", 1.0);
        QVERIFY(controller.isRunning());
        QTRY_COMPARE(target.property("value").toReal(), 1.0);
        QVERIFY(!controller.isRunning());
    }

    void redirectsFromCurrentAnimatedValue() {
        QObject target;
        target.setProperty("value", 0.0);
        animatedwidgets::AnimationController controller;
        auto spec = controller.spec();
        spec.duration = 300;
        controller.setSpec(spec);

        controller.animate(&target, "value", 1.0);
        QTRY_VERIFY(target.property("value").toReal() > 0.1);
        const qreal current = target.property("value").toReal();
        controller.animate(&target, "value", 0.0, 1.0);
        QVERIFY(qAbs(target.property("value").toReal() - current) < 0.001);
        QTRY_COMPARE(target.property("value").toReal(), 0.0);
    }
};

QTEST_GUILESS_MAIN(AnimationControllerTest)
#include "tstanimationcontroller.moc"
