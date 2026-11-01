#include "animatedwidgets/popups/popupmenuitemdelegate.h"
#include "animatedwidgets/popups/popupmenurowtype.h"
#include "animatedwidgets/utils/popupmenuresources.h"
#include "animatedwidgets/utils/iconpainter.h"

#include <QPainter>
#include <QAbstractItemModel>
#include <QFontMetricsF>
#include <QStyle>
#include <QStyleOptionViewItem>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include <QTextOption>
#include <QtMath>

namespace animatedwidgets {

PopupMenuItemDelegate::PopupMenuItemDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
    PopupMenuResources::initialize();
    submenuIcon = QIcon(QStringLiteral(":/animatedwidgets/popups/submenu.svg"));
}

QModelIndex PopupMenuItemDelegate::hoveredIndex() const {
    return m_hoveredIndex;
}

void PopupMenuItemDelegate::setHoveredIndex(const QModelIndex& index) {
    m_hoverManaged = true;
    m_hoveredIndex = index;
}

bool PopupMenuItemDelegate::hasChildItems(const QModelIndex& index) const {
    return !index.data(PopupMenuItem::ChildrenRole).value<QList<PopupMenuNode>>().isEmpty();
}

QVariant PopupMenuItemDelegate::itemCheckState(const QModelIndex& index) const {
    return index.data(Qt::CheckStateRole);
}

