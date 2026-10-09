#include "popupmenuspage.h"
#include "ui_popupmenuspage.h"

#include "animatedwidgets/popups/defaultpopupmenuliststyle.h"
#include "animatedwidgets/popups/defaultpopupmenumodel.h"
#include "animatedwidgets/popups/defaultpopupmenustyle.h"
#include "animatedwidgets/popups/popupmenu.h"
#include "animatedwidgets/popups/searchpopupmenu.h"
#include "animatedwidgets/inputs/iconlineedit.h"

#include <QAbstractItemView>
#include <QColorDialog>
#include <QLineEdit>
#include <QStringList>
#include <QVector>
#include <QLabel>

PopupMenusPage::PopupMenusPage(QWidget* parent)
    : QWidget(parent)
    , m_ui(new Ui::PopupMenusPage)
{
    m_ui->setupUi(this);
    initializeColorEditors();
    resetSettings();

    connect(m_ui->defaultButton, &QPushButton::clicked, this, [this] {
        showMenu(m_ui->defaultButton);
    });
    connect(m_ui->customButton, &QPushButton::clicked, this, [this] {
        applyDarkColors();
        showMenu(m_ui->customButton);
    });
    connect(m_ui->resetButton, &QPushButton::clicked, this, &PopupMenusPage::resetSettings);
    connect(m_ui->groupedButton, &QPushButton::clicked, this, [this] { showGroupedMenu(m_ui->groupedButton); });
    connect(m_ui->modulesButton, &QPushButton::clicked, this, [this] { showModulesMenu(m_ui->modulesButton); });
    connect(m_ui->searchButton, &QPushButton::clicked, this, [this] { showSearchMenu(m_ui->searchButton); });
    connect(m_ui->iconsButton, &QPushButton::clicked, this, [this] { showIconMenu(m_ui->iconsButton); });
}

PopupMenusPage::~PopupMenusPage() = default;

void PopupMenusPage::initializeColorEditors() {
    const QVector<QLineEdit*> editors = {
        m_ui->popupBackgroundEdit, m_ui->popupBorderEdit, m_ui->backgroundEdit,
        m_ui->itemBackgroundEdit, m_ui->itemHoverBackgroundEdit, m_ui->itemSelectedBackgroundEdit,
        m_ui->itemTextEdit, m_ui->itemDisabledTextEdit, m_ui->separatorEdit
    };
    for (auto* editor : editors) {
        auto* button = findChild<QPushButton*>(editor->objectName() + QStringLiteral("Button"));
        connect(button, &QPushButton::clicked, this, [this, editor] {
            const QColor initial(editor->text().trimmed());
            const QColor color = QColorDialog::getColor(initial.isValid() ? initial : QColor(Qt::white),
                                                        this, tr("Choose color"), QColorDialog::ShowAlphaChannel);
            if (color.isValid())
                editor->setText(color.name(QColor::HexArgb));
        });
    }
}

void PopupMenusPage::resetSettings() {
    m_ui->popupBackgroundEdit->setText(QStringLiteral("#ffffff"));
    m_ui->popupBorderEdit->setText(QStringLiteral("#d5dae0"));
    m_ui->backgroundEdit->setText(QStringLiteral("#00000000"));
    m_ui->itemBackgroundEdit->setText(QStringLiteral("#00000000"));
    m_ui->itemHoverBackgroundEdit->setText(QStringLiteral("#F2F3F5"));
    m_ui->itemSelectedBackgroundEdit->setText(QStringLiteral("#E3F3FF"));
    m_ui->itemTextEdit->setText(QStringLiteral("#20242a"));
    m_ui->itemDisabledTextEdit->setText(QStringLiteral("#9298a2"));
    m_ui->separatorEdit->setText(QStringLiteral("#e5e8ec"));

    m_ui->radiusSpin->setValue(8);
    m_ui->borderWidthSpin->setValue(1);
    m_ui->shadowRadiusSpin->setValue(12);
    m_ui->heightSpin->setValue(32);
    m_ui->itemSpacingSpin->setValue(0);
    m_ui->itemRadiusSpin->setValue(4);
    m_ui->listRadiusSpin->setValue(4);
    m_ui->paddingSpin->setValue(8);
    m_ui->spacingSpin->setValue(4);
    m_ui->iconWidthSpin->setValue(14);
    m_ui->iconHeightSpin->setValue(14);
    m_ui->leftMarginSpin->setValue(8);
    m_ui->topMarginSpin->setValue(8);
    m_ui->rightMarginSpin->setValue(8);
    m_ui->bottomMarginSpin->setValue(8);
    m_ui->widthSpin->setValue(260);

    m_ui->shadowCheck->setChecked(true);
    m_ui->borderCheck->setChecked(true);
    m_ui->separatorCheck->setChecked(false);
    m_ui->checkedIconCheck->setChecked(true);
    m_ui->autoCloseCheck->setChecked(true);
    m_ui->checkboxIconsCheck->setChecked(false);
    m_ui->multiSelectionCheck->setChecked(true);
    m_ui->iconPositionCombo->setCurrentIndex(0);
    m_ui->resultLabel->setText(tr("Last action: none"));
    m_ui->highlightCheck->setChecked(true);
    m_ui->highlightBoldCheck->setChecked(true);
    m_ui->highlightColorEdit->setText(QStringLiteral("#1778ff"));
}

