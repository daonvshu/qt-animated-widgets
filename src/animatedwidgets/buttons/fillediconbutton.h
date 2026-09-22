#pragma once

#include "animatedwidgets/buttons/animatedbuttonbase.h"
#include "animatedwidgets/buttons/buttonstatestyle.h"

namespace animatedwidgets {

class FilledIconButton final : public AnimatedButtonBase {
    Q_OBJECT
    Q_PROPERTY(QColor normalBackgroundColor READ normalBackgroundColor WRITE setNormalBackgroundColor)
    Q_PROPERTY(QColor hoverBackgroundColor READ hoverBackgroundColor WRITE setHoverBackgroundColor)
    Q_PROPERTY(QColor pressedBackgroundColor READ pressedBackgroundColor WRITE setPressedBackgroundColor)
    Q_PROPERTY(QColor disabledBackgroundColor READ disabledBackgroundColor WRITE setDisabledBackgroundColor)
    Q_PROPERTY(QColor checkedBackgroundColor READ checkedBackgroundColor WRITE setCheckedBackgroundColor)
    Q_PROPERTY(QColor normalContentColor READ normalContentColor WRITE setNormalContentColor)
    Q_PROPERTY(QColor hoverContentColor READ hoverContentColor WRITE setHoverContentColor)
    Q_PROPERTY(QColor pressedContentColor READ pressedContentColor WRITE setPressedContentColor)
    Q_PROPERTY(QColor disabledContentColor READ disabledContentColor WRITE setDisabledContentColor)
    Q_PROPERTY(QColor checkedContentColor READ checkedContentColor WRITE setCheckedContentColor)
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
    QColor normalBackgroundColor() const; void setNormalBackgroundColor(const QColor& color);
    QColor hoverBackgroundColor() const; void setHoverBackgroundColor(const QColor& color);
    QColor pressedBackgroundColor() const; void setPressedBackgroundColor(const QColor& color);
    QColor disabledBackgroundColor() const; void setDisabledBackgroundColor(const QColor& color);
    QColor checkedBackgroundColor() const; void setCheckedBackgroundColor(const QColor& color);
    QColor normalContentColor() const; void setNormalContentColor(const QColor& color);
    QColor hoverContentColor() const; void setHoverContentColor(const QColor& color);
    QColor pressedContentColor() const; void setPressedContentColor(const QColor& color);
    QColor disabledContentColor() const; void setDisabledContentColor(const QColor& color);
    QColor checkedContentColor() const; void setCheckedContentColor(const QColor& color);
    Shape shape() const; void setShape(Shape shape);
    int cornerRadius() const; void setCornerRadius(int radius);
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

using FilledIconButton = animatedwidgets::FilledIconButton;
