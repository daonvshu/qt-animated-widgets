#include "animatedwidgets/core/animationcontroller.h"

#include <QPropertyAnimation>

namespace animatedwidgets {

AnimationController::AnimationController(QObject* parent)
    : QObject(parent)
    , m_animation(new QPropertyAnimation(this))
{
    connect(m_animation, &QPropertyAnimation::finished, this, &AnimationController::finished);
}

AnimationSpec AnimationController::spec() const {
    return m_spec;
}

void AnimationController::setSpec(const AnimationSpec& spec) {
    m_spec = spec;
}

void AnimationController::animate(QObject* target, const QByteArray& property, const QVariant& endValue, const QVariant& startValue) {
    if (!target || property.isEmpty()) {
        return;
    }

    const QVariant currentValue =
            m_animation->state() == QAbstractAnimation::Running && m_animation->targetObject() == target && m_animation->propertyName() == property
            ? m_animation->currentValue()
            : QVariant();
    m_animation->stop();
    if (m_spec.duration <= 0) {
        target->setProperty(property.constData(), endValue);
        emit finished();
        return;
    }
    m_animation->setTargetObject(target);
    m_animation->setPropertyName(property);
    m_animation->setDuration(qMax(0, m_spec.duration));
    m_animation->setEasingCurve(m_spec.easing);

    const QVariant resolvedStart = currentValue.isValid() ? currentValue : startValue.isValid() ? startValue : target->property(property.constData());
    if (resolvedStart.isValid()) {
        m_animation->setStartValue(resolvedStart);
    }
    m_animation->setEndValue(endValue);
    m_animation->start();
}

void AnimationController::stop() {
    m_animation->stop();
}

bool AnimationController::isRunning() const {
    return m_animation->state() == QAbstractAnimation::Running;
}

} // namespace animatedwidgets
