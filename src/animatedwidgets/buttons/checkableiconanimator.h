#pragma once

#include "animatedwidgets/core/valuetransition.h"

#include <QColor>
#include <QPixmap>
#include <QRect>
#include <QSize>

class QPainter;

namespace animatedwidgets {

class CheckableIconAnimator final : public QObject {
    Q_OBJECT

public:
    enum State {
        Unchecked = 0,
        PartiallyChecked = 1,
        Checked = 2
    };

    explicit CheckableIconAnimator(QObject* parent = nullptr);

    void setIcon(int state, const QPixmap& icon);
    QPixmap icon(int state) const;
    void setColor(int state, const QColor& color);
    QColor color(int state) const;

    QSize iconSize() const;
    void setIconSize(const QSize& size);
    int duration() const;
    void setDuration(int duration);

    int state() const;
    void setState(int state);
    void reset();

    void paint(QPainter& painter, const QRect& rect, qreal opacity = 1.0) const;

signals:
    void changed();

private:
    QPixmap composite() const;

    QPixmap m_icons[3];
    QColor m_colors[3];
    QSize m_iconSize = QSize(18, 18);
    int m_fromState = Unchecked;
    int m_toState = Unchecked;
    QPixmap m_transitionPixmap;
    bool m_transitionUsesPixmap = false;
    ValueTransition m_progress;
};

} // namespace animatedwidgets
