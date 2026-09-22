#pragma once

#include "animatedwidgets/buttons/animatedbuttonbase.h"
#include "animatedwidgets/buttons/buttonstatestyle.h"

namespace animatedwidgets {

class BorderedButton final : public AnimatedButtonBase {
    Q_OBJECT
    Q_PROPERTY(QColor normalBackgroundColor READ normalBackgroundColor WRITE setNormalBackgroundColor)
    Q_PROPERTY(QColor hoverBackgroundColor READ hoverBackgroundColor WRITE setHoverBackgroundColor)
    Q_PROPERTY(QColor pressedBackgroundColor READ pressedBackgroundColor WRITE setPressedBackgroundColor)
    Q_PROPERTY(QColor disabledBackgroundColor READ disabledBackgroundColor WRITE setDisabledBackgroundColor)
    Q_PROPERTY(QColor checkedBackgroundColor READ checkedBackgroundColor WRITE setCheckedBackgroundColor)
    Q_PROPERTY(QColor normalBorderColor READ normalBorderColor WRITE setNormalBorderColor)
    Q_PROPERTY(QColor hoverBorderColor READ hoverBorderColor WRITE setHoverBorderColor)
    Q_PROPERTY(QColor pressedBorderColor READ pressedBorderColor WRITE setPressedBorderColor)
    Q_PROPERTY(QColor disabledBorderColor READ disabledBorderColor WRITE setDisabledBorderColor)
    Q_PROPERTY(QColor checkedBorderColor READ checkedBorderColor WRITE setCheckedBorderColor)
    Q_PROPERTY(QColor normalContentColor READ normalContentColor WRITE setNormalContentColor)
    Q_PROPERTY(QColor hoverContentColor READ hoverContentColor WRITE setHoverContentColor)
    Q_PROPERTY(QColor pressedContentColor READ pressedContentColor WRITE setPressedContentColor)
    Q_PROPERTY(QColor disabledContentColor READ disabledContentColor WRITE setDisabledContentColor)
    Q_PROPERTY(QColor checkedContentColor READ checkedContentColor WRITE setCheckedContentColor)
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
    QColor normalBackgroundColor() const; void setNormalBackgroundColor(const QColor& color);
    QColor hoverBackgroundColor() const; void setHoverBackgroundColor(const QColor& color);
    QColor pressedBackgroundColor() const; void setPressedBackgroundColor(const QColor& color);
    QColor disabledBackgroundColor() const; void setDisabledBackgroundColor(const QColor& color);
    QColor checkedBackgroundColor() const; void setCheckedBackgroundColor(const QColor& color);
    QColor normalBorderColor() const; void setNormalBorderColor(const QColor& color);
    QColor hoverBorderColor() const; void setHoverBorderColor(const QColor& color);
    QColor pressedBorderColor() const; void setPressedBorderColor(const QColor& color);
    QColor disabledBorderColor() const; void setDisabledBorderColor(const QColor& color);
    QColor checkedBorderColor() const; void setCheckedBorderColor(const QColor& color);
    QColor normalContentColor() const; void setNormalContentColor(const QColor& color);
    QColor hoverContentColor() const; void setHoverContentColor(const QColor& color);
    QColor pressedContentColor() const; void setPressedContentColor(const QColor& color);
    QColor disabledContentColor() const; void setDisabledContentColor(const QColor& color);
    QColor checkedContentColor() const; void setCheckedContentColor(const QColor& color);
    int cornerRadius() const; void setCornerRadius(int radius);
    qreal borderWidth() const; void setBorderWidth(qreal width);
    int iconSpacing() const; void setIconSpacing(int spacing);
    IconPlacement iconPlacement() const; void setIconPlacement(IconPlacement placement);
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

using BorderedButton = animatedwidgets::BorderedButton;
