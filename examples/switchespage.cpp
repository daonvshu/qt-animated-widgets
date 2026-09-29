#include "switchespage.h"
#include "ui_switchespage.h"

#include <QIcon>

SwitchesPage::SwitchesPage(QWidget* parent)
    : QWidget(parent)
{
    Ui::SwitchesPage ui;
    ui.setupUi(this);
    ui.switchChecked->setHandleRadius(7);
    ui.switchChecked->setCheckedThumbColor(QColor(QStringLiteral("#e8f5e9")));
    ui.switchCustom->setHandleIcon(QIcon(QStringLiteral(":/animatedwidgets/gallery/switchhandle.svg")));
}
