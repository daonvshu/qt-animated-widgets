#include "animatedwidgets/popups/popupdismissfilter.h"

#include <QApplication>
#include <QEvent>
#include <QWidget>

namespace animatedwidgets {

PopupDismissFilter::PopupDismissFilter(QWidget* popup, QWidget* anchor)
    : QObject(popup)
    , m_popup(popup)
    , m_anchor(anchor)
{
    qApp->installEventFilter(this);
}

void PopupDismissFilter::stop() {
    qApp->removeEventFilter(this);
}

bool PopupDismissFilter::eventFilter(QObject* watched, QEvent* event) {
    if (!m_popup || !m_anchor || !m_popup->isVisible())
        return false;

    const auto* widget = qobject_cast<QWidget*>(watched);
    const bool insidePopup = widget && (widget == m_popup || m_popup->isAncestorOf(widget));
    const bool insideAnchor = widget && (widget == m_anchor || m_anchor->isAncestorOf(widget));
    const bool outsideClick = widget && event->type() == QEvent::MouseButtonPress && !insidePopup && !insideAnchor;
    const bool ownerDeactivated = event->type() == QEvent::WindowDeactivate && watched == m_anchor->window();
    const bool ownerChanged = (watched == m_anchor || watched == m_anchor->window())
        && (event->type() == QEvent::Hide || event->type() == QEvent::Close
            || event->type() == QEvent::Move || event->type() == QEvent::Resize);
    if (outsideClick || ownerDeactivated || ownerChanged || event->type() == QEvent::ApplicationDeactivate) {
        stop();
        emit dismissRequested();
    }
    return false;
}

} // namespace animatedwidgets
