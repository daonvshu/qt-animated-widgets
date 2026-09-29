#include "sliderspage.h"
#include "ui_sliderspage.h"

#include <QVector>

SlidersPage::SlidersPage(QWidget* parent)
    : QWidget(parent)
{
    Ui::SlidersPage ui;
    ui.setupUi(this);
    const QVector<animatedwidgets::Slider*> sliders = {
        ui.sliderHorizontal, ui.sliderTicks, ui.sliderVertical, ui.sliderDisabled
    };
    connect(ui.sliderTargetSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [sliders](int value) {
        for (auto* slider : sliders) slider->setValue(value);
    });
    connect(ui.sliderHorizontal, &QSlider::valueChanged,
            ui.sliderTargetSpin, &QSpinBox::setValue);
    connect(ui.sliderAnimationCheck, &QCheckBox::toggled,
            this, [sliders](bool enabled) {
        for (auto* slider : sliders) slider->setAnimationEnabled(enabled);
    });
    connect(ui.sliderDurationSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [sliders](int duration) {
        for (auto* slider : sliders) slider->setAnimationDuration(duration);
    });
    connect(ui.sliderCurveCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [sliders](int index) {
        const QEasingCurve::Type curves[] = {
            QEasingCurve::OutCubic, QEasingCurve::Linear,
            QEasingCurve::InOutCubic, QEasingCurve::OutQuart
        };
        if (index < 0 || index >= 4) return;
        for (auto* slider : sliders) slider->setAnimationEasingCurve(curves[index]);
    });
}
