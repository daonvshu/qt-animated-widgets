#include "inputspage.h"
#include "ui_inputspage.h"

#include "animatedwidgets/buttons/hovericonbutton.h"

#include <QIcon>
#include <QLabel>
#include <QPalette>
#include <QVector>

InputsPage::InputsPage(QWidget* parent)
    : QWidget(parent)
{
    Ui::InputsPage ui;
    ui.setupUi(this);
    const QVector<animatedwidgets::LineEdit*> inputs = {
        ui.inputNormal, ui.inputError, ui.inputDisabled, ui.inputReadOnly,
        ui.inputLeadingIcon, ui.inputTrailingIcon, ui.inputPassword,
        ui.inputStateColors
    };
    connect(ui.inputErrorCheck, &QCheckBox::toggled,
            ui.inputError, &LineEdit::setHasError);
    connect(ui.inputStateErrorCheck, &QCheckBox::toggled,
            ui.inputStateColors, &LineEdit::setHasError);
    connect(ui.inputStateDisabledCheck, &QCheckBox::toggled,
            ui.inputStateColors, [input = ui.inputStateColors](bool disabled) {
        input->setEnabled(!disabled);
    });
    connect(ui.inputDurationSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [inputs](int duration) {
        for (auto* input : inputs) input->setAnimationDuration(duration);
    });
    connect(ui.passwordAutoHideCheck, &QCheckBox::toggled,
            ui.inputPassword, &PasswordLineEdit::setAutoHideEnabled);
    connect(ui.inputTrailingIcon, &IconLineEdit::trailingIconClicked,
            ui.inputTrailingIcon, &QLineEdit::clear);
    ui.inputComposite->setLeadingWidgetFactory([](QWidget* parent) {
        auto* label = new QLabel(QStringLiteral("USD"), parent);
        label->setMinimumWidth(40);
        label->setAlignment(Qt::AlignCenter);
        return label;
    });
    auto* currencyLabel = static_cast<QLabel*>(ui.inputComposite->leadingWidget());
    const auto updateCurrencyColor = [currencyLabel](const QColor& color) {
        QPalette palette = currencyLabel->palette();
        palette.setColor(QPalette::WindowText, color);
        currencyLabel->setPalette(palette);
    };
    connect(ui.inputComposite, &CompositeLineEdit::currentBorderColorChanged,
            currencyLabel, updateCurrencyColor);
    updateCurrencyColor(ui.inputComposite->currentBorderColor());
    ui.inputComposite->setTrailingWidgetFactory([input = ui.inputComposite](QWidget* parent) {
        auto* button = new HoverIconButton(parent);
        button->setFixedSize(26, 26);
        button->setIcon(QIcon(QStringLiteral(":/animatedwidgets/inputs/clear.svg")));
        button->setIconSize(QSize(16, 16));
        QObject::connect(button, &QPushButton::clicked, input->editor(), &QLineEdit::clear);
        return button;
    });
    ui.inputComposite->setDrawInputLeftBorder(true);
    ui.inputComposite->setDrawInputRightBorder(true);
    connect(ui.inputErrorCheck, &QCheckBox::toggled,
            ui.inputComposite, &CompositeLineEdit::setHasError);
    connect(ui.inputDurationSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            ui.inputComposite, &CompositeLineEdit::setAnimationDuration);
}
