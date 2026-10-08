#pragma once

#include <QStyledItemDelegate>
#include <QColor>
#include <QIcon>
#include <QSize>

namespace animatedwidgets {

class PopupMenuItemDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    explicit PopupMenuItemDelegate(QObject* parent = nullptr);

    void paint(QPainter* painter, const QStyleOptionViewItem& option,
               const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

    QColor itemBackground = QColor(Qt::transparent);
    QColor itemHoverBackground = QColor(QStringLiteral("#F2F3F5"));
    QColor itemSelectedBackground = QColor(QStringLiteral("#E3F3FF"));
    QColor itemText = QColor(QStringLiteral("#20242a"));
    QColor itemDisabledText = QColor(QStringLiteral("#9298a2"));
    QIcon checkedIcon;
    QIcon uncheckedIcon;
    QSize checkedIconSize = QSize(14, 14);
    Qt::Alignment checkedIconAlignment = Qt::AlignLeft;
    int itemHeight = 32;
    int itemSpacing = 0;
    int itemCornerRadius = 4;
    int contentPadding = 8;
    int checkedIconSpacing = 4;
    bool drawCheckedIcon = true;
    bool drawSeparator = false;
    QColor separatorColor = QColor(QStringLiteral("#e5e8ec"));
};

} // namespace animatedwidgets
