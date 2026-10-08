#pragma once

#include <QMargins>

class QAbstractItemView;
class QWidget;

namespace animatedwidgets {

class PopupMenuStyle {
public:
    virtual ~PopupMenuStyle() = default;

    virtual void applyPopup(QWidget* popup) const = 0;
    // Insets reserved by the popup body, excluding the outer shadow.
    virtual QMargins margins() const {
        return QMargins();
    }
};

} // namespace animatedwidgets