void PopupMenusPage::applyDarkColors() {
    m_ui->popupBackgroundEdit->setText(QStringLiteral("#253247"));
    m_ui->popupBorderEdit->setText(QStringLiteral("#657b9a"));
    m_ui->backgroundEdit->setText(QStringLiteral("#00000000"));
    m_ui->itemBackgroundEdit->setText(QStringLiteral("#00000000"));
    m_ui->itemHoverBackgroundEdit->setText(QStringLiteral("#364b69"));
    m_ui->itemSelectedBackgroundEdit->setText(QStringLiteral("#42679b"));
    m_ui->itemTextEdit->setText(QStringLiteral("#edf2fa"));
    m_ui->itemDisabledTextEdit->setText(QStringLiteral("#8190a7"));
    m_ui->separatorEdit->setText(QStringLiteral("#43536b"));
}

bool PopupMenusPage::colorsValid() const {
    const QVector<QLineEdit*> editors = {
        m_ui->popupBackgroundEdit, m_ui->popupBorderEdit, m_ui->backgroundEdit,
        m_ui->itemBackgroundEdit, m_ui->itemHoverBackgroundEdit, m_ui->itemSelectedBackgroundEdit,
        m_ui->itemTextEdit, m_ui->itemDisabledTextEdit, m_ui->separatorEdit
    };
    for (auto* editor : editors) {
        if (!QColor(editor->text().trimmed()).isValid()) {
            m_ui->resultLabel->setText(tr("Invalid color: %1. Use a color name, #RRGGBB or #AARRGGBB.").arg(editor->text()));
            editor->setFocus();
            editor->selectAll();
            return false;
        }
    }
    return true;
}

