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

    QModelIndex hoveredIndex() const;
    void setHoveredIndex(const QModelIndex& index);

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

    QIcon submenuIcon;
    QSize submenuIconSize = QSize(14, 14);
    int submenuIconSpacing = 4;

    int itemHeight = 32;
    int itemSpacing = 2;
    int itemCornerRadius = 4;
    int contentPadding = 8;
    QSize itemIconSize = QSize(16, 16);
    int itemIconSpacing = 4;
    int checkedIconSpacing = 4;
    bool drawCheckedIcon = true;
    bool drawSeparator = false;
    QColor separatorColor = QColor(QStringLiteral("#e5e8ec"));

protected:
    virtual bool hasChildItems(const QModelIndex& index) const;
    virtual QVariant itemCheckState(const QModelIndex& index) const;
    virtual bool isItemSelected(const QStyleOptionViewItem& option, const QModelIndex& index) const;

private:
    bool hasItemIcons(const QModelIndex& index) const;

    QPersistentModelIndex m_hoveredIndex;
    bool m_hoverManaged = false;

    mutable QPointer<const QAbstractItemModel> m_iconModel;
    mutable QList<QMetaObject::Connection> m_iconConnections;
    mutable QPersistentModelIndex m_iconRoot;
    mutable int m_iconColumn = 0;
    mutable bool m_iconCacheValid = false;
    mutable bool m_hasItemIcons = false;
};

} // namespace animatedwidgets
