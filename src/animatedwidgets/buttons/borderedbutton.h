#pragma once

#include "animatedwidgets/buttons/animatedbuttonbase.h"
#include "animatedwidgets/buttons/buttonstatestyle.h"

namespace animatedwidgets {

class BorderedButton : public AnimatedButtonBase {
    Q_OBJECT
    Q_PROPERTY(QColor backgroundNormal READ backgroundNormal WRITE setBackgroundNormal)
    Q_PROPERTY(QColor backgroundHover READ backgroundHover WRITE setBackgroundHover)
    Q_PROPERTY(QColor backgroundPressed READ backgroundPressed WRITE setBackgroundPressed)
    Q_PROPERTY(QColor backgroundDisabled READ backgroundDisabled WRITE setBackgroundDisabled)
    Q_PROPERTY(QColor backgroundChecked READ backgroundChecked WRITE setBackgroundChecked)
    Q_PROPERTY(QColor borderNormal READ borderNormal WRITE setBorderNormal)
    Q_PROPERTY(QColor borderHover READ borderHover WRITE setBorderHover)
    Q_PROPERTY(QColor borderPressed READ borderPressed WRITE setBorderPressed)
    Q_PROPERTY(QColor borderDisabled READ borderDisabled WRITE setBorderDisabled)
    Q_PROPERTY(QColor borderChecked READ borderChecked WRITE setBorderChecked)
    Q_PROPERTY(QColor contentNormal READ contentNormal WRITE setContentNormal)
    Q_PROPERTY(QColor contentHover READ contentHover WRITE setContentHover)
    Q_PROPERTY(QColor contentPressed READ contentPressed WRITE setContentPressed)
    Q_PROPERTY(QColor contentDisabled READ contentDisabled WRITE setContentDisabled)
    Q_PROPERTY(QColor contentChecked READ contentChecked WRITE setContentChecked)
    Q_PROPERTY(bool animateContentColor READ animateContentColor WRITE setAnimateContentColor)
    Q_PROPERTY(int darkerStep READ darkerStep WRITE setDarkerStep)
    Q_PROPERTY(int animationDuration READ animationDuration WRITE setAnimationDuration)
    Q_PROPERTY(int cornerRadius READ cornerRadius WRITE setCornerRadius)
    Q_PROPERTY(qreal borderWidth READ borderWidth WRITE setBorderWidth)
    Q_PROPERTY(int iconSpacing READ iconSpacing WRITE setIconSpacing)
    Q_PROPERTY(IconPlacement iconPlacement READ iconPlacement WRITE setIconPlacement)
public:
    explicit BorderedButton(QWidget* parent = nullptr);
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
    QColor borderNormal() const;
    void setBorderNormal(const QColor& color);
    QColor borderHover() const;
    void setBorderHover(const QColor& color);
    QColor borderPressed() const;
    void setBorderPressed(const QColor& color);
    QColor borderDisabled() const;
    void setBorderDisabled(const QColor& color);
    QColor borderChecked() const;
    void setBorderChecked(const QColor& color);
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
    int cornerRadius() const;
    void setCornerRadius(int radius);
    qreal borderWidth() const;
    void setBorderWidth(qreal width);
    int iconSpacing() const;
    void setIconSpacing(int spacing);
    IconPlacement iconPlacement() const;
    void setIconPlacement(IconPlacement placement);
    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent* event) override;
    void updateAnimationTargets(bool immediate) override;

private:
    QColor backgroundForState(ButtonState state) const;
    QColor borderForState(ButtonState state) const;
    QColor contentForState(ButtonState state) const;
    QColor m_normalBackground = QColor("#ffffff");
    QColor m_hoverBackground;
    QColor m_pressedBackground;
    QColor m_disabledBackground = QColor("#f2f3f5");
    QColor m_checkedBackground;
    QColor m_normalBorder = QColor("#c8ccd4");
    QColor m_hoverBorder;
    QColor m_pressedBorder;
    QColor m_disabledBorder = QColor("#e0e2e6");
    QColor m_checkedBorder;
    QColor m_normalContent = QColor("#202124");
    QColor m_hoverContent;
    QColor m_pressedContent;
    QColor m_disabledContent = QColor("#9298a2");
    QColor m_checkedContent;
    int m_cornerRadius = 6;
    qreal m_borderWidth = 1.0;
    int m_iconSpacing = 6;
    IconPlacement m_iconPlacement = IconPlacement::BeforeText;
};

} // namespace animatedwidgets

#ifndef ANIMATEDWIDGETS_DESIGNER_PLUGIN
using BorderedButton = animatedwidgets::BorderedButton;
#endif
