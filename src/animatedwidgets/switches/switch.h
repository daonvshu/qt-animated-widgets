#pragma once

#include "animatedwidgets/namespacealias.h"

#include "animatedwidgets/core/valuetransition.h"

#include <QCheckBox>
#include <QColor>
#include <QIcon>

namespace animatedwidgets {

class Switch : public QCheckBox {
    Q_OBJECT
    Q_PROPERTY(QColor uncheckedTrackColor READ uncheckedTrackColor WRITE setUncheckedTrackColor)
    Q_PROPERTY(QColor checkedTrackColor READ checkedTrackColor WRITE setCheckedTrackColor)
    Q_PROPERTY(QColor disabledTrackColor READ disabledTrackColor WRITE setDisabledTrackColor)
    Q_PROPERTY(QColor uncheckedThumbColor READ uncheckedThumbColor WRITE setUncheckedThumbColor)
    Q_PROPERTY(QColor checkedThumbColor READ checkedThumbColor WRITE setCheckedThumbColor)
    Q_PROPERTY(QColor disabledThumbColor READ disabledThumbColor WRITE setDisabledThumbColor)
    Q_PROPERTY(QSize trackSize READ trackSize WRITE setTrackSize)
    Q_PROPERTY(int thumbMargin READ thumbMargin WRITE setThumbMargin)
    Q_PROPERTY(int handleRadius READ handleRadius WRITE setHandleRadius)
    Q_PROPERTY(QIcon handleIcon READ handleIcon WRITE setHandleIcon)
    Q_PROPERTY(bool iconHint READ iconHint WRITE setIconHint)
    Q_PROPERTY(int textSpacing READ textSpacing WRITE setTextSpacing)
    Q_PROPERTY(int animationDuration READ animationDuration WRITE setAnimationDuration)

public:
    explicit Switch(QWidget* parent = nullptr);

    QColor uncheckedTrackColor() const;
    void setUncheckedTrackColor(const QColor& color);
    QColor checkedTrackColor() const;
    void setCheckedTrackColor(const QColor& color);
    QColor disabledTrackColor() const;
    void setDisabledTrackColor(const QColor& color);
    QColor uncheckedThumbColor() const;
    void setUncheckedThumbColor(const QColor& color);
    QColor checkedThumbColor() const;
    void setCheckedThumbColor(const QColor& color);
    QColor disabledThumbColor() const;
    void setDisabledThumbColor(const QColor& color);
    QSize trackSize() const;
    void setTrackSize(const QSize& size);
    int thumbMargin() const;
    void setThumbMargin(int margin);
    int handleRadius() const;
    void setHandleRadius(int radius);
    QIcon handleIcon() const;
    void setHandleIcon(const QIcon& icon);
    bool iconHint() const;
    void setIconHint(bool enabled);
    int textSpacing() const;
    void setTextSpacing(int spacing);
    int animationDuration() const;
    void setAnimationDuration(int duration);

    QSize sizeHint() const override;

protected:
    bool hitButton(const QPoint& pos) const override;
    void paintEvent(QPaintEvent* event) override;
    void changeEvent(QEvent* event) override;
    void showEvent(QShowEvent* event) override;
    void nextCheckState() override;

private:
    void updateTargets(bool immediate = false);

    QColor m_uncheckedTrackColor = QColor(QStringLiteral("#c8ccd4"));
    QColor m_checkedTrackColor = QColor(QStringLiteral("#3478f6"));
    QColor m_disabledTrackColor = QColor(QStringLiteral("#e5e7eb"));
    QColor m_uncheckedThumbColor = QColor(Qt::white);
    QColor m_checkedThumbColor = QColor(Qt::white);
    QColor m_disabledThumbColor = QColor(QStringLiteral("#a8adb7"));
    QSize m_trackSize = QSize(44, 24);
    int m_thumbMargin = 3;
    int m_handleRadius = 0;
    QIcon m_handleIcon;
    bool m_iconHint = false;
    int m_textSpacing = 8;
    ValueTransition m_position;
    ValueTransition m_trackColor;
    ValueTransition m_thumbColor;
};

} // namespace animatedwidgets

#ifndef ANIMATEDWIDGETS_DESIGNER_PLUGIN
using Switch = animatedwidgets::Switch;
#endif
