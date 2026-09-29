#pragma once

#include "animatedwidgets/buttons/animatedbuttonbase.h"
#include "animatedwidgets/buttons/buttonstatestyle.h"

class OutlineButton final : public animatedwidgets::AnimatedButtonBase {
    Q_OBJECT
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
    explicit OutlineButton(QWidget* parent = nullptr);
    animatedwidgets::ButtonStateStyle normalStyle() const;
    void setNormalStyle(const animatedwidgets::ButtonStateStyle& style);
    animatedwidgets::ButtonStateStyle hoverStyle() const;
    void setHoverStyle(const animatedwidgets::ButtonStateStyle& style);
    animatedwidgets::ButtonStateStyle pressedStyle() const;
    void setPressedStyle(const animatedwidgets::ButtonStateStyle& style);
    animatedwidgets::ButtonStateStyle disabledStyle() const;
    void setDisabledStyle(const animatedwidgets::ButtonStateStyle& style);
    animatedwidgets::ButtonStateStyle checkedStyle() const;
    void setCheckedStyle(const animatedwidgets::ButtonStateStyle& style);
    QColor normalBorderColor() const;
    void setNormalBorderColor(const QColor& color);
    QColor hoverBorderColor() const;
    void setHoverBorderColor(const QColor& color);
    QColor pressedBorderColor() const;
    void setPressedBorderColor(const QColor& color);
    QColor disabledBorderColor() const;
    void setDisabledBorderColor(const QColor& color);
    QColor checkedBorderColor() const;
    void setCheckedBorderColor(const QColor& color);
    QColor normalContentColor() const;
    void setNormalContentColor(const QColor& color);
    QColor hoverContentColor() const;
    void setHoverContentColor(const QColor& color);
    QColor pressedContentColor() const;
    void setPressedContentColor(const QColor& color);
    QColor disabledContentColor() const;
    void setDisabledContentColor(const QColor& color);
    QColor checkedContentColor() const;
    void setCheckedContentColor(const QColor& color);
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
    QColor borderForState(animatedwidgets::ButtonState state) const;
    QColor contentForState(animatedwidgets::ButtonState state) const;
    QColor m_normalBorder = QColor("#3478f6");
    QColor m_hoverBorder;
    QColor m_pressedBorder;
    QColor m_disabledBorder = QColor("#c8ccd4");
    QColor m_checkedBorder;
    QColor m_normalContent = QColor("#3478f6");
    QColor m_hoverContent;
    QColor m_pressedContent;
    QColor m_disabledContent = QColor("#9298a2");
    QColor m_checkedContent;
    int m_cornerRadius = 6;
    qreal m_borderWidth = 1.0;
    int m_iconSpacing = 6;
    IconPlacement m_iconPlacement = IconPlacement::BeforeText;
};

namespace animatedwidgets {
using OutlineButton = ::OutlineButton;
} // namespace animatedwidgets
