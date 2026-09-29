#include "progresspage.h"
#include "ui_progresspage.h"

#include <QSignalBlocker>
#include <QVector>

ProgressPage::ProgressPage(QWidget* parent)
    : QWidget(parent)
{
    Ui::ProgressPage ui;
    ui.setupUi(this);
    const QVector<animatedwidgets::ProgressBar*> bars = {
        ui.progressStandard, ui.progressCount, ui.progressDisabled
    };
    connect(ui.progressValueSlider, &QSlider::valueChanged,
            this, [ui](int value) {
        ui.progressValueSpin->setValue(value);
    });
    connect(ui.progressValueSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [ui](double value) {
        const QSignalBlocker blocker(ui.progressValueSlider);
        ui.progressValueSlider->setValue(qRound(value));
        ui.progressStandard->setRealValue(value);
        ui.progressCount->setRealValue(value * 2.0);
    });
    connect(ui.progressAnimationCheck, &QCheckBox::toggled,
            this, [bars](bool enabled) {
        for (auto* bar : bars) bar->setAnimationEnabled(enabled);
    });
    connect(ui.progressDurationSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [bars](int duration) {
        for (auto* bar : bars) bar->setAnimationDuration(duration);
    });
    connect(ui.progressCurveCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [bars](int index) {
        const QEasingCurve::Type curves[] = {
            QEasingCurve::OutCubic, QEasingCurve::Linear,
            QEasingCurve::InOutCubic, QEasingCurve::OutQuart
        };
        if (index < 0 || index >= 4) return;
        for (auto* bar : bars) bar->setAnimationEasingCurve(curves[index]);
    });
}
