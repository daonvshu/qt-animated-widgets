#include "stylemessagepopup.h"

#include <QKeyEvent>

StyleMessagePopup::StyleMessagePopup(const QString& title, const QString& message, QWidget* parent)
    : StylePopupDialog(parent)
{
    createLayout();
    setWindowTitle(title);
    ui.titleLabel->setText(title);
    ui.messageLabel->setText(message);

    connect(this, &StylePopupDialog::finished, this, [this](int result) {
        emit actionSelected(static_cast<Action>(result));
    });
}

void StyleMessagePopup::createLayout() {
    ui.setupUi(this);
    ui.secondaryButton->hide();
    ui.cancelButton->hide();
    setFocusProxy(ui.primaryButton);
}

void StyleMessagePopup::setPrimaryAction(Action action, const QString& text) {
    m_primaryAction = action;
    ui.primaryButton->setText(text);
}

void StyleMessagePopup::setSecondaryAction(Action action, const QString& text) {
    m_secondaryAction = action;
    ui.secondaryButton->setText(text);
    ui.secondaryButton->show();
}

void StyleMessagePopup::setCancelVisible(bool visible) {
    ui.cancelButton->setVisible(visible);
}

void StyleMessagePopup::on_primaryButton_clicked() {
    finish(static_cast<int>(m_primaryAction));
}

void StyleMessagePopup::on_secondaryButton_clicked() {
    finish(static_cast<int>(m_secondaryAction));
}

void StyleMessagePopup::on_cancelButton_clicked() {
    finish(static_cast<int>(Action::Cancel));
}

void StyleMessagePopup::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        if (ui.cancelButton->hasFocus())
            on_cancelButton_clicked();
        else if (ui.secondaryButton->hasFocus())
            on_secondaryButton_clicked();
        else
            on_primaryButton_clicked();
        event->accept();
        return;
    }
    StylePopupDialog::keyPressEvent(event);
}
