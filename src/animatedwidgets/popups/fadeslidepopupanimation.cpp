#include "animatedwidgets/popups/fadeslidepopupanimation.h"

#include <QPointer>
#include <QWidget>

namespace animatedwidgets {

FadeSlidePopupAnimation::FadeSlidePopupAnimation(QObject* parent)
    : PopupAnimation(parent)
{
    m_showOpacity = new QPropertyAnimation(&m_showGroup);
    m_showGeometry = new QPropertyAnimation(&m_showGroup);
    m_hideOpacity = new QPropertyAnimation(&m_hideGroup);
    m_hideGeometry = new QPropertyAnimation(&m_hideGroup);
    m_showOpacity->setPropertyName("windowOpacity");
    m_showGeometry->setPropertyName("geometry");
    m_hideOpacity->setPropertyName("windowOpacity");
    m_hideGeometry->setPropertyName("geometry");

    m_showOpacity->setEasingCurve(QEasingCurve::OutCubic);
    m_showGeometry->setEasingCurve(QEasingCurve::OutCubic);
    m_hideOpacity->setEasingCurve(QEasingCurve::InCubic);
    m_hideGeometry->setEasingCurve(QEasingCurve::InCubic);

    m_showGroup.addAnimation(m_showOpacity);
    m_showGroup.addAnimation(m_showGeometry);
    m_hideGroup.addAnimation(m_hideOpacity);
    m_hideGroup.addAnimation(m_hideGeometry);
    connect(&m_showGroup, &QParallelAnimationGroup::finished, this, [this] {
        auto finished = std::move(m_showFinished);
        if (finished)
            finished();
    });
    connect(&m_hideGroup, &QParallelAnimationGroup::finished, this, [this] {
        auto finished = std::move(m_hideFinished);
        if (finished)
            finished();
    });
    setDuration(m_duration);
}

QPoint FadeSlidePopupAnimation::slideOffset(Qt::Edge edge, int distance) {
    switch (edge) {
    case Qt::TopEdge:
        return QPoint(0, distance);
    case Qt::BottomEdge:
        return QPoint(0, -distance);
    case Qt::LeftEdge:
        return QPoint(distance, 0);
    case Qt::RightEdge:
        return QPoint(-distance, 0);
    }
    return QPoint(0, distance);
}

void FadeSlidePopupAnimation::prepareShow(QWidget* popup, Qt::Edge edge, const QRect& target) {
    stop();

    QRect start = target;
    start.translate(slideOffset(edge, m_slideDistance));
    popup->setGeometry(start);
    popup->setWindowOpacity(0.0);
}

void FadeSlidePopupAnimation::show(QWidget* popup, Qt::Edge edge, const QRect& target, std::function<void()> finished) {
    prepareShow(popup, edge, target);

    const QRect start = popup->geometry();

    m_showOpacity->setTargetObject(popup);
    m_showGeometry->setTargetObject(popup);
    m_showOpacity->setStartValue(0.0);
    m_showOpacity->setEndValue(1.0);
    m_showGeometry->setStartValue(start);
    m_showGeometry->setEndValue(target);

    QPointer<QWidget> guard(popup);
    m_showFinished = [guard, finished] {
        if (guard) {
            guard->setWindowOpacity(1.0);
            finished();
        }
    };
    m_showGroup.start();
}

void FadeSlidePopupAnimation::hide(QWidget* popup, Qt::Edge edge, std::function<void()> finished) {
    stop();

    const QRect start = popup->geometry();
    QRect end = start;
    end.translate(slideOffset(edge, m_slideDistance * 3 / 5));

    m_hideOpacity->setTargetObject(popup);
    m_hideGeometry->setTargetObject(popup);
    m_hideOpacity->setStartValue(popup->windowOpacity());
    m_hideOpacity->setEndValue(0.0);
    m_hideGeometry->setStartValue(start);
    m_hideGeometry->setEndValue(end);

    QPointer<QWidget> guard(popup);
    m_hideFinished = [guard, finished] {
        if (guard)
            finished();
    };
    m_hideGroup.start();
}

void FadeSlidePopupAnimation::stop() {
    m_showGroup.stop();
    m_hideGroup.stop();
    m_showFinished = {};
    m_hideFinished = {};
}

int FadeSlidePopupAnimation::duration() const {
    return m_duration;
}

void FadeSlidePopupAnimation::setDuration(int duration) {
    m_duration = qMax(0, duration);
    m_showOpacity->setDuration(m_duration);
    m_showGeometry->setDuration(m_duration);
    m_hideOpacity->setDuration(m_duration * 4 / 5);
    m_hideGeometry->setDuration(m_duration * 4 / 5);
}

int FadeSlidePopupAnimation::slideDistance() const {
    return m_slideDistance;
}

void FadeSlidePopupAnimation::setSlideDistance(int distance) {
    m_slideDistance = qMax(0, distance);
}

} // namespace animatedwidgets
