#include "animatedwidgets/popups/popupmenuitemdelegate.h"

#include <QPainter>
#include <QAbstractItemModel>
#include <QStyle>
#include <QStyleOptionViewItem>

namespace animatedwidgets {

PopupMenuItemDelegate::PopupMenuItemDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
}

void PopupMenuItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
                                  const QModelIndex& index) const {
    painter->save();
    painter->setClipRect(option.rect);
    if (index.data(SeparatorRole).toBool()) {
        painter->setPen(separatorColor);
        painter->drawLine(option.rect.left() + qMax(0, contentPadding), option.rect.center().y(),
                          option.rect.right() - qMax(0, contentPadding), option.rect.center().y());
        painter->restore();
        return;
    }

    QStyleOptionViewItem itemOption(option);
    initStyleOption(&itemOption, index);
    painter->setFont(itemOption.font);

    const bool hasNextItem = index.row() < index.model()->rowCount(index.parent()) - 1;
    const int spacing = hasNextItem ? qBound(0, itemSpacing, qMax(0, option.rect.height() - 1)) : 0;
    const QRect itemRect = option.rect.adjusted(0, 0, 0, -spacing);

    const QVariant checkState = index.data(Qt::CheckStateRole);
    const bool selected = checkState.isValid() ? checkState.toInt() == Qt::Checked
                                               : option.state.testFlag(QStyle::State_Selected);
    QColor background = itemBackground;
    if (selected)
        background = itemSelectedBackground;
    else if (option.state & QStyle::State_MouseOver)
        background = itemHoverBackground;
    if (itemCornerRadius > 0) {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(Qt::NoPen);
        painter->setBrush(background);
        painter->drawRoundedRect(QRectF(itemRect), itemCornerRadius, itemCornerRadius);
        painter->restore();
    } else {
        painter->fillRect(itemRect, background);
    }

    const bool enabled = index.flags().testFlag(Qt::ItemIsEnabled);
    painter->setPen(enabled ? itemText : itemDisabledText);

    const int padding = qMax(0, contentPadding);
    QRect contentRect = itemRect.adjusted(padding, 0, -padding, 0);
    if (drawCheckedIcon && checkState.isValid() && (!checkedIcon.isNull() || !uncheckedIcon.isNull())) {
        const bool rightAligned = checkedIconAlignment.testFlag(Qt::AlignRight);
        const int iconX = rightAligned ? contentRect.right() - checkedIconSize.width() + 1 : contentRect.left();
        const QRect iconRect(iconX, contentRect.center().y() - checkedIconSize.height() / 2,
                             checkedIconSize.width(), checkedIconSize.height());
        const QIcon indicator = checkState.toInt() == Qt::Checked ? checkedIcon
            : checkState.toInt() == Qt::Unchecked ? uncheckedIcon : QIcon();
        indicator.paint(painter, iconRect, Qt::AlignCenter, enabled ? QIcon::Normal : QIcon::Disabled);
        if (rightAligned)
            contentRect.setRight(iconRect.left() - 1 - checkedIconSpacing);
        else
            contentRect.setLeft(iconRect.right() + 1 + checkedIconSpacing);
    }

    const QIcon icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
    if (!icon.isNull()) {
        const int iconSize = option.decorationSize.width();
        const QRect iconRect(contentRect.left(), contentRect.center().y() - iconSize / 2, iconSize, iconSize);
        icon.paint(painter, iconRect, Qt::AlignCenter, enabled ? QIcon::Normal : QIcon::Disabled);
        contentRect.setLeft(iconRect.right() + 1 + checkedIconSpacing);
    }

    const QString shortcut = index.data(ShortcutRole).toString();
    if (!shortcut.isEmpty()) {
        const int shortcutWidth = itemOption.fontMetrics.horizontalAdvance(shortcut);
        const QRect shortcutRect(contentRect.right() - shortcutWidth + 1, contentRect.top(),
                                 shortcutWidth, contentRect.height());
        painter->drawText(shortcutRect, Qt::AlignVCenter | Qt::AlignRight, shortcut);
        contentRect.setRight(shortcutRect.left() - 1 - qMax(8, checkedIconSpacing));
    }

    const QString text = index.data(Qt::DisplayRole).toString();
    painter->drawText(contentRect, Qt::AlignVCenter | Qt::AlignLeft,
                      itemOption.fontMetrics.elidedText(text, Qt::ElideRight, qMax(0, contentRect.width())));

    if (drawSeparator && hasNextItem) {
        painter->setPen(separatorColor);
        painter->drawLine(itemRect.bottomLeft(), itemRect.bottomRight());
    }

    painter->restore();
}

QSize PopupMenuItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
    if (index.data(SeparatorRole).toBool())
        return QSize(2 * qMax(0, contentPadding) + 1, 8);

    QSize size = QStyledItemDelegate::sizeHint(option, index);
    const QString shortcut = index.data(ShortcutRole).toString();
    if (!shortcut.isEmpty()) {
        QStyleOptionViewItem itemOption(option);
        initStyleOption(&itemOption, index);
        size.rwidth() += itemOption.fontMetrics.horizontalAdvance(shortcut) + qMax(8, checkedIconSpacing);
    }
    if (drawCheckedIcon && index.data(Qt::CheckStateRole).isValid()
        && (!checkedIcon.isNull() || !uncheckedIcon.isNull())) {
        size.rwidth() += checkedIconSize.width() + checkedIconSpacing;
        size.setHeight(qMax(size.height(), checkedIconSize.height()));
    }
    size.rwidth() += 2 * qMax(0, contentPadding);
    size.setHeight(qMax(itemHeight, size.height()));
    if (index.row() < index.model()->rowCount(index.parent()) - 1)
        size.rheight() += qMax(0, itemSpacing);
    return size;
}

} // namespace animatedwidgets
