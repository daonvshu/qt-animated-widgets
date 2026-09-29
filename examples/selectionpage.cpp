#include "selectionpage.h"
#include "ui_selectionpage.h"

SelectionPage::SelectionPage(QWidget* parent)
    : QWidget(parent)
{
    Ui::SelectionPage ui;
    ui.setupUi(this);
    // checkState is not a Qt property, so Designer cannot round-trip it in the .ui file.
    ui.checkPartial->setCheckState(Qt::PartiallyChecked);
}
