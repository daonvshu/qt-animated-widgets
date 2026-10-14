#include "stylewidgetitempopup.h"

#include <QSignalBlocker>

StyleWidgetItemPopup::StyleWidgetItemPopup(const QString& category,
    const QList<animatedwidgets::styleeditor::StyleEditorWidgetType>& types, QWidget* parent)
    : StylePopupDialog(parent)
{
    createLayout();
    ui.categoryLabel->setText(category);
    {
        const QSignalBlocker blocker(ui.typeCombo);
        for (const auto& type : types)
            ui.typeCombo->addItem(type.name, type.id);
    }
    on_typeCombo_currentIndexChanged(ui.typeCombo->currentIndex());

    connect(this, &StylePopupDialog::finished, this, [this](int result) {
        if (result == 1)
            emit itemAccepted(m_typeId, m_displayName);
    });
}

void StyleWidgetItemPopup::createLayout() {
    ui.setupUi(this);
    setWindowTitle(QStringLiteral("Add widget style"));
    setFocusProxy(ui.nameEdit);
}

void StyleWidgetItemPopup::setItem(const QString& typeId, const QString& displayName) {
    setWindowTitle(QStringLiteral("Edit widget style"));
    ui.titleLabel->setText(windowTitle());
    ui.acceptButton->setText(QStringLiteral("Apply"));
    ui.hintLabel->setText(QStringLiteral("Renaming keeps all properties. Changing type keeps compatible properties and resets the others."));
    {
        const QSignalBlocker blocker(ui.typeCombo);
        const int index = ui.typeCombo->findData(typeId);
        ui.typeCombo->setCurrentIndex(index >= 0 ? index : 0);
    }
    m_defaultName.clear();
    ui.nameEdit->setText(displayName);
    ui.nameEdit->selectAll();
    on_nameEdit_textChanged();
}

void StyleWidgetItemPopup::on_typeCombo_currentIndexChanged(int) {
    const QString previousDefault = m_defaultName;
    m_defaultName = ui.typeCombo->currentText() + QStringLiteral(" style");
    if (ui.nameEdit->text().isEmpty() || ui.nameEdit->text() == previousDefault)
        ui.nameEdit->setText(m_defaultName);
    on_nameEdit_textChanged();
}

void StyleWidgetItemPopup::on_nameEdit_textChanged() {
    ui.acceptButton->setEnabled(ui.typeCombo->currentIndex() >= 0 && !ui.nameEdit->text().trimmed().isEmpty());
}

void StyleWidgetItemPopup::on_nameEdit_returnPressed() {
    on_acceptButton_clicked();
}

void StyleWidgetItemPopup::on_acceptButton_clicked() {
    if (!ui.acceptButton->isEnabled())
        return;

    m_typeId = ui.typeCombo->currentData().toString();
    m_displayName = ui.nameEdit->text().trimmed();
    finish(1);
}

void StyleWidgetItemPopup::on_cancelButton_clicked() {
    finish(0);
}