bool PopupMenuItemDelegate::isItemSelected(const QStyleOptionViewItem& option, const QModelIndex& index) const {
    const QVariant checkState = itemCheckState(index);
    return !hasChildItems(index) && (checkState.isValid() ? checkState.toInt() == Qt::Checked
                                                        : option.state.testFlag(QStyle::State_Selected));
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
    const QFont& font = index.data(TypeRole).toInt() == static_cast<int>(PopupMenuRowType::GroupTitle)
        ? groupTitleFont : itemFont;
    itemOption.font = font.resolve(itemOption.font);
    const QFontMetricsF metrics(itemOption.font);
    painter->setFont(itemOption.font);

    const bool hasNextItem = index.row() < index.model()->rowCount(index.parent()) - 1;
    const int spacing = hasNextItem ? qBound(0, itemSpacing, qMax(0, option.rect.height() - 1)) : 0;
    const QRect itemRect = option.rect.adjusted(0, 0, 0, -spacing);

    if (index.data(TypeRole).toInt() == static_cast<int>(PopupMenuRowType::GroupTitle)) {
        painter->setPen(groupTitleText);
        const QRect content = itemRect.adjusted(qMax(0, contentPadding), 0, -qMax(0, contentPadding), 0);
        painter->drawText(content, Qt::AlignVCenter | Qt::AlignLeft,
                          metrics.elidedText(itemOption.text, itemOption.textElideMode, qMax(0, content.width())));
        painter->restore();
        return;
    }

    const QVariant checkState = itemCheckState(index);
    const bool hasChildren = hasChildItems(index);
    const bool selected = isItemSelected(option, index);
    QColor background = itemBackground;
    if (selected)
        background = itemSelectedBackground;
    else if (m_hoverManaged ? m_hoveredIndex == index : option.state.testFlag(QStyle::State_MouseOver))
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
    const QColor iconColor = iconHint ? (enabled ? itemText : itemDisabledText) : QColor();
    const QIcon::Mode iconMode = enabled ? QIcon::Normal : QIcon::Disabled;

    const int padding = qMax(0, contentPadding);
    QRect contentRect = itemRect.adjusted(padding, 0, -padding, 0);
    if (hasChildren) {
        const int iconX = contentRect.right() - submenuIconSize.width() + 1;
        const qreal iconY = contentRect.top() + (contentRect.height() - submenuIconSize.height()) / 2.0;
        const QRectF iconRect(iconX, iconY, submenuIconSize.width(), submenuIconSize.height());
        IconPainter::draw(*painter, iconRect, submenuIcon, iconColor, iconMode);
        contentRect.setRight(iconX - 1 - submenuIconSpacing);
    } else if (drawCheckedIcon && checkState.isValid() && (!checkedIcon.isNull() || !uncheckedIcon.isNull())) {
        const bool rightAligned = checkedIconAlignment.testFlag(Qt::AlignRight);
        const int iconX = rightAligned ? contentRect.right() - checkedIconSize.width() + 1 : contentRect.left();
        const qreal iconY = contentRect.top() + (contentRect.height() - checkedIconSize.height()) / 2.0;
        const QRectF iconRect(iconX, iconY, checkedIconSize.width(), checkedIconSize.height());
        const QIcon indicator = checkState.toInt() == Qt::Checked ? checkedIcon
            : checkState.toInt() == Qt::Unchecked ? uncheckedIcon : QIcon();
        IconPainter::draw(*painter, iconRect, indicator, iconColor, iconMode);
        if (rightAligned)
            contentRect.setRight(iconX - 1 - checkedIconSpacing);
        else
            contentRect.setLeft(iconX + checkedIconSize.width() + checkedIconSpacing);
    }

    const QIcon icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
    if (hasItemIcons(index)) {
        const QSize iconSize = itemIconSize.expandedTo(QSize(1, 1));
        const int iconX = contentRect.left();
        const qreal iconY = contentRect.top() + (contentRect.height() - iconSize.height()) / 2.0;
        const QRectF iconRect(iconX, iconY, iconSize.width(), iconSize.height());
        IconPainter::draw(*painter, iconRect, icon, iconColor, iconMode);
        contentRect.setLeft(iconX + iconSize.width() + itemIconSpacing);
    }

    const QString shortcut = index.data(ShortcutRole).toString();
    if (!shortcut.isEmpty()) {
        const int shortcutWidth = qCeil(metrics.horizontalAdvance(shortcut));
        const QRect shortcutRect(contentRect.right() - shortcutWidth + 1, contentRect.top(),
                                 shortcutWidth, contentRect.height());
        painter->drawText(shortcutRect, Qt::AlignVCenter | Qt::AlignRight, shortcut);
        contentRect.setRight(shortcutRect.left() - 1 - qMax(8, checkedIconSpacing));
    }

    const QString text = index.data(Qt::DisplayRole).toString();
    const QString highlighted = index.data(PopupMenuItem::HighlightRole).toString();
    if (!highlighted.isEmpty() && !contentRect.isEmpty()) {
        QTextDocument document;
        document.setDocumentMargin(0);
        document.setDefaultFont(itemOption.font);
        QTextOption textOption;
        textOption.setWrapMode(QTextOption::NoWrap);
        document.setDefaultTextOption(textOption);
        document.setHtml(highlighted);
        document.setTextWidth(contentRect.width());

        QAbstractTextDocumentLayout::PaintContext context;
        context.palette.setColor(QPalette::Text, enabled ? itemText : itemDisabledText);
        painter->save();
        painter->setClipRect(contentRect, Qt::IntersectClip);
        painter->translate(contentRect.left(), contentRect.top() + (contentRect.height() - document.size().height()) / 2.0);
        document.documentLayout()->draw(painter, context);
        painter->restore();
    } else {
        painter->drawText(contentRect, Qt::AlignVCenter | Qt::AlignLeft,
                          metrics.elidedText(text, itemOption.textElideMode, qMax(0, contentRect.width())));
    }

    if (drawSeparator && hasNextItem) {
        painter->setPen(separatorColor);
        painter->drawLine(itemRect.bottomLeft(), itemRect.bottomRight());
    }

    painter->restore();
}

