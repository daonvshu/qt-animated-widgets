#pragma once

#include "animatedwidgets/buttons/animatedbuttonbase.h"
#include "animatedwidgets/buttons/buttonstatestyle.h"

class HoverButton final : public animatedwidgets::AnimatedButtonBase {
    Q_OBJECT
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
    Q_PROPERTY(int cornerRadius READ cornerRadius WRITE setCornerRadius)
    Q_PROPERTY(int iconSpacing READ iconSpacing WRITE setIconSpacing)
    Q_PROPERTY(IconPlacement iconPlacement READ iconPlacement WRITE setIconPlacement)
public:
    explicit HoverButton(QWidget* parent = nullptr);
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
    QColor hoverBackgroundColor() const;
    void setHoverBackgroundColor(const QColor& color);
    QColor pressedBackgroundColor() const;
    void setPressedBackgroundColor(const QColor& color);
    QColor disabledBackgroundColor() const;
    void setDisabledBackgroundColor(const QColor& color);
    QColor checkedBackgroundColor() const;
    void setCheckedBackgroundColor(const QColor& color);
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
    int iconSpacing() const;
    void setIconSpacing(int spacing);
    IconPlacement iconPlacement() const;
    void setIconPlacement(IconPlacement placement);
    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent* event) override;
    void updateAnimationTargets(bool immediate) override;

private:
    QColor backgroundForState(animatedwidgets::ButtonState state) const;
    QColor contentForState(animatedwidgets::ButtonState state) const;
    QColor m_hoverBackground;
    QColor m_pressedBackground;
    QColor m_disabledBackground = Qt::transparent;
    QColor m_checkedBackground;
    QColor m_normalContent = QColor("#202124");
    QColor m_hoverContent;
    QColor m_pressedContent;
    QColor m_disabledContent = QColor("#9298a2");
    QColor m_checkedContent;
    int m_cornerRadius = 6;
    int m_iconSpacing = 6;
    IconPlacement m_iconPlacement = IconPlacement::BeforeText;
};

namespace animatedwidgets {
using HoverButton = ::HoverButton;
} // namespace animatedwidgets
