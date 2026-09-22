#pragma once

#include "animatedwidgets/buttons/buttonstate.h"

#include <QPushButton>
#include <QVariantAnimation>

namespace animatedwidgets {

class AnimatedButtonBase : public QPushButton {
    Q_OBJECT

public:
    enum class IconPlacement { BeforeText, AfterText, AboveText, BelowText };
    Q_ENUM(IconPlacement)
    enum class Shape { Circle, RoundedRectangle };
    Q_ENUM(Shape)

    int animationDuration() const;
    void setAnimationDuration(int duration);
    int darkerStep() const;
    void setDarkerStep(int step);
    bool animateContentColor() const;
    void setAnimateContentColor(bool enabled);

protected:
    explicit AnimatedButtonBase(QWidget* parent = nullptr);
    ButtonState buttonState() const;
    void refreshButtonState(bool immediate = false);
    void animateBackgroundColor(const QColor& target, bool immediate = false);
    void animateBorderColor(const QColor& target, bool immediate = false);
    void animateContentColor(const QColor& target, bool immediate = false);
    QColor animatedBackgroundColor() const;
    QColor animatedBorderColor() const;
    QColor animatedContentColor() const;
    QColor darkerStateColor(const QColor& normal, int multiplier = 1) const;

    virtual void updateAnimationTargets(bool immediate) = 0;
    bool event(QEvent* event) override;

private:
    void animateColor(QVariantAnimation& animation, const QColor& target, bool immediate);

    QVariantAnimation m_backgroundAnimation;
    QVariantAnimation m_borderAnimation;
    QVariantAnimation m_contentAnimation;
    int m_animationDuration = 180;
    int m_darkerStep = 15;
    bool m_animateContentColor = false;
};

} // namespace animatedwidgets
