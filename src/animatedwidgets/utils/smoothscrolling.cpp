#include "animatedwidgets/utils/smoothscrolling.h"

#include <QAbstractScrollArea>
#include <QAbstractItemView>

#ifdef ANIMATEDWIDGETS_HAS_SMOOTHSCROLLBAR
#include <smoothscroll/smoothscrollcontroller.h>
#endif

namespace animatedwidgets {

void SmoothScrolling::install(QAbstractScrollArea* area) {
    if (!area)
        return;

    if (auto* view = qobject_cast<QAbstractItemView*>(area)) {
        view->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        view->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    }

#ifdef ANIMATEDWIDGETS_HAS_SMOOTHSCROLLBAR
    if (area->findChild<sscroll::SmoothScrollController*>(QString(), Qt::FindDirectChildrenOnly))
        return;

    auto* controller = new sscroll::SmoothScrollController(area, area);
    sscroll::SmoothScrollSettings settings;
    settings.animationDuration = 300;
    settings.easingCurve = QEasingCurve::OutCubic;
    controller->setSettings(settings);
#endif
}

} // namespace animatedwidgets
