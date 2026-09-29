#include "animatedwidgets/sliders/slider.h"

#include <QMouseEvent>
#include <QPainter>
#include <QStyle>
#include <QStyleOptionSlider>

Slider::Slider(QWidget* parent)
    : QSlider(parent)
    , m_position(this)
{
    initializeAnimation();
}

Slider::Slider(Qt::Orientation orientation, QWidget* parent)
    : QSlider(orientation, parent)
    , m_position(this)
{
    initializeAnimation();
}

void Slider::initializeAnimation() {
    m_lastMinimum = minimum();
    m_lastMaximum = maximum();
    m_position.setDuration(220);
    m_position.setEasingCurve(QEasingCurve::OutCubic);
    m_position.setTarget(double(value()), true);
    connect(&m_position, &animatedwidgets::ValueTransition::valueChanged, this, [this] { update(); });
    connect(this, &QSlider::valueChanged, this, [this](int currentValue) {
        const bool rangeChanged = m_lastMinimum != minimum() || m_lastMaximum != maximum();
        m_lastMinimum = minimum();
        m_lastMaximum = maximum();
        m_position.setTarget(double(currentValue), rangeChanged || !m_animationEnabled || m_pointerInput || isSliderDown());
    });
    connect(this, &QSlider::sliderMoved, this, [this](int position) {
        if (isSliderDown())
            m_position.setTarget(double(position), true);
    });
}

bool Slider::animationEnabled() const {
    return m_animationEnabled;
}

void Slider::setAnimationEnabled(bool enabled) {
    if (m_animationEnabled == enabled)
        return;
    m_animationEnabled = enabled;
    if (!enabled)
        m_position.setTarget(double(sliderPosition()), true);
}

int Slider::animationDuration() const {
    return m_position.duration();
}

void Slider::setAnimationDuration(int duration) {
    m_position.setDuration(duration);
    if (duration <= 0)
        m_position.setTarget(double(sliderPosition()), true);
}

QEasingCurve::Type Slider::animationEasingCurve() const {
    return m_position.easingCurve().type();
}

void Slider::setAnimationEasingCurve(QEasingCurve::Type type) {
    m_position.setEasingCurve(QEasingCurve(type));
}

void Slider::paintEvent(QPaintEvent* event) {
    if (m_lastMinimum != minimum() || m_lastMaximum != maximum()) {
        m_lastMinimum = minimum();
        m_lastMaximum = maximum();
        m_position.setTarget(double(sliderPosition()), true);
    }
    if (!m_animationEnabled || m_pointerInput || isSliderDown() || !m_position.isRunning() || maximum() == minimum()) {
        QSlider::paintEvent(event);
        return;
    }

    QStyleOptionSlider option;
    initStyleOption(&option);
    const QStyleOptionSlider original = option;
    const double ratio = (m_position.current().toDouble() - minimum()) / (double(maximum()) - minimum());
    option.minimum = 0;
    option.maximum = 10000;
    option.sliderPosition = qRound(qBound(0.0, ratio, 1.0) * option.maximum);
    option.sliderValue = option.sliderPosition;
    option.singleStep = qRound(qBound(1.0, double(singleStep()) * option.maximum / (double(maximum()) - minimum()), double(option.maximum)));
    option.pageStep = qRound(qBound(1.0, double(pageStep()) * option.maximum / (double(maximum()) - minimum()), double(option.maximum)));
    option.tickPosition = QSlider::NoTicks;
    option.subControls = QStyle::SC_SliderGroove | QStyle::SC_SliderHandle;

    QPainter painter(this);
    style()->drawComplexControl(QStyle::CC_Slider, &option, &painter, this);
    if (original.tickPosition != QSlider::NoTicks) {
        QStyleOptionSlider ticks = original;
        ticks.subControls = QStyle::SC_SliderTickmarks;
        style()->drawComplexControl(QStyle::CC_Slider, &ticks, &painter, this);
    }
}

void Slider::mousePressEvent(QMouseEvent* event) {
    m_pointerInput = true;
    m_position.setTarget(double(sliderPosition()), true);
    QSlider::mousePressEvent(event);
}

void Slider::mouseReleaseEvent(QMouseEvent* event) {
    QSlider::mouseReleaseEvent(event);
    m_pointerInput = false;
    m_position.setTarget(double(sliderPosition()), true);
}
