#include "comboboxespage.h"
#include "ui_comboboxespage.h"

#include "animatedwidgets/popups/defaultpopupmenuliststyle.h"
#include "animatedwidgets/popups/defaultpopupmenustyle.h"

#include <QLabel>
#include <QStringList>

ComboBoxesPage::ComboBoxesPage(QWidget* parent)
    : QWidget(parent)
{
    Ui::ComboBoxesPage ui;
    ui.setupUi(this);

    const QStringList choices = {QStringLiteral("Design"), QStringLiteral("Development"), QStringLiteral("Testing"),
                                 QStringLiteral("Delivery")};
    ui.comboNormal->addGroupTitle(QStringLiteral("Planning"));
    ui.comboNormal->addItem(QStringLiteral("Design"));
    ui.comboNormal->addItem(QStringLiteral("Development"));
    ui.comboNormal->insertSeparator(ui.comboNormal->count());
    ui.comboNormal->addGroupTitle(QStringLiteral("Release"));
    ui.comboNormal->addItem(QStringLiteral("Testing"));
    ui.comboNormal->addItem(QStringLiteral("Delivery"));
    ui.comboNormal->setCurrentIndex(1);
    ui.comboEditable->addItems(choices);
    ui.comboEditable->setCurrentText(QStringLiteral("Development"));
    ui.comboEditable->setMaxVisibleItems(3);
    ui.comboDisabled->addItems(choices);
    ui.comboComposite->editor()->addItems(choices);

    ui.comboSearch->addGroupTitle(QStringLiteral("Local assets"));
    ui.comboSearch->addItem(QStringLiteral("Orchard"), QStringLiteral("orchard"));
    ui.comboSearch->addItem(QStringLiteral("Harbor"), QStringLiteral("harbor"));
    ui.comboSearch->insertSeparator(ui.comboSearch->count());
    ui.comboSearch->addGroupTitle(QStringLiteral("Shared assets"));
    ui.comboSearch->addItem(QStringLiteral("Summit"), QStringLiteral("summit"));
    ui.comboSearch->addItem(QStringLiteral("Meadow"), QStringLiteral("meadow"));
    ui.comboSearch->setCurrentIndex(1);
    ui.comboSearch->setMaxVisibleItems(6);
    ui.comboSearch->lineEdit()->setPlaceholderText(QStringLiteral("Type to search assets"));
    auto* emptyLabel = new QLabel(QStringLiteral("No matching assets"));
    emptyLabel->setAlignment(Qt::AlignCenter);
    emptyLabel->setMinimumHeight(72);
    ui.comboSearch->setEmptyWidget(emptyLabel);

    ui.comboIcons->setIconSize(QSize(20, 20));
    ui.comboIcons->addItem(QIcon(QStringLiteral(":/animatedwidgets/gallery/folder.svg")), QStringLiteral("Project folder"));
    ui.comboIcons->addItem(QIcon(QStringLiteral(":/animatedwidgets/gallery/document.svg")), QStringLiteral("Release notes"));
    ui.comboIcons->addItem(QIcon(QStringLiteral(":/animatedwidgets/gallery/bookmark.svg")), QStringLiteral("Saved view"));
    ui.comboIcons->addItem(QStringLiteral("No icon"));
    connect(ui.comboIcons, &ComboBox::popupAboutToShow, this, [](animatedwidgets::PopupMenu* popup) {
        auto* style = new animatedwidgets::DefaultPopupMenuListStyle;
        style->itemIconSize = QSize(20, 20);
        style->itemIconSpacing = 8;
        style->checkedIcon = QIcon(QStringLiteral(":/animatedwidgets/gallery/buttoncheck.svg"));
        style->checkedIconAlignment = Qt::AlignRight;
        popup->setListStyle(style);
    });

    ui.comboComposite->setLeadingWidget(new QLabel(QStringLiteral("Team"), ui.comboComposite));
    ui.comboComposite->setTrailingWidget(new QLabel(QStringLiteral("Role"), ui.comboComposite));
    ui.comboComposite->setTopWidget(new QLabel(QStringLiteral("Assignment"), ui.comboComposite));
    ui.comboComposite->setBottomWidget(new QLabel(QStringLiteral("Choose a team"), ui.comboComposite));
    ui.comboComposite->setDrawInputLeftBorder(true);
    ui.comboComposite->setDrawInputRightBorder(true);
    ui.comboComposite->setDrawInputTopBorder(true);
    ui.comboComposite->setDrawInputBottomBorder(true);

    connect(ui.comboComposite, &CompositeComboBox::popupAboutToShow, this, [](animatedwidgets::PopupMenu* popup) {
        auto* style = new animatedwidgets::DefaultPopupMenuStyle;
        style->popupBackground = QColor(QStringLiteral("#253247"));
        style->popupBorder = QColor(QStringLiteral("#657b9a"));
        style->popupCornerRadius = 12;
        popup->setPopupStyle(style);

        auto* listStyle = new animatedwidgets::DefaultPopupMenuListStyle;
        listStyle->itemHoverBackground = QColor(QStringLiteral("#364b69"));
        listStyle->itemSelectedBackground = QColor(QStringLiteral("#42679b"));
        listStyle->itemText = QColor(QStringLiteral("#edf2fa"));
        listStyle->itemDisabledText = QColor(QStringLiteral("#8190a7"));
        listStyle->checkedIcon = QIcon(QStringLiteral(":/animatedwidgets/comboboxes/checked.svg"));
        listStyle->checkedIconAlignment = Qt::AlignRight;
        listStyle->itemHeight = 36;
        listStyle->itemSpacing = 4;
        popup->setListStyle(listStyle);
    });

    connect(ui.errorCheck, &QCheckBox::toggled, ui.comboNormal, &ComboBox::setHasError);
    connect(ui.popupOnEditCheck, &QCheckBox::toggled, ui.comboEditable, &ComboBox::setPopupOnEdit);
    connect(ui.popupOnEditCheck, &QCheckBox::toggled, ui.comboSearch, &ComboBox::setPopupOnEdit);
    connect(ui.popupOnEditCheck, &QCheckBox::toggled, ui.comboComposite, &CompositeComboBox::setPopupOnEdit);
    connect(ui.arrowCheck, &QCheckBox::toggled, ui.comboNormal, &ComboBox::setArrowVisible);
    connect(ui.arrowCheck, &QCheckBox::toggled, ui.comboEditable, &ComboBox::setArrowVisible);
    connect(ui.arrowCheck, &QCheckBox::toggled, ui.comboSearch, &ComboBox::setArrowVisible);
    connect(ui.arrowCheck, &QCheckBox::toggled, ui.comboComposite, &CompositeComboBox::setArrowVisible);
    connect(ui.durationSpin, QOverload<int>::of(&QSpinBox::valueChanged), ui.comboNormal, &ComboBox::setAnimationDuration);
    connect(ui.durationSpin, QOverload<int>::of(&QSpinBox::valueChanged), ui.comboEditable, &ComboBox::setAnimationDuration);
    connect(ui.durationSpin, QOverload<int>::of(&QSpinBox::valueChanged), ui.comboSearch, &ComboBox::setAnimationDuration);
    connect(ui.durationSpin, QOverload<int>::of(&QSpinBox::valueChanged), ui.comboComposite, &CompositeComboBox::setAnimationDuration);
}
