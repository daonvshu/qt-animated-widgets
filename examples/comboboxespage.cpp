#include "comboboxespage.h"
#include "ui_comboboxespage.h"

#include "animatedwidgets/popups/defaultpopupmenuliststyle.h"
#include "animatedwidgets/popups/defaultpopupmenustyle.h"
#include "animatedwidgets/popups/defaultpopupmenumodel.h"

#include <QLabel>
#include <QStringList>
#include <QStandardItemModel>

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

    ui.comboMulti->setStrings(choices);
    ui.comboMulti->setSelectedIndexes({0, 1});
    ui.comboMultiDisabled->setStrings(choices);
    ui.comboMultiDisabled->setSelectedIndexes({1, 2});

    setupTreeExamples(ui);
    setupCreationExamples(ui);

    const int tagColorRole = Qt::UserRole + 100;
    QList<animatedwidgets::PopupMenuItem> tagItems;
    const QStringList categories = {QStringLiteral("Ocean"), QStringLiteral("Sunrise"), QStringLiteral("Forest"), QStringLiteral("Lavender"),
                                    QStringLiteral("Coral"), QStringLiteral("Slate"), QStringLiteral("Cloud"), QStringLiteral("Sand")};
    const QStringList colors = {QStringLiteral("#1778ff"), QStringLiteral("#c78d0c"), QStringLiteral("#24875e"), QStringLiteral("#8b5cf6"),
                                QStringLiteral("#d76655"), QStringLiteral("#64748b"), QStringLiteral("#4d8caa"), QStringLiteral("#aa8752")};
    for (int i = 0; i < categories.size(); ++i) {
        animatedwidgets::PopupMenuItem item;
        item.text = categories.at(i);
        item.values.insert(tagColorRole, QColor(colors.at(i)));
        tagItems.append(item);
    }
    ui.comboMultiSearch->setItems(tagItems);
    ui.comboMultiSearch->setPlaceholderText(QStringLiteral("Filter categories"));
    ui.comboMultiSearch->setMaxVisibleItems(5);
    ui.comboMultiSearch->setTagStyleFactory([tagColorRole](const QModelIndex& index) {
        animatedwidgets::TagStyle style;
        style.content = index.data(tagColorRole).value<QColor>();
        style.background = style.content;
        style.background.setAlpha(24);
        return style;
    });
    ui.comboMultiSearch->setSelectedIndexes({0, 1});
    for (auto* combo : {ui.comboMulti, ui.comboMultiSearch}) {
        connect(combo, &MultiSelectComboBox::selectedIndexesChanged, this, [combo, label = ui.multiResultLabel](const QList<int>& indexes) {
            QStringList selected;
            for (int index : indexes)
                selected.append(combo->model()->indexForItem(index).data().toString());
            label->setText(QStringLiteral("Selected: %1").arg(selected.isEmpty() ? QStringLiteral("none") : selected.join(QStringLiteral(", "))));
        });
        connect(ui.errorCheck, &QCheckBox::toggled, combo, &MultiSelectComboBox::setHasError);
        connect(ui.arrowCheck, &QCheckBox::toggled, combo, &MultiSelectComboBox::setArrowVisible);
        connect(ui.durationSpin, QOverload<int>::of(&QSpinBox::valueChanged), combo, &MultiSelectComboBox::setAnimationDuration);
    }

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

void ComboBoxesPage::setupTreeExamples(const Ui::ComboBoxesPage& ui) {
    auto* treeModel = new QStandardItemModel(this);
    auto* workspace = new QStandardItem(QIcon(QStringLiteral(":/animatedwidgets/gallery/folder.svg")), QStringLiteral("Workspace"));
    auto* documents = new QStandardItem(QStringLiteral("Documents"));
    auto* guide = new QStandardItem(QIcon(QStringLiteral(":/animatedwidgets/gallery/document.svg")), QStringLiteral("Quick guide"));
    guide->setData(QStringLiteral("guide"), Qt::UserRole);
    documents->appendRow(guide);
    auto* notes = new QStandardItem(QStringLiteral("Release notes"));
    notes->setData(QStringLiteral("notes"), Qt::UserRole);
    documents->appendRow(notes);
    workspace->appendRow(documents);

    auto* media = new QStandardItem(QStringLiteral("Media"));
    auto* illustrations = new QStandardItem(QStringLiteral("Illustrations"));
    illustrations->setData(QStringLiteral("illustrations"), Qt::UserRole);
    media->appendRow(illustrations);
    auto* archived = new QStandardItem(QStringLiteral("Archived assets"));
    archived->setEnabled(false);
    media->appendRow(archived);
    workspace->appendRow(media);
    treeModel->appendRow(workspace);

    auto* shared = new QStandardItem(QStringLiteral("Shared library"));
    shared->appendRow(new QStandardItem(QStringLiteral("Templates")));
    treeModel->appendRow(shared);

    for (auto* combo : {ui.comboTree, ui.comboTreeParents}) {
        combo->setModel(treeModel);
        combo->setMaxVisibleItems(8);
        connect(combo, &TreeComboBox::currentModelIndexChanged, this, [label = ui.treeResultLabel](const QModelIndex& index) {
            QStringList path;
            for (QModelIndex node = index; node.isValid(); node = node.parent())
                path.prepend(node.data().toString());
            label->setText(QStringLiteral("Path: %1 | Data: %2").arg(path.join(QStringLiteral(" / ")), index.data(Qt::UserRole).toString()));
        });
        connect(ui.errorCheck, &QCheckBox::toggled, combo, &ComboBox::setHasError);
        connect(ui.arrowCheck, &QCheckBox::toggled, combo, &ComboBox::setArrowVisible);
        connect(ui.durationSpin, QOverload<int>::of(&QSpinBox::valueChanged), combo, &ComboBox::setAnimationDuration);
    }
    ui.comboTree->setCurrentModelIndex(guide->index());
    ui.comboTreeParents->setCurrentModelIndex(workspace->index());
}

void ComboBoxesPage::setupCreationExamples(const Ui::ComboBoxesPage& ui) {
    ui.comboCreatable->addItems({QStringLiteral("Illustration"), QStringLiteral("Motion"), QStringLiteral("Typography")});
    ui.comboCreatable->setCurrentIndex(-1);
    ui.comboCreatable->lineEdit()->setPlaceholderText(QStringLiteral("Choose or create a category"));
    ui.comboCreatableEmpty->lineEdit()->setPlaceholderText(QStringLiteral("Type at least two characters"));

    for (auto* combo : {ui.comboCreatable, ui.comboCreatableEmpty}) {
        combo->setMaxVisibleItems(5);
        connect(combo, &CreatableComboBox::createRequested, this, [combo, label = ui.creationResultLabel](const QString& text) {
            combo->addItem(text, QStringLiteral("created:%1").arg(text));
            combo->setCurrentIndex(combo->count() - 1);
            label->setText(QStringLiteral("Created and selected: %1").arg(text));
        });
        connect(ui.errorCheck, &QCheckBox::toggled, combo, &ComboBox::setHasError);
        connect(ui.arrowCheck, &QCheckBox::toggled, combo, &ComboBox::setArrowVisible);
        connect(ui.popupOnEditCheck, &QCheckBox::toggled, combo, &ComboBox::setPopupOnEdit);
        connect(ui.durationSpin, QOverload<int>::of(&QSpinBox::valueChanged), combo, &ComboBox::setAnimationDuration);
    }
}
