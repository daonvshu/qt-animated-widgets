#include "../styleeditor/utils/screencolorutils.h"

#include <QtTest>

#include <limits>

class ScreenColorUtilsTest : public QObject {
    Q_OBJECT

private slots:
    void mapsScreenCoordinates_data();
    void mapsScreenCoordinates();
    void samplesPhysicalPixels();
    void rejectsInvalidCoordinates();
};

void ScreenColorUtilsTest::mapsScreenCoordinates_data() {
    QTest::addColumn<QSize>("imageSize");
    QTest::addColumn<QRect>("screenGeometry");
    QTest::addColumn<QPointF>("position");
    QTest::addColumn<QPoint>("pixel");

    QTest::newRow("standard") << QSize(4, 3) << QRect(100, 50, 4, 3) << QPointF(101, 52) << QPoint(1, 2);
    QTest::newRow("negative-origin") << QSize(8, 6) << QRect(-4, -3, 4, 3) << QPointF(-2.5, -1.5) << QPoint(3, 3);
    QTest::newRow("fractional-scale") << QSize(5, 5) << QRect(0, 0, 4, 4) << QPointF(3.9, 3.9) << QPoint(4, 4);
    QTest::newRow("first-pixel") << QSize(8, 6) << QRect(-4, -3, 4, 3) << QPointF(-4, -3) << QPoint(0, 0);
}

void ScreenColorUtilsTest::mapsScreenCoordinates() {
    QFETCH(QSize, imageSize);
    QFETCH(QRect, screenGeometry);
    QFETCH(QPointF, position);
    QFETCH(QPoint, pixel);
    QCOMPARE(ScreenColorUtils::imagePosition(imageSize, screenGeometry, position), pixel);
}

void ScreenColorUtilsTest::samplesPhysicalPixels() {
    QImage image(4, 2, QImage::Format_ARGB32);
    image.fill(Qt::black);
    image.setDevicePixelRatio(2.0);
    image.setPixelColor(0, 0, Qt::red);
    image.setPixelColor(1, 0, Qt::blue);
    image.setPixelColor(3, 1, Qt::green);
    const QRect screen(-2, -1, 2, 1);

    QCOMPARE(ScreenColorUtils::colorAt(image, screen, QPointF(-1.75, -0.75)), QColor(Qt::red));
    QCOMPARE(ScreenColorUtils::colorAt(image, screen, QPointF(-1.25, -0.75)), QColor(Qt::blue));
    QCOMPARE(ScreenColorUtils::colorAt(image, screen, QPointF(-0.25, -0.25)), QColor(Qt::green));
}

void ScreenColorUtilsTest::rejectsInvalidCoordinates() {
    const QSize imageSize(4, 2);
    const QRect screen(-2, -1, 2, 1);
    QCOMPARE(ScreenColorUtils::imagePosition(QSize(), screen, QPointF(-1, -0.5)), QPoint(-1, -1));
    QCOMPARE(ScreenColorUtils::imagePosition(imageSize, QRect(), QPointF()), QPoint(-1, -1));
    QCOMPARE(ScreenColorUtils::imagePosition(imageSize, screen, QPointF(0, -0.5)), QPoint(-1, -1));
    QCOMPARE(ScreenColorUtils::imagePosition(imageSize, screen, QPointF(-1, 0)), QPoint(-1, -1));
    QCOMPARE(ScreenColorUtils::imagePosition(imageSize, screen, QPointF(-2.1, -0.5)), QPoint(-1, -1));
    const qreal invalid = std::numeric_limits<qreal>::quiet_NaN();
    QCOMPARE(ScreenColorUtils::imagePosition(imageSize, screen, QPointF(invalid, 0)), QPoint(-1, -1));
    QVERIFY(!ScreenColorUtils::colorAt(QImage(), screen, QPointF(-1, -0.5)).isValid());
}

QTEST_APPLESS_MAIN(ScreenColorUtilsTest)

#include "tstscreencolorutils.moc"
