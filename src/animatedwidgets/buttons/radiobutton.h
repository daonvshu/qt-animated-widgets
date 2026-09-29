#pragma once

#include "animatedwidgets/buttons/checkableiconanimator.h"

#include <QColor>
#include <QPixmap>
#include <QRadioButton>

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
    CheckableIconAnimator m_animator;
    int m_iconSpacing = 8;
};

} // namespace animatedwidgets

using RadioButton = animatedwidgets::RadioButton;
