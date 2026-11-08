#pragma once

#include "animatedwidgets/namespacealias.h"

#include "animatedwidgets/buttons/checkableiconanimator.h"

#include <QCheckBox>
#include <QColor>
#include <QIcon>
#include <QPixmap>

namespace animatedwidgets {

class CheckBox : public QCheckBox {
    Q_OBJECT
    Q_PROPERTY(QPixmap checkedIcon READ checkedIcon WRITE setCheckedIcon)
    Q_PROPERTY(QPixmap uncheckedIcon READ uncheckedIcon WRITE setUncheckedIcon)
    Q_PROPERTY(QPixmap partiallyCheckedIcon READ partiallyCheckedIcon WRITE setPartiallyCheckedIcon)
    Q_PROPERTY(bool iconHint READ iconHint WRITE setIconHint)
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
    void setCheckedIcon(const QIcon& icon);
    QPixmap uncheckedIcon() const;
    void setUncheckedIcon(const QPixmap& icon);
    void setUncheckedIcon(const QIcon& icon);
    QPixmap partiallyCheckedIcon() const;
    void setPartiallyCheckedIcon(const QPixmap& icon);
    void setPartiallyCheckedIcon(const QIcon& icon);
    bool iconHint() const;
    void setIconHint(bool enabled);
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
    void checkStateSet() override;

private:
    CheckableIconAnimator m_animator;
    int m_iconSpacing = 8;
};

} // namespace animatedwidgets

#ifndef ANIMATEDWIDGETS_DESIGNER_PLUGIN
using CheckBox = animatedwidgets::CheckBox;
#endif