void PopupMenusPage::showMenu(QWidget* anchor) {
    if (m_popup || !colorsValid())
        return;

    auto* popup = new animatedwidgets::PopupMenu(this);
    m_popup = popup;
    popup->setAutoClose(m_ui->autoCloseCheck->isChecked());

    auto* surface = new animatedwidgets::DefaultPopupMenuStyle;
    surface->shadowVisible = m_ui->shadowCheck->isChecked();
    surface->borderVisible = m_ui->borderCheck->isChecked();
    surface->popupCornerRadius = m_ui->radiusSpin->value();
    surface->popupBorderWidth = m_ui->borderWidthSpin->value();
    surface->shadowRadius = m_ui->shadowRadiusSpin->value();
    surface->popupBackground = QColor(m_ui->popupBackgroundEdit->text().trimmed());
    surface->popupBorder = QColor(m_ui->popupBorderEdit->text().trimmed());
    popup->setPopupStyle(surface);

    auto* listStyle = new animatedwidgets::DefaultPopupMenuListStyle;
    listStyle->background = QColor(m_ui->backgroundEdit->text().trimmed());
    listStyle->itemBackground = QColor(m_ui->itemBackgroundEdit->text().trimmed());
    listStyle->itemHoverBackground = QColor(m_ui->itemHoverBackgroundEdit->text().trimmed());
    listStyle->itemSelectedBackground = QColor(m_ui->itemSelectedBackgroundEdit->text().trimmed());
    listStyle->itemText = QColor(m_ui->itemTextEdit->text().trimmed());
    listStyle->itemDisabledText = QColor(m_ui->itemDisabledTextEdit->text().trimmed());
    listStyle->separator = QColor(m_ui->separatorEdit->text().trimmed());
    listStyle->separatorVisible = m_ui->separatorCheck->isChecked();
    listStyle->checkedIconVisible = m_ui->checkedIconCheck->isChecked();
    listStyle->checkedIcon = QIcon(QStringLiteral(":/animatedwidgets/gallery/buttoncheck.svg"));
    if (m_ui->checkboxIconsCheck->isChecked()) {
        listStyle->checkedIcon = QIcon(QStringLiteral(":/animatedwidgets/gallery/checkboxchecked.svg"));
        listStyle->uncheckedIcon = QIcon(QStringLiteral(":/animatedwidgets/gallery/checkboxunchecked.svg"));
    }
    listStyle->checkedIconAlignment = m_ui->iconPositionCombo->currentIndex() == 1 ? Qt::AlignRight : Qt::AlignLeft;
    listStyle->checkedIconSize = QSize(m_ui->iconWidthSpin->value(), m_ui->iconHeightSpin->value());
    listStyle->itemHeight = m_ui->heightSpin->value();
    listStyle->itemSpacing = m_ui->itemSpacingSpin->value();
    listStyle->itemCornerRadius = m_ui->itemRadiusSpin->value();
    listStyle->cornerRadius = m_ui->listRadiusSpin->value();
    listStyle->contentPadding = m_ui->paddingSpin->value();
    listStyle->iconSpacing = m_ui->spacingSpin->value();
    listStyle->viewMargins = QMargins(m_ui->leftMarginSpin->value(), m_ui->topMarginSpin->value(),
                                      m_ui->rightMarginSpin->value(), m_ui->bottomMarginSpin->value());
    popup->setListStyle(listStyle);

    const QStringList options = {tr("Open"), tr("Copy"), tr("Pin"), tr("Unavailable action")};
    popup->setStrings(options);
    auto* model = popup->defaultModel();
    model->setSelectionMode(m_ui->multiSelectionCheck->isChecked()
                               ? animatedwidgets::DefaultPopupMenuModel::MultiSelection
                               : animatedwidgets::DefaultPopupMenuModel::SingleSelection);
    model->setCurrentIndex(2);
    model->setItemEnabled(3, false);
    popup->view()->setCurrentIndex(model->index(0, 0));

    popup->setAdaptiveHeight(m_ui->widthSpin->value());

    connect(model, &animatedwidgets::DefaultPopupMenuModel::currentIndexChanged, this, [this, options](int row) {
        m_ui->resultLabel->setText(tr("Selected: %1").arg(row >= 0 ? options.at(row) : tr("none")));
    });
    connect(model, &animatedwidgets::DefaultPopupMenuModel::selectedIndexesChanged, this, [this, options](const QList<int>& rows) {
        QStringList selected;
        for (int row : rows)
            selected.append(options.at(row));
        m_ui->resultLabel->setText(tr("Selected: %1").arg(selected.isEmpty() ? tr("none") : selected.join(QStringLiteral(", "))));
    });

    popup->moveToTargetBottom(anchor, Qt::AlignLeft);
    popup->show();
}

void PopupMenusPage::showGroupedMenu(QWidget* anchor) {
    if (m_popup)
        return;

    using animatedwidgets::PopupMenuItem;
    auto* popup = new animatedwidgets::PopupMenu(this);
    QList<PopupMenuItem> items;
    for (const auto& group : {tr("Saved views"), tr("Shared views"), tr("Quick views")}) {
        if (!items.isEmpty()) {
            PopupMenuItem separator;
            separator.type = PopupMenuItem::Separator;
            items.append(separator);
        }
        PopupMenuItem title;
        title.text = group;
        title.type = PopupMenuItem::GroupTitle;
        items.append(title);

        PopupMenuItem option;
        option.text = tr("%1 preset").arg(group);
        option.values.insert(Qt::UserRole + 100, static_cast<int>(items.size()));
        items.append(option);
    }
    popup->setItems(items);
    popup->defaultModel()->setCurrentIndex(4);
    auto* style = new animatedwidgets::DefaultPopupMenuListStyle;
    style->checkedIcon = QIcon(QStringLiteral(":/animatedwidgets/gallery/buttoncheck.svg"));
    style->checkedIconAlignment = Qt::AlignRight;
    popup->setListStyle(style);
    showVariant(popup, anchor);
}

void PopupMenusPage::showModulesMenu(QWidget* anchor) {
    if (m_popup)
        return;

    using animatedwidgets::PopupMenuItem;
    auto* popup = new animatedwidgets::PopupMenu(this);
    auto* header = new QPushButton(tr("Run task"), popup);
    header->setMinimumHeight(36);
    popup->setTopWidget(header);
    connect(header, &QPushButton::clicked, this, [this, popup] {
        m_ui->resultLabel->setText(tr("Task requested"));
        popup->close();
    });

    QList<PopupMenuItem> items;
    PopupMenuItem title;
    title.text = tr("Available operations");
    title.type = PopupMenuItem::GroupTitle;
    items.append(title);
    for (const auto& text : {tr("Inspect records"), tr("Refresh cache"), tr("Export report"),
                             tr("Validate entries"), tr("Sync changes"), tr("Archive results"), tr("Schedule backup")}) {
        PopupMenuItem item;
        item.text = text;
        items.append(item);
    }
    popup->setItems(items);
    popup->setBottomWidget(new QLabel(tr("Choose an operation to continue"), popup));
    showVariant(popup, anchor);
}