QSize PopupMenuItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
    if (index.data(SeparatorRole).toBool())
        return QSize(2 * qMax(0, contentPadding) + 1, 8);

    QStyleOptionViewItem itemOption(option);
    initStyleOption(&itemOption, index);
    const QFont& font = index.data(TypeRole).toInt() == static_cast<int>(PopupMenuRowType::GroupTitle)
        ? groupTitleFont : itemFont;
    itemOption.font = font.resolve(itemOption.font);
    const QFontMetricsF metrics(itemOption.font);
    const QVariant explicitSize = index.data(Qt::SizeHintRole);
    QSize size = explicitSize.isValid() ? explicitSize.toSize()
        : QSize(qCeil(metrics.horizontalAdvance(itemOption.text)), qCeil(metrics.height()));
    const QString shortcut = index.data(ShortcutRole).toString();
    if (!shortcut.isEmpty()) {
        size.rwidth() += qCeil(metrics.horizontalAdvance(shortcut)) + qMax(8, checkedIconSpacing);
    }
    const bool hasChildren = hasChildItems(index);
    if (hasChildren) {
        size.rwidth() += submenuIconSize.width() + submenuIconSpacing;
        size.setHeight(qMax(size.height(), submenuIconSize.height()));
    } else if (drawCheckedIcon && index.data(Qt::CheckStateRole).isValid()
        && (!checkedIcon.isNull() || !uncheckedIcon.isNull())) {
        size.rwidth() += checkedIconSize.width() + checkedIconSpacing;
        size.setHeight(qMax(size.height(), checkedIconSize.height()));
    }
    if (index.data(TypeRole).toInt() == static_cast<int>(PopupMenuRowType::Option) && hasItemIcons(index)) {
        const QSize iconSize = itemIconSize.expandedTo(QSize(1, 1));
        size.rwidth() += iconSize.width() + itemIconSpacing;
        size.setHeight(qMax(size.height(), iconSize.height()));
    }
    size.rwidth() += 2 * qMax(0, contentPadding);
    size.setHeight(qMax(index.data(TypeRole).toInt() == static_cast<int>(PopupMenuRowType::GroupTitle) ? groupTitleHeight : itemHeight,
                        size.height()));
    if (index.row() < index.model()->rowCount(index.parent()) - 1)
        size.rheight() += qMax(0, itemSpacing);
    return size;
}

bool PopupMenuItemDelegate::hasItemIcons(const QModelIndex& index) const {
    if (!index.isValid())
        return false;

    if (m_iconModel != index.model()) {
        for (const auto& connection : m_iconConnections)
            disconnect(connection);
        m_iconConnections.clear();
        m_iconModel = index.model();
        m_iconCacheValid = false;

        auto* self = const_cast<PopupMenuItemDelegate*>(this);
        const auto invalidate = [self] {
            self->m_iconCacheValid = false;
            if (self->m_iconModel && self->m_iconModel->rowCount(self->m_iconRoot) > 0)
                emit self->sizeHintChanged(self->m_iconModel->index(0, self->m_iconColumn, self->m_iconRoot));
        };
        m_iconConnections.append(connect(m_iconModel.data(), &QAbstractItemModel::dataChanged, self,
            [invalidate](const QModelIndex&, const QModelIndex&, const QVector<int>& roles) {
                if (roles.isEmpty() || roles.contains(Qt::DecorationRole) || roles.contains(PopupMenuItem::TypeRole)
                    || roles.contains(PopupMenuItem::SeparatorRole) || roles.contains(Qt::AccessibleDescriptionRole))
                    invalidate();
            }));
        m_iconConnections.append(connect(m_iconModel.data(), &QAbstractItemModel::rowsInserted, self, invalidate));
        m_iconConnections.append(connect(m_iconModel.data(), &QAbstractItemModel::rowsRemoved, self, invalidate));
        m_iconConnections.append(connect(m_iconModel.data(), &QAbstractItemModel::rowsMoved, self, invalidate));
        m_iconConnections.append(connect(m_iconModel.data(), &QAbstractItemModel::columnsInserted, self, invalidate));
        m_iconConnections.append(connect(m_iconModel.data(), &QAbstractItemModel::columnsRemoved, self, invalidate));
        m_iconConnections.append(connect(m_iconModel.data(), &QAbstractItemModel::columnsMoved, self, invalidate));
        m_iconConnections.append(connect(m_iconModel.data(), &QAbstractItemModel::modelReset, self, invalidate));
        m_iconConnections.append(connect(m_iconModel.data(), &QAbstractItemModel::layoutChanged, self, invalidate));
    }

    if (m_iconCacheValid && m_iconRoot == index.parent() && m_iconColumn == index.column())
        return m_hasItemIcons;

    // Cache per sibling list/column so painting each row does not rescan the whole model.
    m_iconRoot = index.parent();
    m_iconColumn = index.column();
    m_hasItemIcons = false;
    for (int row = 0; row < index.model()->rowCount(m_iconRoot); ++row) {
        const QModelIndex sibling = index.model()->index(row, m_iconColumn, m_iconRoot);
        if (sibling.data(TypeRole).toInt() == static_cast<int>(PopupMenuRowType::Option) && !sibling.data(SeparatorRole).toBool()
            && !qvariant_cast<QIcon>(sibling.data(Qt::DecorationRole)).isNull()) {
            m_hasItemIcons = true;
            break;
        }
    }
    m_iconCacheValid = true;
    return m_hasItemIcons;
}

} // namespace animatedwidgets
