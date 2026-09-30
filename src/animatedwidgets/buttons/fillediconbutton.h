#pragma once

#include "animatedwidgets/buttons/animatedbuttonbase.h"
#include "animatedwidgets/buttons/buttonstatestyle.h"

namespace animatedwidgets {

class FilledIconButton : public AnimatedButtonBase {
    Q_OBJECT
    Q_PROPERTY(QColor backgroundNormal READ backgroundNormal WRITE setBackgroundNormal)
    Q_PROPERTY(QColor backgroundHover READ backgroundHover WRITE setBackgroundHover)
    Q_PROPERTY(QColor backgroundPressed READ backgroundPressed WRITE setBackgroundPressed)
    Q_PROPERTY(QColor backgroundDisabled READ backgroundDisabled WRITE setBackgroundDisabled)
    Q_PROPERTY(QColor backgroundChecked READ backgroundChecked WRITE setBackgroundChecked)
    Q_PROPERTY(QColor contentNormal READ contentNormal WRITE setContentNormal)
    Q_PROPERTY(QColor contentHover READ contentHover WRITE setContentHover)
    Q_PROPERTY(QColor contentPressed READ contentPressed WRITE setContentPressed)
    Q_PROPERTY(QColor contentDisabled READ contentDisabled WRITE setContentDisabled)
    Q_PROPERTY(QColor contentChecked READ contentChecked WRITE setContentChecked)
    Q_PROPERTY(bool animateContentColor READ animateContentColor WRITE setAnimateContentColor)
    Q_PROPERTY(int darkerStep READ darkerStep WRITE setDarkerStep)
    Q_PROPERTY(int animationDuration READ animationDuration WRITE setAnimationDuration)
    Q_PROPERTY(Shape shape READ shape WRITE setShape)
    Q_PROPERTY(int cornerRadius READ cornerRadius WRITE setCornerRadius)
public:
    explicit FilledIconButton(QWidget* parent = nullptr);
    ButtonStateStyle normalStyle() const;
    void setNormalStyle(const ButtonStateStyle& style);
    ButtonStateStyle hoverStyle() const;
    void setHoverStyle(const ButtonStateStyle& style);
    ButtonStateStyle pressedStyle() const;
    void setPressedStyle(const ButtonStateStyle& style);
    ButtonStateStyle disabledStyle() const;
    void setDisabledStyle(const ButtonStateStyle& style);
    ButtonStateStyle checkedStyle() const;
    void setCheckedStyle(const ButtonStateStyle& style);
    QColor backgroundNormal() const;
    void setBackgroundNormal(const QColor& color);
    QColor backgroundHover() const;
    void setBackgroundHover(const QColor& color);
    QColor backgroundPressed() const;
    void setBackgroundPressed(const QColor& color);
    QColor backgroundDisabled() const;
    void setBackgroundDisabled(const QColor& color);
    QColor backgroundChecked() const;
    void setBackgroundChecked(const QColor& color);
    QColor contentNormal() const;
    void setContentNormal(const QColor& color);
    QColor contentHover() const;
    void setContentHover(const QColor& color);
    QColor contentPressed() const;
    void setContentPressed(const QColor& color);
    QColor contentDisabled() const;
    void setContentDisabled(const QColor& color);
    QColor contentChecked() const;
    void setContentChecked(const QColor& color);
    Shape shape() const;
    void setShape(Shape shape);
    int cornerRadius() const;
    void setCornerRadius(int radius);
    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent* event) override;
    void updateAnimationTargets(bool immediate) override;

private:
    QColor backgroundForState(ButtonState state) const;
    QColor contentForState(ButtonState state) const;
    QColor m_normalBackground = QColor("#3478f6");
    QColor m_hoverBackground;
    QColor m_pressedBackground;
    QColor m_disabledBackground = QColor("#e4e7ed");
    QColor m_checkedBackground;
    QColor m_normalContent = QColor("#ffffff");
    QColor m_hoverContent;
    QColor m_pressedContent;
    QColor m_disabledContent = QColor("#9298a2");
    QColor m_checkedContent;
    Shape m_shape = Shape::Circle;
    int m_cornerRadius = 6;
};

} // namespace animatedwidgets

#ifndef ANIMATEDWIDGETS_DESIGNER_PLUGIN
using FilledIconButton = animatedwidgets::FilledIconButton;
#endif
