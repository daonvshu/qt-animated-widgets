#include "animatedwidgets/popups/defaultpopupmenuliststyle.h"
#include "animatedwidgets/popups/popupmenuviewportclip.h"

#include <QAbstractItemView>

namespace animatedwidgets {

void DefaultPopupMenuListStyle::apply(QAbstractItemView* view) const {
    if (!view)
        return;

    view->setFrameShape(QFrame::NoFrame);
    view->setAutoFillBackground(false);
    view->setStyleSheet(QStringLiteral("QAbstractItemView { background-color: %1; border: none; border-radius: %2px; }")
                           .arg(background.name(QColor::HexArgb))
                           .arg(qMax(0, cornerRadius)));
    view->ensurePolished();
    view->viewport()->setAutoFillBackground(true);
    PopupMenuViewportClip::apply(view->viewport(), cornerRadius);

    if (auto* delegate = qobject_cast<PopupMenuItemDelegate*>(view->itemDelegate())) {
        delegate->itemBackground = itemBackground;
        delegate->itemHoverBackground = itemHoverBackground;
        delegate->itemSelectedBackground = itemSelectedBackground;
        delegate->itemText = itemText;
        delegate->itemDisabledText = itemDisabledText;
        delegate->separatorColor = separator;
        delegate->groupTitleText = groupTitleText;
        delegate->groupTitleHeight = qMax(0, groupTitleHeight);
        delegate->itemFont = itemFont;
        delegate->groupTitleFont = groupTitleFont;
        delegate->checkedIcon = checkedIcon;
        delegate->uncheckedIcon = uncheckedIcon;
        delegate->checkedIconSize = checkedIconSize;
        delegate->checkedIconAlignment = checkedIconAlignment;
        delegate->submenuIcon = submenuIcon.isNull()
            ? QIcon(QStringLiteral(":/animatedwidgets/popups/submenu.svg")) : submenuIcon;
        delegate->submenuIconSize = submenuIconSize.expandedTo(QSize(1, 1));
        delegate->submenuIconSpacing = qMax(0, submenuIconSpacing);
        delegate->itemHeight = itemHeight;
        delegate->itemSpacing = qMax(0, itemSpacing);
        delegate->itemCornerRadius = qMax(0, itemCornerRadius);
        delegate->contentPadding = qMax(0, contentPadding);
        delegate->itemIconSize = itemIconSize.expandedTo(QSize(1, 1));
        delegate->itemIconSpacing = qMax(0, itemIconSpacing);
        delegate->checkedIconSpacing = iconSpacing;
        delegate->drawCheckedIcon = checkedIconVisible;
        delegate->drawSeparator = separatorVisible;
    }
    view->doItemsLayout();
    view->viewport()->update();
}

QMargins DefaultPopupMenuListStyle::margins() const {
    return viewMargins;
}

} // namespace animatedwidgets
