#include "animatedwidgets/utils/buttonstatecolors.h"

#include <QtTest>

using namespace animatedwidgets;

class ButtonStateColorsTest : public QObject {
    Q_OBJECT

private slots:
    void defaultFallbacks();
    void explicitHoverFallbacks();
    void hoverOnlyFill();
};

void ButtonStateColorsTest::defaultFallbacks()
{
    const QColor normal("#808080");
    const ButtonStateColors colors{normal, {}, {}, {}, {}};

    QCOMPARE(ButtonStateColorUtils::resolve(colors, ButtonState::Normal, 15), normal);
    QCOMPARE(ButtonStateColorUtils::resolve(colors, ButtonState::Hover, 15), normal.darker(115));
    QCOMPARE(ButtonStateColorUtils::resolve(colors, ButtonState::Pressed, 15), normal.darker(130));
    QCOMPARE(ButtonStateColorUtils::resolve(colors, ButtonState::Checked, 15), normal.darker(130));
    QCOMPARE(ButtonStateColorUtils::resolve(colors, ButtonState::Disabled, 15), normal);
}

void ButtonStateColorsTest::explicitHoverFallbacks()
{
    const QColor normal("#808080");
    const QColor hover("#3478f6");
    const QColor pressed("#123456");
    const ButtonStateColors colors{normal, hover, {}, {}, {}};

    QCOMPARE(ButtonStateColorUtils::resolve(colors, ButtonState::Hover, 15), hover);
    QCOMPARE(ButtonStateColorUtils::resolve(colors, ButtonState::Pressed, 15), hover.darker(115));
    QCOMPARE(ButtonStateColorUtils::resolve(colors, ButtonState::Checked, 15), hover.darker(115));

    const ButtonStateColors withPressed{normal, hover, pressed, {}, {}};
    QCOMPARE(ButtonStateColorUtils::resolve(withPressed, ButtonState::Checked, 15), pressed);
}

void ButtonStateColorsTest::hoverOnlyFill()
{
    const QColor active("#3478f6");
    const ButtonStateColors colors{active, {}, {}, {}, {}};
    QColor transparentActive = active;
    transparentActive.setAlpha(0);

    QCOMPARE(ButtonStateColorUtils::resolveHoverOnly(colors, ButtonState::Normal, 15), transparentActive);
    QCOMPARE(ButtonStateColorUtils::resolveHoverOnly(colors, ButtonState::Hover, 15), active);
    QCOMPARE(ButtonStateColorUtils::resolveHoverOnly(colors, ButtonState::Pressed, 15), active.darker(130));
    QCOMPARE(ButtonStateColorUtils::resolveHoverOnly(colors, ButtonState::Checked, 15), active.darker(130));
    QCOMPARE(ButtonStateColorUtils::resolveHoverOnly(colors, ButtonState::Disabled, 15), transparentActive);

    const QColor hover("#e9effb");
    const ButtonStateColors withHover{active, hover, {}, {}, QColor(Qt::transparent)};
    QColor transparentHover = hover;
    transparentHover.setAlpha(0);
    QCOMPARE(ButtonStateColorUtils::resolveHoverOnly(withHover, ButtonState::Normal, 15), transparentHover);
    QCOMPARE(ButtonStateColorUtils::resolveHoverOnly(withHover, ButtonState::Hover, 15), hover);
    QCOMPARE(ButtonStateColorUtils::resolveHoverOnly(withHover, ButtonState::Disabled, 15), transparentHover);
}

QTEST_APPLESS_MAIN(ButtonStateColorsTest)

#include "tstbuttonstatecolors.moc"
