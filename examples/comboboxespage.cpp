#include "comboboxespage.h"
#include "ui_comboboxespage.h"

#include <QLabel>
#include <QStringList>

ComboBoxesPage::ComboBoxesPage(QWidget* parent)
    : QWidget(parent)
{
    Ui::ComboBoxesPage ui;
    ui.setupUi(this);

    const QStringList choices = {QStringLiteral("Design"), QStringLiteral("Development"), QStringLiteral("Testing"),
                                 QStringLiteral("Delivery")};
    ui.comboNormal->addItems(choices);
    ui.comboEditable->addItems(choices);
    ui.comboEditable->setCurrentText(QStringLiteral("Development"));
    ui.comboDisabled->addItems(choices);
    ui.comboComposite->editor()->addItems(choices);

    ui.comboComposite->setLeadingWidget(new QLabel(QStringLiteral("Team"), ui.comboComposite));
    ui.comboComposite->setTrailingWidget(new QLabel(QStringLiteral("Role"), ui.comboComposite));
    ui.comboComposite->setTopWidget(new QLabel(QStringLiteral("Assignment"), ui.comboComposite));
    ui.comboComposite->setBottomWidget(new QLabel(QStringLiteral("Choose a team"), ui.comboComposite));
    ui.comboComposite->setDrawInputLeftBorder(true);
    ui.comboComposite->setDrawInputRightBorder(true);
    ui.comboComposite->setDrawInputTopBorder(true);
    ui.comboComposite->setDrawInputBottomBorder(true);

    connect(ui.errorCheck, &QCheckBox::toggled, ui.comboNormal, &ComboBox::setHasError);
    connect(ui.arrowCheck, &QCheckBox::toggled, ui.comboNormal, &ComboBox::setArrowVisible);
    connect(ui.arrowCheck, &QCheckBox::toggled, ui.comboEditable, &ComboBox::setArrowVisible);
    connect(ui.arrowCheck, &QCheckBox::toggled, ui.comboComposite, &CompositeComboBox::setArrowVisible);
    connect(ui.durationSpin, QOverload<int>::of(&QSpinBox::valueChanged), ui.comboNormal, &ComboBox::setAnimationDuration);
    connect(ui.durationSpin, QOverload<int>::of(&QSpinBox::valueChanged), ui.comboEditable, &ComboBox::setAnimationDuration);
    connect(ui.durationSpin, QOverload<int>::of(&QSpinBox::valueChanged), ui.comboComposite, &CompositeComboBox::setAnimationDuration);
}
