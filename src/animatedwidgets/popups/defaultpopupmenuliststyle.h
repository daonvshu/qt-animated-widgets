#pragma once

#include "animatedwidgets/popups/popupmenuliststyle.h"
#include "animatedwidgets/popups/popupmenuitemdelegate.h"

namespace animatedwidgets {

class DefaultPopupMenuListStyle : public PopupMenuListStyle {
public:
    void apply(QAbstractItemView* view) const override;
    QMargins margins() const override;

    QColor background = Qt::transparent; // List viewport background.
    QColor itemBackground = Qt::transparent; // Normal item background.
    QColor itemHoverBackground = QColor(QStringLiteral("#F2F3F5")); // Hovered item background.
    QColor itemSelectedBackground = QColor(QStringLiteral("#E3F3FF")); // Checked item background, or selected for items without a check state; takes precedence over hover.
    QColor itemText = QColor(QStringLiteral("#20242a")); // Enabled item text color.
    QColor itemDisabledText = QColor(QStringLiteral("#9298a2")); // Disabled item text color.
    QColor separator = QColor(QStringLiteral("#e5e8ec")); // Separator line color.
    QColor groupTitleText = QColor(QStringLiteral("#9298a2")); // Non-selectable group heading text.
    int groupTitleHeight = 28; // Minimum group heading height in logical pixels.

    QIcon checkedIcon; // Icon drawn for Qt::Checked items.
    QIcon uncheckedIcon; // Optional icon drawn for Qt::Unchecked items.
    QSize checkedIconSize = QSize(14, 14); // Size shared by checked and unchecked icons.
    Qt::Alignment checkedIconAlignment = Qt::AlignLeft; // Check indicator at the left or right content edge.

    QMargins viewMargins = QMargins(8, 8, 8, 8); // Spacing between the list and popup body.
    int itemHeight = 32; // Minimum item height in logical pixels.
    int itemSpacing = 0; // Vertical gap between item backgrounds, added to each row except the last.
    int itemCornerRadius = 4; // Background corner radius for normal, hovered and selected items.
    int cornerRadius = 4; // List background and viewport clipping radius.
    int contentPadding = 8; // Left and right inset of item text and icons.
    int iconSpacing = 4; // Gap after checked and decoration icons.

    bool checkedIconVisible = true; // Draw the configured checked icon.
    bool separatorVisible = false; // Draw a line below each item except the last.
};

} // namespace animatedwidgets
