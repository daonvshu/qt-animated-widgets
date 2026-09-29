#pragma once

#include <QEasingCurve>
#include <QObject>
#include <QVariant>
#include <QVariantAnimation>

namespace animatedwidgets {

class ValueTransition final : public QObject {
    Q_OBJECT

public:
    explicit ValueTransition(QObject* parent = nullptr);

    int duration() const;
    void setDuration(int duration);
    QEasingCurve easingCurve() const;
    void setEasingCurve(const QEasingCurve& easingCurve);

    QVariant current() const;
    QVariant target() const;
    bool isRunning() const;

    void setTarget(const QVariant& target, bool immediate = false);

signals:
    void valueChanged();

private:
    QVariantAnimation m_animation;
    QVariant m_target;
};

} // namespace animatedwidgets
