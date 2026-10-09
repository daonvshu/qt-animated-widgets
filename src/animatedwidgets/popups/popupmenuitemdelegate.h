#pragma once

#include <QStyledItemDelegate>
#include <QColor>
#include <QFont>
#include <QIcon>
#include <QSize>
#include <QPointer>
#include <QPersistentModelIndex>
#include "animatedwidgets/popups/popupmenuitem.h"

namespace animatedwidgets {

class PopupMenuItemDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    enum ItemRole { SeparatorRole = PopupMenuItem::SeparatorRole, ShortcutRole = PopupMenuItem::ShortcutRole,
                    TypeRole = PopupMenuItem::TypeRole };

    explicit PopupMenuItemDelegate(QObject* parent = nullptr);

    void paint(QPainter* painter, const QStyleOptionViewItem& option,
               const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

    QColor itemBackground = QColor(Qt::transparent);
    QColor itemHoverBackground = QColor(QStringLiteral("#F2F3F5"));
    QColor itemSelectedBackground = QColor(QStringLiteral("#E3F3FF"));
    QColor itemText = QColor(QStringLiteral("#20242a"));
    QColor itemDisabledText = QColor(QStringLiteral("#9298a2"));
    QColor groupTitleText = QColor(QStringLiteral("#9298a2"));
    int groupTitleHeight = 28;

    QFont itemFont;
    QFont groupTitleFont;

    QIcon checkedIcon;
    QIcon uncheckedIcon;
    QSize checkedIconSize = QSize(14, 14);
    Qt::Alignment checkedIconAlignment = Qt::AlignLeft;
    int itemHeight = 32;
    int itemSpacing = 0;
    int itemCornerRadius = 4;
    int contentPadding = 8;
    QSize itemIconSize = QSize(16, 16);
    int itemIconSpacing = 4;
    int checkedIconSpacing = 4;
    bool drawCheckedIcon = true;
    bool drawSeparator = false;
    QColor separatorColor = QColor(QStringLiteral("#e5e8ec"));

private:
    bool hasItemIcons(const QModelIndex& index) const;

    mutable QPointer<const QAbstractItemModel> m_iconModel;
    mutable QList<QMetaObject::Connection> m_iconConnections;
    mutable QPersistentModelIndex m_iconRoot;
    mutable int m_iconColumn = 0;
    mutable bool m_iconCacheValid = false;
    mutable bool m_hasItemIcons = false;
};

} // namespace animatedwidgets
