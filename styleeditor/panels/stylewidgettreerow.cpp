#include "stylewidgettreerow.h"

#include "stylewidgettreemodel.h"
#include "../themes/stylethememanager.h"

StyleWidgetTreeRow::StyleWidgetTreeRow(QWidget* parent)
    : QWidget(parent)
{
    createLayout();
}

void StyleWidgetTreeRow::createLayout() {
    ui.setupUi(this);
    ui.nameLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui.detailLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    StyleThemeManager::prepareWidget(this);
}

void StyleWidgetTreeRow::bind(const QModelIndex& index) {
    m_index = QPersistentModelIndex(index);
    const bool category = !index.parent().isValid();
    ui.nameLabel->setText(index.data().toString());
    ui.detailLabel->setText(index.data(StyleWidgetTreeModel::DetailRole).toString());
    setToolTip(index.data(Qt::ToolTipRole).toString());

    QFont font = ui.nameLabel->font();
    font.setBold(category);
    ui.nameLabel->setFont(font);
    ui.actionButton->setIcon(QIcon(category ? QStringLiteral(":/styleeditor/icons/add.svg") : QStringLiteral(":/styleeditor/icons/edit.svg")));
    ui.actionButton->setToolTip(category ? QStringLiteral("Add a style") : QStringLiteral("Edit type and style name"));
    ui.actionButton->setAccessibleName(ui.actionButton->toolTip());
    ui.actionButton->setEnabled(index.data(StyleWidgetTreeModel::ActionEnabledRole).toBool());
}

void StyleWidgetTreeRow::unbind() {
    m_index = QPersistentModelIndex();
    ui.nameLabel->clear();
    ui.detailLabel->clear();
    setToolTip(QString());
    ui.actionButton->setEnabled(false);
}

void StyleWidgetTreeRow::on_actionButton_clicked() {
    if (m_index.isValid())
        emit actionRequested(m_index);
}
