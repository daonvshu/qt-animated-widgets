#pragma once

#include "animatedwidgets/core/valuetransition.h"

#include <QColor>
#include <QIcon>
#include <QPixmap>
#include <QRectF>
#include <QSize>

class QPainter;

namespace animatedwidgets {

class CheckableIconAnimator final : public QObject {
    Q_OBJECT

public:
    enum State { Unchecked = 0, PartiallyChecked = 1, Checked = 2 };

    explicit CheckableIconAnimator(QObject* parent = nullptr);

    void setIcon(int state, const QPixmap& icon);
    void setIcon(int state, const QIcon& icon);
    QPixmap icon(int state) const;
    void setColor(int state, const QColor& color);
    QColor color(int state) const;

    bool iconHint() const;
    void setIconHint(bool enabled);

    QSize iconSize() const;
    void setIconSize(const QSize& size);
    int duration() const;
    void setDuration(int duration);

    int state() const;
    void setState(int state);
    void reset();

    void paint(QPainter& painter, const QRectF& rect, qreal opacity = 1.0) const;

signals:
    void changed();

private:
    qreal devicePixelRatio() const;
    QPixmap composite() const;

    QIcon m_icons[3];
    QPixmap m_iconPixmaps[3];
    QColor m_colors[3];
    QSize m_iconSize = QSize(18, 18);
    int m_fromState = Unchecked;
    int m_toState = Unchecked;
    QPixmap m_transitionPixmap;
    bool m_transitionUsesPixmap = false;
    bool m_iconHint = false;
    ValueTransition m_progress;
};

} // namespace animatedwidgets
