#pragma once

#include "animationspec.h"

#include <QByteArray>
#include <QObject>
#include <QVariant>

class QPropertyAnimation;

namespace animatedwidgets {

class AnimationController final : public QObject {
    Q_OBJECT

public:
    explicit AnimationController(QObject* parent = nullptr);

    AnimationSpec spec() const;
    void setSpec(const AnimationSpec& spec);

    void animate(QObject* target, const QByteArray& property, const QVariant& endValue, const QVariant& startValue = QVariant());
    void stop();
    bool isRunning() const;

signals:
    void finished();

private:
    AnimationSpec m_spec;
    QPropertyAnimation* m_animation = nullptr;
};

} // namespace animatedwidgets
