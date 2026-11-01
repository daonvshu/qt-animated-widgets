#include "animatedwidgets/buttons/checkableiconanimator.h"

#include "animatedwidgets/utils/checkableiconpainter.h"

#include <QPainter>
#include <QWidget>

namespace animatedwidgets {

CheckableIconAnimator::CheckableIconAnimator(QObject* parent)
    : QObject(parent)
{
    m_progress.setDuration(300);
    m_progress.setEasingCurve(QEasingCurve::InOutQuad);
    connect(&m_progress, &ValueTransition::valueChanged, this, &CheckableIconAnimator::changed);
}

void CheckableIconAnimator::setIcon(int state, const QPixmap& icon) {
    m_icons[state] = QIcon(icon);
    m_iconPixmaps[state] = icon;
    reset();
}

void CheckableIconAnimator::setIcon(int state, const QIcon& icon) {
    m_icons[state] = icon;
    m_iconPixmaps[state] = {};
    reset();
}

QPixmap CheckableIconAnimator::icon(int state) const {
    if (!m_iconPixmaps[state].isNull())
        return m_iconPixmaps[state];
    return m_icons[state].pixmap(m_iconSize);
}

void CheckableIconAnimator::setColor(int state, const QColor& color) {
    m_colors[state] = color;
    reset();
}

QColor CheckableIconAnimator::color(int state) const {
    return m_colors[state];
}

bool CheckableIconAnimator::iconHint() const {
    return m_iconHint;
}

void CheckableIconAnimator::setIconHint(bool enabled) {
    if (m_iconHint == enabled)
        return;
    m_iconHint = enabled;
    reset();
    emit changed();
}

QSize CheckableIconAnimator::iconSize() const {
    return m_iconSize;
}

void CheckableIconAnimator::setIconSize(const QSize& size) {
    m_iconSize = size.expandedTo(QSize(1, 1));
    reset();
}

int CheckableIconAnimator::duration() const {
    return m_progress.duration();
}

void CheckableIconAnimator::setDuration(int duration) {
    m_progress.setDuration(qMax(0, duration));
    if (m_progress.duration() == 0) {
        reset();
    }
}

int CheckableIconAnimator::state() const {
    return m_toState;
}

void CheckableIconAnimator::setState(int state) {
    if (state == m_toState && !m_progress.isRunning()) {
        return;
    }

    if (m_progress.isRunning()) {
        m_transitionPixmap = composite();
        m_transitionUsesPixmap = true;
    } else {
        m_transitionPixmap = {};
        m_transitionUsesPixmap = false;
    }
    m_fromState = m_toState;
    m_toState = state;

    if (m_progress.duration() == 0) {
        reset();
        return;
    }
    m_progress.setTarget(0.0, true);
    m_progress.setTarget(1.0);
}

void CheckableIconAnimator::reset() {
    m_progress.setTarget(1.0, true);
    m_fromState = m_toState;
    m_transitionPixmap = {};
    m_transitionUsesPixmap = false;
}

qreal CheckableIconAnimator::devicePixelRatio() const {
    if (const auto* widget = qobject_cast<const QWidget*>(parent()))
        return widget->devicePixelRatioF();
    return 1.0;
}

QPixmap CheckableIconAnimator::composite() const {
    const qreal ratio = devicePixelRatio();
    QPixmap snapshot((QSizeF(m_iconSize) * ratio).toSize());
    snapshot.setDevicePixelRatio(ratio);
    snapshot.fill(Qt::transparent);

    QPainter painter(&snapshot);
    const QRectF rect(QPointF(0, 0), QSizeF(m_iconSize));
    const qreal mix = m_progress.current().toReal();
    if (m_transitionUsesPixmap) {
        CheckableIconPainter::draw(painter, rect, m_transitionPixmap, {}, 1.0 - mix);
    } else {
        CheckableIconPainter::draw(painter, rect, m_icons[m_fromState], m_iconHint ? m_colors[m_fromState] : QColor(), 1.0 - mix);
    }
    CheckableIconPainter::draw(painter, rect, m_icons[m_toState], m_iconHint ? m_colors[m_toState] : QColor(), mix);
    return snapshot;
}

void CheckableIconAnimator::paint(QPainter& painter, const QRectF& rect, qreal opacity) const {
    if (!m_progress.isRunning()) {
        CheckableIconPainter::draw(painter, rect, m_icons[m_toState], m_iconHint ? m_colors[m_toState] : QColor(), opacity);
        return;
    }

    const qreal mix = m_progress.current().toReal();
    if (m_transitionUsesPixmap) {
        CheckableIconPainter::draw(painter, rect, m_transitionPixmap, {}, (1.0 - mix) * opacity);
    } else {
        CheckableIconPainter::draw(painter, rect, m_icons[m_fromState], m_iconHint ? m_colors[m_fromState] : QColor(), (1.0 - mix) * opacity);
    }
    CheckableIconPainter::draw(painter, rect, m_icons[m_toState], m_iconHint ? m_colors[m_toState] : QColor(), mix * opacity);
}

} // namespace animatedwidgets
