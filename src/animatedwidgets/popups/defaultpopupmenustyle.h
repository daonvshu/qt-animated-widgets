#pragma once

#include "animatedwidgets/popups/popupmenustyle.h"

#include <QColor>

namespace animatedwidgets {

class DefaultPopupMenuStyle : public PopupMenuStyle {
public:
    void applyPopup(QWidget* popup) const override;
    QMargins margins() const override;

    QColor popupBackground = QColor(Qt::white); // Popup body background.
    QColor popupBorder = QColor(QStringLiteral("#d5dae0")); // Popup border color.

    int popupBorderWidth = 1; // Border width in logical pixels.
    int popupCornerRadius = 8; // Corner radius shared by the body and shadow shape.
    int shadowRadius = 12; // Shadow blur radius and reserved outer margin.

    bool shadowVisible = true; // Reserve and draw the outer shadow.
    bool borderVisible = true; // Include the border in the generated stylesheet.
};

} // namespace animatedwidgets
