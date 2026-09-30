#pragma once

#include "animatedwidgets/core/valuetransition.h"

#include <QSlider>

namespace animatedwidgets {

class Slider : public QSlider {
    Q_OBJECT
    Q_PROPERTY(bool animationEnabled READ animationEnabled WRITE setAnimationEnabled)
    Q_PROPERTY(int animationDuration READ animationDuration WRITE setAnimationDuration)
    Q_PROPERTY(QEasingCurve::Type animationEasingCurve READ animationEasingCurve WRITE setAnimationEasingCurve)

public:
    explicit Slider(QWidget* parent = nullptr);
    explicit Slider(Qt::Orientation orientation, QWidget* parent = nullptr);

    bool animationEnabled() const;
    void setAnimationEnabled(bool enabled);
    int animationDuration() const;
    void setAnimationDuration(int duration);
    QEasingCurve::Type animationEasingCurve() const;
    void setAnimationEasingCurve(QEasingCurve::Type type);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    void initializeAnimation();

    ValueTransition m_position;
    bool m_animationEnabled = true;
    bool m_pointerInput = false;
    int m_lastMinimum = 0;
    int m_lastMaximum = 99;
};

} // namespace animatedwidgets

#ifndef ANIMATEDWIDGETS_DESIGNER_PLUGIN
using Slider = animatedwidgets::Slider;
#endif
