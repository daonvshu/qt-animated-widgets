#include "animatedwidgets/popups/popupanimation.h"

namespace animatedwidgets {

PopupAnimation::PopupAnimation(QObject* parent)
    : QObject(parent)
{
}

void PopupAnimation::prepareShow(QWidget*, Qt::Edge, const QRect&) {
}

} // namespace animatedwidgets
