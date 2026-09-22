#include "ui_gallery.h"

#include "animatedwidgets/buttons/buttonstatestyle.h"

#include <QApplication>
#include <QPainter>
#include <QPixmap>
#include <QSvgRenderer>
#include <QVector>

namespace {

QIcon sampleCheckIcon()
{
    static const QByteArray svg = QByteArrayLiteral(
        "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 24 24\">"
        "<path d=\"M4.5 12.5 9.5 17.5 19.5 6.5\" fill=\"none\" "
        "stroke=\"#202124\" stroke-width=\"2.4\" stroke-linecap=\"round\" "
        "stroke-linejoin=\"round\"/>"
        "</svg>");
    static const QIcon icon = [] {
        QIcon result;
        QSvgRenderer renderer(svg);
        for (const QSize size : {QSize(16, 16), QSize(18, 18), QSize(20, 20), QSize(24, 24)}) {
            QPixmap pixmap(size);
            pixmap.fill(Qt::transparent);
            QPainter painter(&pixmap);
            renderer.render(&painter);
            result.addPixmap(pixmap, QIcon::Normal, QIcon::Off);
            result.addPixmap(pixmap, QIcon::Normal, QIcon::On);
        }
        return result;
    }();
    return icon;
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
    gallery.show();
    return app.exec();
}
