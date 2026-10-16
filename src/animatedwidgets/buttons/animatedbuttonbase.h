#pragma once

#include "animatedwidgets/buttons/buttonstate.h"
#include "animatedwidgets/core/valuetransition.h"
#include "animatedwidgets/utils/buttonpainter.h"

#include <QPushButton>

namespace animatedwidgets {

class AnimatedButtonBase : public QPushButton {
    Q_OBJECT
    Q_PROPERTY(bool iconHint READ iconHint WRITE setIconHint)

public:
    enum IconPlacement { BeforeText, AfterText, AboveText, BelowText };
    Q_ENUM(IconPlacement)
    enum class Shape { Circle, RoundedRectangle };
    Q_ENUM(Shape)

    int animationDuration() const;
    void setAnimationDuration(int duration);
    int darkerStep() const;
    void setDarkerStep(int step);
    bool animateContentColor() const;
    void setAnimateContentColor(bool enabled);

    bool iconHint() const;
    void setIconHint(bool enabled);

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
    static ButtonContentOptions::Placement contentPlacement(IconPlacement placement);

    virtual void updateAnimationTargets(bool immediate) = 0;
    bool event(QEvent* event) override;

private:
    ValueTransition m_backgroundTransition;
    ValueTransition m_borderTransition;
    ValueTransition m_contentTransition;
    int m_animationDuration = 180;
    int m_darkerStep = 15;
    bool m_animateContentColor = false;
    bool m_iconHint = false;
};

} // namespace animatedwidgets
