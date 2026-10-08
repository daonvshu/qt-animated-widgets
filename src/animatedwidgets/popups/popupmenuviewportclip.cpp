#include "animatedwidgets/popups/popupmenuviewportclip.h"

#include <QEvent>
#include <QPainterPath>
#include <QRegion>
#include <QWidget>

namespace animatedwidgets {

PopupMenuViewportClip::PopupMenuViewportClip(QWidget* viewport)
    : QObject(viewport)
    , m_viewport(viewport)
{
    viewport->installEventFilter(this);
}

void PopupMenuViewportClip::apply(QWidget* viewport, int radius) {
    auto* clip = viewport->findChild<PopupMenuViewportClip*>(QString(), Qt::FindDirectChildrenOnly);
    if (!clip)
        clip = new PopupMenuViewportClip(viewport);

    clip->m_radius = qMax(0, radius);
    clip->updateMask();
}

bool PopupMenuViewportClip::eventFilter(QObject* watched, QEvent* event) {
    if (watched == m_viewport && (event->type() == QEvent::Resize || event->type() == QEvent::Show))
        updateMask();
    return QObject::eventFilter(watched, event);
}

void PopupMenuViewportClip::updateMask() {
    if (m_radius == 0) {
        m_viewport->clearMask();
        return;
    }

    QPainterPath path;
    path.addRoundedRect(QRectF(m_viewport->rect()), m_radius, m_radius);
    m_viewport->setMask(QRegion(path.toFillPolygon().toPolygon()));
}

} // namespace animatedwidgets
