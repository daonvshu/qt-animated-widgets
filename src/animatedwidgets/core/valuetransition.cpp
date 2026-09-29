#include "animatedwidgets/core/valuetransition.h"

namespace animatedwidgets {

ValueTransition::ValueTransition(QObject* parent)
    : QObject(parent)
{
    m_animation.setEasingCurve(QEasingCurve::OutCubic);
    connect(&m_animation, &QVariantAnimation::valueChanged, this, &ValueTransition::valueChanged);
}

int ValueTransition::duration() const {
    return m_animation.duration();
}

void ValueTransition::setDuration(int duration) {
    m_animation.setDuration(qMax(0, duration));
}

QEasingCurve ValueTransition::easingCurve() const {
    return m_animation.easingCurve();
}

void ValueTransition::setEasingCurve(const QEasingCurve& easingCurve) {
    m_animation.setEasingCurve(easingCurve);
}

QVariant ValueTransition::current() const {
    return m_animation.state() == QAbstractAnimation::Running ? m_animation.currentValue() : m_target;
}

QVariant ValueTransition::target() const {
    return m_target;
}

bool ValueTransition::isRunning() const {
    return m_animation.state() == QAbstractAnimation::Running;
}

void ValueTransition::setTarget(const QVariant& target, bool immediate) {
    if (!immediate && m_target == target) {
        return;
    }

    const QVariant previous = current();
    m_animation.stop();
    m_target = target;

    const bool animate = !immediate && m_animation.duration() > 0 && previous.isValid() && previous != target;
    m_animation.setStartValue(animate ? previous : target);
    m_animation.setEndValue(target);
    if (animate) {
        m_animation.start();
    } else {
        emit valueChanged();
    }
}

} // namespace animatedwidgets