void PopupMenusPage::showSearchMenu(QWidget* anchor) {
    if (m_popup)
        return;

    const QColor highlightColor(m_ui->highlightColorEdit->text().trimmed());
    if (!highlightColor.isValid()) {
        m_ui->resultLabel->setText(tr("Invalid highlight color"));
        m_ui->highlightColorEdit->setFocus();
        m_ui->highlightColorEdit->selectAll();
        return;
    }

    using animatedwidgets::PopupMenuItem;
    auto* popup = new animatedwidgets::SearchPopupMenu(this);
    popup->setHighlightEnabled(m_ui->highlightCheck->isChecked());
    popup->setHighlightColor(highlightColor);
    popup->setHighlightBold(m_ui->highlightBoldCheck->isChecked());
    popup->searchEditor()->setPlaceholderText(tr("Search assets"));
    auto* emptyLabel = new QLabel(tr("No matching assets\nTry another keyword"), popup);
    emptyLabel->setAlignment(Qt::AlignCenter);
    emptyLabel->setMinimumHeight(80);
    popup->setEmptyWidget(emptyLabel);
    QList<PopupMenuItem> items;
    for (const auto& group : {tr("Images"), tr("Documents")}) {
        PopupMenuItem title;
        title.text = group;
        title.type = PopupMenuItem::GroupTitle;
        items.append(title);
        for (const auto& name : {tr("Orchard"), tr("Harbor"), tr("Summit")}) {
            PopupMenuItem item;
            item.text = tr("%1 - %2").arg(name, group);
            item.values.insert(Qt::UserRole + 100, static_cast<int>(items.size()));
            items.append(item);
        }
    }
    popup->setItems(items);
    popup->setBottomWidget(new QLabel(tr("Type to filter; Down to select"), popup));
    connect(popup, &animatedwidgets::SearchPopupMenu::sourceItemTriggered, this, [this](const QModelIndex& index) {
        m_ui->resultLabel->setText(tr("Asset ID %1: %2").arg(index.data(Qt::UserRole + 100).toInt())
                                     .arg(index.data().toString()));
    });
    showVariant(popup, anchor);
}

void PopupMenusPage::showIconMenu(QWidget* anchor) {
    if (m_popup)
        return;

    using animatedwidgets::PopupMenuItem;
    auto* popup = new animatedwidgets::PopupMenu(this);
    PopupMenuItem folder;
    folder.text = tr("Project folder");
    folder.icon = QIcon(QStringLiteral(":/animatedwidgets/gallery/folder.svg"));
    PopupMenuItem document;
    document.text = tr("Release notes");
    document.icon = QIcon(QStringLiteral(":/animatedwidgets/gallery/document.svg"));
    PopupMenuItem bookmark;
    bookmark.text = tr("Saved view");
    bookmark.icon = QIcon(QStringLiteral(":/animatedwidgets/gallery/bookmark.svg"));
    PopupMenuItem plain;
    plain.text = tr("No icon");
    PopupMenuItem disabled = document;
    disabled.text = tr("Unavailable document");
    disabled.enabled = false;
    popup->setItems({folder, document, bookmark, plain, disabled});
    popup->defaultModel()->setCurrentIndex(2);

    auto* style = new animatedwidgets::DefaultPopupMenuListStyle;
    style->itemIconSize = QSize(20, 20);
    style->itemIconSpacing = 8;
    style->checkedIcon = QIcon(QStringLiteral(":/animatedwidgets/gallery/buttoncheck.svg"));
    style->checkedIconAlignment = Qt::AlignRight;
    popup->setListStyle(style);
    popup->setBottomWidget(new QLabel(tr("Item icons: 20 px; text gap: 8 px"), popup));
    showVariant(popup, anchor);
}

void PopupMenusPage::showVariant(animatedwidgets::PopupMenu* popup, QWidget* anchor) {
    m_popup = popup;
    if (!qobject_cast<animatedwidgets::SearchPopupMenu*>(popup)) {
        connect(popup, &animatedwidgets::PopupMenu::itemTriggered, this, [this](const QModelIndex& index) {
            m_ui->resultLabel->setText(tr("Selected: %1").arg(index.data().toString()));
        });
    }
    popup->setAdaptiveHeight(300, 8);
    popup->moveToTargetBottom(anchor, Qt::AlignLeft);
    popup->show();
}
