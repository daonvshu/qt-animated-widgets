#pragma once

#include "animatedwidgets/namespacealias.h"

#include "animatedwidgets/buttons/animatedbuttonbase.h"
#include "animatedwidgets/buttons/buttonstatestyle.h"

namespace animatedwidgets {

class OutlineButton : public AnimatedButtonBase {
    Q_OBJECT
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

    Q_PROPERTY(Qt::Alignment contentAlignment READ contentAlignment WRITE setContentAlignment)
    Q_PROPERTY(int contentHorizontalPadding READ contentHorizontalPadding WRITE setContentHorizontalPadding)
    Q_PROPERTY(int contentVerticalPadding READ contentVerticalPadding WRITE setContentVerticalPadding)

public:
    explicit OutlineButton(QWidget* parent = nullptr);
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
    QColor borderForState(ButtonState state) const;
    QColor contentForState(ButtonState state) const;
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

    Qt::Alignment m_contentAlignment = Qt::AlignCenter;
    int m_contentHorizontalPadding = 0;
    int m_contentVerticalPadding = 0;
};

} // namespace animatedwidgets

#ifndef ANIMATEDWIDGETS_DESIGNER_PLUGIN
using OutlineButton = animatedwidgets::OutlineButton;
#endif
