#pragma once

#include "animatedwidgets/buttons/checkableiconanimator.h"

#include <QCheckBox>
#include <QColor>
#include <QPixmap>

class CheckBox final : public QCheckBox {
    Q_OBJECT
    Q_PROPERTY(QPixmap checkedIcon READ checkedIcon WRITE setCheckedIcon)
    Q_PROPERTY(QPixmap uncheckedIcon READ uncheckedIcon WRITE setUncheckedIcon)
    Q_PROPERTY(QPixmap partiallyCheckedIcon READ partiallyCheckedIcon WRITE setPartiallyCheckedIcon)
    Q_PROPERTY(QColor checkedIconColor READ checkedIconColor WRITE setCheckedIconColor)
    Q_PROPERTY(QColor uncheckedIconColor READ uncheckedIconColor WRITE setUncheckedIconColor)
    Q_PROPERTY(QColor partiallyCheckedIconColor READ partiallyCheckedIconColor WRITE setPartiallyCheckedIconColor)
    Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize)
    Q_PROPERTY(int iconSpacing READ iconSpacing WRITE setIconSpacing)
    Q_PROPERTY(int animationDuration READ animationDuration WRITE setAnimationDuration)

public:
    explicit CheckBox(QWidget* parent = nullptr);

    QPixmap checkedIcon() const;
    void setCheckedIcon(const QPixmap& icon);
    QPixmap uncheckedIcon() const;
    void setUncheckedIcon(const QPixmap& icon);
    QPixmap partiallyCheckedIcon() const;
    void setPartiallyCheckedIcon(const QPixmap& icon);
    QColor checkedIconColor() const;
    void setCheckedIconColor(const QColor& color);
    QColor uncheckedIconColor() const;
    void setUncheckedIconColor(const QColor& color);
    QColor partiallyCheckedIconColor() const;
    void setPartiallyCheckedIconColor(const QColor& color);
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
    animatedwidgets::CheckableIconAnimator m_animator;
    int m_iconSpacing = 8;
};

namespace animatedwidgets {
using CheckBox = ::CheckBox;
} // namespace animatedwidgets
