#pragma once

#include <QColor>
#include <QPixmap>
#include <QRadioButton>
#include <QVariantAnimation>

namespace animatedwidgets {

class RadioButton final : public QRadioButton {
    Q_OBJECT
    Q_PROPERTY(QPixmap checkedIcon READ checkedIcon WRITE setCheckedIcon)
    Q_PROPERTY(QPixmap uncheckedIcon READ uncheckedIcon WRITE setUncheckedIcon)
    Q_PROPERTY(QColor checkedIconColor READ checkedIconColor WRITE setCheckedIconColor)
    Q_PROPERTY(QColor uncheckedIconColor READ uncheckedIconColor WRITE setUncheckedIconColor)
    Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize)
    Q_PROPERTY(int iconSpacing READ iconSpacing WRITE setIconSpacing)
    Q_PROPERTY(int animationDuration READ animationDuration WRITE setAnimationDuration)

public:
    explicit RadioButton(QWidget* parent = nullptr);

    QPixmap checkedIcon() const;
    void setCheckedIcon(const QPixmap& icon);
    QPixmap uncheckedIcon() const;
    void setUncheckedIcon(const QPixmap& icon);
    QColor checkedIconColor() const;
    void setCheckedIconColor(const QColor& color);
    QColor uncheckedIconColor() const;
    void setUncheckedIconColor(const QColor& color);
    QSize iconSize() const;
    void setIconSize(const QSize& size);
    int iconSpacing() const;
    void setIconSpacing(int spacing);
    int animationDuration() const;
    void setAnimationDuration(int duration);

    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent* event) override;
    void changeEvent(QEvent* event) override;

private:
    enum class IconState { Unchecked, Checked };

    IconState stateFromChecked(bool checked) const;
    QPixmap iconForState(IconState state) const;
    QColor colorForState(IconState state) const;
    QPixmap currentComposite() const;
    void startAnimation(IconState state);
    void resetAnimation();
    void drawText(QPainter& painter, const QRect& textRect) const;

    QPixmap m_checkedIcon;
    QPixmap m_uncheckedIcon;
    QColor m_checkedIconColor;
    QColor m_uncheckedIconColor;
    QSize m_iconSize = QSize(18, 18);
    int m_iconSpacing = 8;
    QVariantAnimation m_animation;
    IconState m_fromState = IconState::Unchecked;
    IconState m_toState = IconState::Unchecked;
    QPixmap m_transitionPixmap;
    qreal m_progress = 1.0;
    bool m_transitionUsesPixmap = false;
};

} // namespace animatedwidgets

using RadioButton = animatedwidgets::RadioButton;
