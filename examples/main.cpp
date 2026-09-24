#include "ui_gallery.h"

#include "animatedwidgets/buttons/buttonstatestyle.h"

#include <QApplication>
#include <QIcon>
#include <QPixmap>
#include <QVector>

namespace {

QIcon sampleCheckIcon()
{
    return QIcon(QStringLiteral(":/animatedwidgets/gallery/buttoncheck.svg"));
}

void configureSelection(Ui::Gallery& ui)
{
    const QPixmap radioUnchecked(QStringLiteral(":/animatedwidgets/gallery/radiounchecked.svg"));
    const QPixmap radioChecked(QStringLiteral(":/animatedwidgets/gallery/radiochecked.svg"));
    const QPixmap checkboxUnchecked(QStringLiteral(":/animatedwidgets/gallery/checkboxunchecked.svg"));
    const QPixmap checkboxPartial(QStringLiteral(":/animatedwidgets/gallery/checkboxpartial.svg"));
    const QPixmap checkboxChecked(QStringLiteral(":/animatedwidgets/gallery/checkboxchecked.svg"));

    const QVector<animatedwidgets::RadioButton*> radios = {
        ui.radioInteractive, ui.radioChecked, ui.radioDisabled
    };
    for (auto* radio : radios) {
        radio->setUncheckedIcon(radioUnchecked);
        radio->setCheckedIcon(radioChecked);
        radio->setIconSize(QSize(18, 18));
    }

    const QVector<animatedwidgets::CheckBox*> checkBoxes = {
        ui.checkInteractive, ui.checkPartial, ui.checkChecked, ui.checkDisabled
    };
    for (auto* checkBox : checkBoxes) {
        checkBox->setUncheckedIcon(checkboxUnchecked);
        checkBox->setPartiallyCheckedIcon(checkboxPartial);
        checkBox->setCheckedIcon(checkboxChecked);
        checkBox->setIconSize(QSize(18, 18));
    }
    // checkState is not a Qt property, so Designer cannot round-trip it in the .ui file.
    ui.checkPartial->setCheckState(Qt::PartiallyChecked);
}

void configureGallery(Ui::Gallery& ui)
{
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

    const QIcon icon = sampleCheckIcon();
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

    QObject::connect(ui.placementCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
                     ui.buttonsPage, [textButtons](int index) {
        for (int i = 0; i < textButtons.size(); ++i) {
            textButtons[i]->setProperty("iconPlacement", index == 0 ? i / 3 : index - 1);
        }
    });
    QObject::connect(ui.spacingSpin, QOverload<int>::of(&QSpinBox::valueChanged),
                     ui.buttonsPage, [textButtons](int value) {
        for (auto* button : textButtons) button->setProperty("iconSpacing", value);
    });
    QObject::connect(ui.depthSpin, QOverload<int>::of(&QSpinBox::valueChanged),
                     ui.buttonsPage, [textButtons, iconButtons](int value) {
        for (auto* button : textButtons) button->setProperty("darkerStep", value);
        for (auto* button : iconButtons) button->setProperty("darkerStep", value);
    });
    QObject::connect(ui.animateContentCheck, &QCheckBox::toggled, ui.buttonsPage,
                     [textButtons, iconButtons](bool enabled) {
        for (auto* button : textButtons) button->setProperty("animateContentColor", enabled);
        for (auto* button : iconButtons) button->setProperty("animateContentColor", enabled);
    });
    QObject::connect(ui.shapeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
                     ui.buttonsPage, [iconButtons](int index) {
        for (int i = 0; i < iconButtons.size(); ++i) {
            iconButtons[i]->setProperty("shape", index == 0
                ? ((i / 3) % 2 == 0 ? 0 : 1)
                : (index == 1 ? 0 : 1));
        }
    });
}

} // namespace

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QWidget gallery;
    Ui::Gallery ui;
    ui.setupUi(&gallery);
    configureGallery(ui);
    configureSelection(ui);
    gallery.show();
    return app.exec();
}
