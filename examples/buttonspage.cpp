#include "buttonspage.h"
#include "ui_buttonspage.h"

#include "animatedwidgets/buttons/buttonstatestyle.h"

#include <QIcon>
#include <QVector>

ButtonsPage::ButtonsPage(QWidget* parent)
    : QWidget(parent)
{
    Ui::ButtonsPage ui;
    ui.setupUi(this);
    const QVector<QPushButton*> textButtons = {
        ui.filledInteractive, ui.filledChecked, ui.filledDisabled,
        ui.outlineInteractive, ui.outlineChecked, ui.outlineDisabled,
        ui.borderedInteractive, ui.borderedChecked, ui.borderedDisabled,
        ui.hoverInteractive, ui.hoverChecked, ui.hoverDisabled
    };
    const QVector<QPushButton*> iconButtons = {
        ui.filledIconInteractive, ui.filledIconChecked, ui.filledIconDisabled,
        ui.outlineIconInteractive, ui.outlineIconChecked, ui.outlineIconDisabled,
        ui.borderedIconInteractive, ui.borderedIconChecked, ui.borderedIconDisabled,
        ui.hoverIconInteractive, ui.hoverIconChecked, ui.hoverIconDisabled
    };

    const QIcon icon(QStringLiteral(":/animatedwidgets/gallery/buttoncheck.svg"));
    for (int i = 0; i < textButtons.size(); ++i) {
        textButtons[i]->setIcon(icon);
        textButtons[i]->setIconSize(QSize(18, 18));
        textButtons[i]->setProperty("animateContentColor", true);
        textButtons[i]->setProperty("iconPlacement", i / 3);
    }
    for (int i = 0; i < iconButtons.size(); ++i) {
        iconButtons[i]->setIcon(icon);
        iconButtons[i]->setIconSize(QSize(20, 20));
        iconButtons[i]->setProperty("animateContentColor", true);
        iconButtons[i]->setProperty("shape", (i / 3) % 2);
    }

    using animatedwidgets::ButtonStateStyle;
    ui.filledChecked->setNormalStyle(ButtonStateStyle(QColor("#c8ccd4"), {}, QColor("#202124")));
    ui.filledChecked->setCheckedStyle(ButtonStateStyle(QColor("#3478f6"), {}, QColor("#ffffff")));
    ui.filledIconChecked->setNormalStyle(ButtonStateStyle(QColor("#c8ccd4"), {}, QColor("#202124")));
    ui.filledIconChecked->setCheckedStyle(ButtonStateStyle(QColor("#3478f6"), {}, QColor("#ffffff")));
    ui.outlineChecked->setNormalStyle(ButtonStateStyle({}, QColor("#a8adb7"), QColor("#6b7280")));
    ui.outlineChecked->setCheckedStyle(ButtonStateStyle({}, QColor("#3478f6"), QColor("#3478f6")));
    ui.outlineIconChecked->setNormalStyle(ButtonStateStyle({}, QColor("#a8adb7"), QColor("#6b7280")));
    ui.outlineIconChecked->setCheckedStyle(ButtonStateStyle({}, QColor("#3478f6"), QColor("#3478f6")));

    connect(ui.placementCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [textButtons](int index) {
        for (int i = 0; i < textButtons.size(); ++i) {
            textButtons[i]->setProperty("iconPlacement", index == 0 ? i / 3 : index - 1);
        }
    });
    connect(ui.spacingSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [textButtons](int value) {
        for (auto* button : textButtons) button->setProperty("iconSpacing", value);
    });
    connect(ui.depthSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [textButtons, iconButtons](int value) {
        for (auto* button : textButtons) button->setProperty("darkerStep", value);
        for (auto* button : iconButtons) button->setProperty("darkerStep", value);
    });
    connect(ui.animateContentCheck, &QCheckBox::toggled, this,
            [textButtons, iconButtons](bool enabled) {
        for (auto* button : textButtons) button->setProperty("animateContentColor", enabled);
        for (auto* button : iconButtons) button->setProperty("animateContentColor", enabled);
    });
    connect(ui.shapeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [iconButtons](int index) {
        for (int i = 0; i < iconButtons.size(); ++i) {
            iconButtons[i]->setProperty("shape", index == 0
                ? ((i / 3) % 2 == 0 ? 0 : 1)
                : (index == 1 ? 0 : 1));
        }
    });
}
