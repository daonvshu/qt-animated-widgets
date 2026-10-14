#pragma once

#include "animatedwidgets/namespacealias.h"

#include "animatedwidgets/buttons/animatedbuttonbase.h"
#include "animatedwidgets/buttons/buttonstatestyle.h"

namespace animatedwidgets {

class FilledButton : public AnimatedButtonBase {
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
    Q_PROPERTY(int cornerRadius READ cornerRadius WRITE setCornerRadius)
    Q_PROPERTY(int iconSpacing READ iconSpacing WRITE setIconSpacing)
    Q_PROPERTY(IconPlacement iconPlacement READ iconPlacement WRITE setIconPlacement)

    Q_PROPERTY(Qt::Alignment contentAlignment READ contentAlignment WRITE setContentAlignment)
    Q_PROPERTY(int contentHorizontalPadding READ contentHorizontalPadding WRITE setContentHorizontalPadding)
    Q_PROPERTY(int contentVerticalPadding READ contentVerticalPadding WRITE setContentVerticalPadding)

public:
    explicit FilledButton(QWidget* parent = nullptr);

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
    int cornerRadius() const;
    void setCornerRadius(int radius);
    int iconSpacing() const;
    void setIconSpacing(int spacing);
    IconPlacement iconPlacement() const;
    void setIconPlacement(IconPlacement placement);

    Qt::Alignment contentAlignment() const;
    void setContentAlignment(Qt::Alignment alignment);
    int contentHorizontalPadding() const;
    void setContentHorizontalPadding(int padding);
    int contentVerticalPadding() const;
    void setContentVerticalPadding(int padding);

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
    int m_cornerRadius = 6;
    int m_iconSpacing = 6;
    IconPlacement m_iconPlacement = IconPlacement::BeforeText;

    Qt::Alignment m_contentAlignment = Qt::AlignCenter;
    int m_contentHorizontalPadding = 0;
    int m_contentVerticalPadding = 0;
};

} // namespace animatedwidgets

#ifndef ANIMATEDWIDGETS_DESIGNER_PLUGIN
using FilledButton = animatedwidgets::FilledButton;
#endif
