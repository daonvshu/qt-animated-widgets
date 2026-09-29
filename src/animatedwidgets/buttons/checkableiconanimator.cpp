#include "animatedwidgets/buttons/checkableiconanimator.h"

#include "animatedwidgets/utils/checkableiconpainter.h"

#include <QPainter>

namespace animatedwidgets {

CheckableIconAnimator::CheckableIconAnimator(QObject* parent)
    : QObject(parent)
{
    m_progress.setDuration(180);
    m_progress.setEasingCurve(QEasingCurve::InOutQuad);
    connect(&m_progress, &ValueTransition::valueChanged, this, &CheckableIconAnimator::changed);
}

void CheckableIconAnimator::setIcon(int state, const QPixmap& icon) {
    m_icons[state] = icon;
    reset();
}

QPixmap CheckableIconAnimator::icon(int state) const {
    return m_icons[state];
}

void CheckableIconAnimator::setColor(int state, const QColor& color) {
    m_colors[state] = color;
    reset();
}

QColor CheckableIconAnimator::color(int state) const {
    return m_colors[state];
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

QPixmap CheckableIconAnimator::composite() const {
    QPixmap snapshot(m_iconSize);
    snapshot.fill(Qt::transparent);
    QPainter painter(&snapshot);
    const qreal mix = m_progress.current().toReal();
    if (m_transitionUsesPixmap) {
        CheckableIconPainter::draw(painter, snapshot.rect(), m_transitionPixmap, {}, 1.0 - mix);
    } else {
        CheckableIconPainter::draw(painter, snapshot.rect(), m_icons[m_fromState], m_colors[m_fromState], 1.0 - mix);
    }
    CheckableIconPainter::draw(painter, snapshot.rect(), m_icons[m_toState], m_colors[m_toState], mix);
    return snapshot;
}

void CheckableIconAnimator::paint(QPainter& painter, const QRect& rect, qreal opacity) const {
    if (!m_progress.isRunning()) {
        CheckableIconPainter::draw(painter, rect, m_icons[m_toState], m_colors[m_toState], opacity);
        return;
    }

    const qreal mix = m_progress.current().toReal();
    if (m_transitionUsesPixmap) {
        CheckableIconPainter::draw(painter, rect, m_transitionPixmap, {}, (1.0 - mix) * opacity);
    } else {
        CheckableIconPainter::draw(painter, rect, m_icons[m_fromState], m_colors[m_fromState], (1.0 - mix) * opacity);
    }
    CheckableIconPainter::draw(painter, rect, m_icons[m_toState], m_colors[m_toState], mix * opacity);
}

} // namespace animatedwidgets
