#include "animatedwidgets/buttons/animatedbuttonbase.h"

#include <QEvent>
#include <QStyleOptionButton>

namespace animatedwidgets {

AnimatedButtonBase::AnimatedButtonBase(QWidget* parent)
    : QPushButton(parent)
    , m_backgroundTransition(this)
    , m_borderTransition(this)
    , m_contentTransition(this)
{
    setAutoDefault(false);
    setAttribute(Qt::WA_Hover, true);
    for (auto* transition : {&m_backgroundTransition, &m_borderTransition, &m_contentTransition}) {
        transition->setDuration(m_animationDuration);
        transition->setEasingCurve(QEasingCurve::OutCubic);
        connect(transition, &ValueTransition::valueChanged, this, [this] { update(); });
    }
    connect(this, &QPushButton::toggled, this, [this] { refreshButtonState(); });
}

int AnimatedButtonBase::animationDuration() const {
    return m_animationDuration;
}

void AnimatedButtonBase::setAnimationDuration(int duration) {
    m_animationDuration = qMax(0, duration);
    for (auto* transition : {&m_backgroundTransition, &m_borderTransition, &m_contentTransition}) {
        transition->setDuration(m_animationDuration);
    }
    refreshButtonState(m_animationDuration == 0);
}

int AnimatedButtonBase::darkerStep() const {
    return m_darkerStep;
}

void AnimatedButtonBase::setDarkerStep(int step) {
    m_darkerStep = qMax(0, step);
    refreshButtonState();
}

bool AnimatedButtonBase::animateContentColor() const {
    return m_animateContentColor;
}

void AnimatedButtonBase::setAnimateContentColor(bool enabled) {
    m_animateContentColor = enabled;
    refreshButtonState();
}

ButtonState AnimatedButtonBase::buttonState() const {
    QStyleOptionButton option;
    initStyleOption(&option);
    if (!(option.state & QStyle::State_Enabled))
        return ButtonState::Disabled;
    if (option.state & QStyle::State_Sunken)
        return ButtonState::Pressed;
    if (option.state & QStyle::State_On)
        return ButtonState::Checked;
    if (option.state & QStyle::State_MouseOver)
        return ButtonState::Hover;
    return ButtonState::Normal;
}

void AnimatedButtonBase::refreshButtonState(bool immediate) {
    updateAnimationTargets(immediate);
}

void AnimatedButtonBase::animateBackgroundColor(const QColor& target, bool immediate) {
    m_backgroundTransition.setTarget(target, immediate);
}

void AnimatedButtonBase::animateBorderColor(const QColor& target, bool immediate) {
    m_borderTransition.setTarget(target, immediate);
}

void AnimatedButtonBase::animateContentColor(const QColor& target, bool immediate) {
    m_contentTransition.setTarget(target, immediate);
}

QColor AnimatedButtonBase::animatedBackgroundColor() const {
    return m_backgroundTransition.current().value<QColor>();
}

QColor AnimatedButtonBase::animatedBorderColor() const {
    return m_borderTransition.current().value<QColor>();
}

QColor AnimatedButtonBase::animatedContentColor() const {
    return m_contentTransition.current().value<QColor>();
}

ButtonContentOptions::Placement AnimatedButtonBase::contentPlacement(IconPlacement placement) {
    switch (placement) {
        case IconPlacement::AfterText:
            return ButtonContentOptions::Placement::AfterText;
        case IconPlacement::AboveText:
            return ButtonContentOptions::Placement::AboveText;
        case IconPlacement::BelowText:
            return ButtonContentOptions::Placement::BelowText;
        case IconPlacement::BeforeText:
            break;
    }
    return ButtonContentOptions::Placement::BeforeText;
}

bool AnimatedButtonBase::event(QEvent* event) {
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
