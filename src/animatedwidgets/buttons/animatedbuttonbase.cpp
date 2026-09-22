#include "animatedwidgets/buttons/animatedbuttonbase.h"

#include <QEvent>
#include <QStyleOptionButton>

namespace animatedwidgets {

AnimatedButtonBase::AnimatedButtonBase(QWidget* parent)
    : QPushButton(parent), m_backgroundAnimation(this), m_borderAnimation(this),
      m_contentAnimation(this)
{
    setAutoDefault(false);
    setAttribute(Qt::WA_Hover, true);
    for (auto* animation : {&m_backgroundAnimation, &m_borderAnimation, &m_contentAnimation}) {
        animation->setDuration(m_animationDuration);
        animation->setEasingCurve(QEasingCurve::OutCubic);
        connect(animation, &QVariantAnimation::valueChanged, this, [this] { update(); });
    }
    connect(this, &QPushButton::toggled, this, [this] { refreshButtonState(); });
}

int AnimatedButtonBase::animationDuration() const { return m_animationDuration; }
void AnimatedButtonBase::setAnimationDuration(int duration)
{
    m_animationDuration = qMax(0, duration);
    for (auto* animation : {&m_backgroundAnimation, &m_borderAnimation, &m_contentAnimation}) {
        animation->setDuration(m_animationDuration);
    }
    refreshButtonState(m_animationDuration == 0);
}

int AnimatedButtonBase::darkerStep() const { return m_darkerStep; }
void AnimatedButtonBase::setDarkerStep(int step)
{
    m_darkerStep = qMax(0, step);
    refreshButtonState();
}

bool AnimatedButtonBase::animateContentColor() const { return m_animateContentColor; }
void AnimatedButtonBase::setAnimateContentColor(bool enabled)
{
    m_animateContentColor = enabled;
    refreshButtonState();
}

ButtonState AnimatedButtonBase::buttonState() const
{
    QStyleOptionButton option;
    initStyleOption(&option);
    if (!(option.state & QStyle::State_Enabled)) return ButtonState::Disabled;
    if (option.state & QStyle::State_Sunken) return ButtonState::Pressed;
    if (option.state & QStyle::State_On) return ButtonState::Checked;
    if (option.state & QStyle::State_MouseOver) return ButtonState::Hover;
    return ButtonState::Normal;
}

void AnimatedButtonBase::refreshButtonState(bool immediate)
{
    updateAnimationTargets(immediate);
}

void AnimatedButtonBase::animateColor(QVariantAnimation& animation, const QColor& target, bool immediate)
{
    if (!immediate && animation.endValue().value<QColor>() == target) return;
    const QColor current = animation.currentValue().value<QColor>();
    animation.stop();
    animation.setStartValue(immediate || m_animationDuration == 0 || !current.isValid() ? target : current);
    animation.setEndValue(target);
    if (!immediate && m_animationDuration > 0 && current != target) animation.start();
}

void AnimatedButtonBase::animateBackgroundColor(const QColor& target, bool immediate)
{
    animateColor(m_backgroundAnimation, target, immediate);
}

void AnimatedButtonBase::animateBorderColor(const QColor& target, bool immediate)
{
    animateColor(m_borderAnimation, target, immediate);
}

void AnimatedButtonBase::animateContentColor(const QColor& target, bool immediate)
{
    animateColor(m_contentAnimation, target, immediate);
}

QColor AnimatedButtonBase::animatedBackgroundColor() const
{
    return m_backgroundAnimation.currentValue().value<QColor>();
}

QColor AnimatedButtonBase::animatedBorderColor() const
{
    return m_borderAnimation.currentValue().value<QColor>();
}

QColor AnimatedButtonBase::animatedContentColor() const
{
    return m_contentAnimation.currentValue().value<QColor>();
}

QColor AnimatedButtonBase::darkerStateColor(const QColor& normal, int multiplier) const
{
    return normal.darker(100 + qMax(0, multiplier) * m_darkerStep);
}

bool AnimatedButtonBase::event(QEvent* event)
{
    const bool handled = QPushButton::event(event);
    switch (event->type()) {
    case QEvent::Enter:
    case QEvent::Leave:
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonDblClick:
    case QEvent::MouseButtonRelease:
    case QEvent::KeyPress:
    case QEvent::KeyRelease:
    case QEvent::FocusIn:
    case QEvent::FocusOut:
    case QEvent::EnabledChange:
    case QEvent::StyleChange:
    case QEvent::PaletteChange:
        refreshButtonState();
        break;
    default:
        break;
    }
    return handled;
}

} // namespace animatedwidgets
