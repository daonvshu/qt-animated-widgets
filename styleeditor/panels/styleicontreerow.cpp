#include "styleicontreerow.h"

#include "styleicontreemodel.h"
#include "../themes/stylethememanager.h"

#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>

StyleIconTreeRow::StyleIconTreeRow(QWidget* parent)
    : QWidget(parent)
{
    createLayout();
}

void StyleIconTreeRow::createLayout() {
    setAttribute(Qt::WA_TransparentForMouseEvents);
    auto* rowLayout = new QHBoxLayout(this);
    rowLayout->setContentsMargins(4, 4, 8, 4);
    rowLayout->setSpacing(8);

    m_iconLabel = new QLabel(this);
    m_iconLabel->setFixedSize(28, 28);
    m_iconLabel->setAlignment(Qt::AlignCenter);
    m_nameLabel = new QLabel(this);
    m_nameLabel->setTextFormat(Qt::PlainText);
    m_nameLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    rowLayout->addWidget(m_iconLabel);
    rowLayout->addWidget(m_nameLabel, 1);

    StyleThemeManager::prepareWidget(this);
}

void StyleIconTreeRow::bind(const QModelIndex& index) {
    const bool icon = !index.data(StyleIconTreeModel::ReferenceRole).toString().isEmpty();
    m_iconLabel->setVisible(icon);
    if (icon) {
        const QIcon preview(index.data(StyleIconTreeModel::FilePathRole).toString());
        const qreal ratio = devicePixelRatioF();
        QPixmap pixmap = preview.pixmap(QSize(28, 28) * ratio);
        pixmap.setDevicePixelRatio(ratio);
        m_iconLabel->setPixmap(pixmap);
    } else {
        m_iconLabel->clear();
    }
    m_nameLabel->setText(index.data().toString());
    QFont font = m_nameLabel->font();
    font.setBold(!icon);
    m_nameLabel->setFont(font);
    setToolTip(index.data(Qt::ToolTipRole).toString());
    setAccessibleName(m_nameLabel->text());
}

void StyleIconTreeRow::unbind() {
    m_iconLabel->clear();
    m_nameLabel->clear();
    setToolTip(QString());
    setAccessibleName(QString());
}
