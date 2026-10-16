#include "animatedwidgets/popups/defaultpopupmenustyle.h"
#include "animatedwidgets/popups/popupmenu.h"

#include <QWidget>

namespace animatedwidgets {

void DefaultPopupMenuStyle::applyPopup(QWidget* popup) const {
    if (!popup)
        return;

    const int cornerRadius = qMax(0, popupCornerRadius);
    if (auto* menu = qobject_cast<PopupMenu*>(popup)) {
        menu->setSurfaceMetrics(shadowVisible ? shadowRadius : 0, cornerRadius);
        menu->setShadowColor(shadowColor);
    }

    popup->setStyleSheet(QStringLiteral("animatedwidgets--PopupMenu { background-color: %1; border: %2px solid %3; border-radius: %4px; }")
                              .arg(popupBackground.name(QColor::HexArgb))
                              .arg(borderVisible ? qMax(0, popupBorderWidth) : 0)
                              .arg(popupBorder.name(QColor::HexArgb))
                              .arg(cornerRadius));
    popup->ensurePolished();
    popup->setAutoFillBackground(false);
    popup->setAttribute(Qt::WA_StyledBackground, false);
    popup->update();
}

QMargins DefaultPopupMenuStyle::margins() const {
    const int border = borderVisible ? qMax(0, popupBorderWidth) : 0;
    return QMargins(border, border, border, border);
}

} // namespace animatedwidgets
