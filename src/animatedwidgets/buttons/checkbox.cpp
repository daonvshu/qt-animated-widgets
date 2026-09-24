#include "animatedwidgets/buttons/checkbox.h"

#include "animatedwidgets/buttons/checkableiconpainter.h"

#include <QEvent>
#include <QPainter>

namespace animatedwidgets {

CheckBox::CheckBox(QWidget* parent)
    : QCheckBox(parent), m_animation(this)
{
    m_animation.setDuration(180);
    m_animation.setEasingCurve(QEasingCurve::InOutQuad);
    connect(&m_animation, &QVariantAnimation::valueChanged, this, [this](const QVariant& value) {
        m_progress = value.toReal();
        update();
    });
    connect(&m_animation, &QVariantAnimation::finished, this, [this] {
        m_progress = 1.0;
        m_transitionUsesPixmap = false;
        m_transitionPixmap = {};
        update();
    });
    connect(this, QOverload<int>::of(&QCheckBox::stateChanged), this, [this](int state) {
        startAnimation(static_cast<Qt::CheckState>(state));
    });
}

QPixmap CheckBox::checkedIcon() const { return m_checkedIcon; }
void CheckBox::setCheckedIcon(const QPixmap& icon)
{
    m_checkedIcon = icon;
    resetAnimation();
    updateGeometry();
    update();
}

QPixmap CheckBox::uncheckedIcon() const { return m_uncheckedIcon; }
void CheckBox::setUncheckedIcon(const QPixmap& icon)
{
    m_uncheckedIcon = icon;
    resetAnimation();
    updateGeometry();
    update();
}

QPixmap CheckBox::partiallyCheckedIcon() const { return m_partiallyCheckedIcon; }
void CheckBox::setPartiallyCheckedIcon(const QPixmap& icon)
{
    m_partiallyCheckedIcon = icon;
    resetAnimation();
    updateGeometry();
    update();
}

QColor CheckBox::checkedIconColor() const { return m_checkedIconColor; }
void CheckBox::setCheckedIconColor(const QColor& color)
{
    m_checkedIconColor = color;
    resetAnimation();
    update();
}

QColor CheckBox::uncheckedIconColor() const { return m_uncheckedIconColor; }
void CheckBox::setUncheckedIconColor(const QColor& color)
{
    m_uncheckedIconColor = color;
    resetAnimation();
    update();
}

QColor CheckBox::partiallyCheckedIconColor() const { return m_partiallyCheckedIconColor; }
void CheckBox::setPartiallyCheckedIconColor(const QColor& color)
{
    m_partiallyCheckedIconColor = color;
    resetAnimation();
    update();
}

QSize CheckBox::iconSize() const { return m_iconSize; }
void CheckBox::setIconSize(const QSize& size)
{
    m_iconSize = size.expandedTo(QSize(1, 1));
    resetAnimation();
    updateGeometry();
    update();
}

int CheckBox::iconSpacing() const { return m_iconSpacing; }
void CheckBox::setIconSpacing(int spacing)
{
    m_iconSpacing = qMax(0, spacing);
    updateGeometry();
    update();
}

int CheckBox::animationDuration() const { return m_animation.duration(); }
void CheckBox::setAnimationDuration(int duration)
{
    const int normalizedDuration = qMax(0, duration);
    m_animation.setDuration(normalizedDuration);
    if (normalizedDuration == 0) resetAnimation();
}

QPixmap CheckBox::iconForState(Qt::CheckState state) const
{
    switch (state) {
    case Qt::Checked: return m_checkedIcon;
    case Qt::PartiallyChecked: return m_partiallyCheckedIcon;
    case Qt::Unchecked: return m_uncheckedIcon;
    }
    return m_uncheckedIcon;
}

QColor CheckBox::colorForState(Qt::CheckState state) const
{
    switch (state) {
    case Qt::Checked: return m_checkedIconColor;
    case Qt::PartiallyChecked: return m_partiallyCheckedIconColor;
    case Qt::Unchecked: return m_uncheckedIconColor;
    }
    return {};
}

QPixmap CheckBox::currentComposite() const
{
    QPixmap snapshot(m_iconSize);
    snapshot.fill(Qt::transparent);
    QPainter painter(&snapshot);
    if (m_transitionUsesPixmap) {
        drawCheckableIcon(painter, snapshot.rect(), m_transitionPixmap, {}, 1.0 - m_progress);
    } else {
        drawCheckableIcon(painter, snapshot.rect(), iconForState(m_fromState),
                          colorForState(m_fromState), 1.0 - m_progress);
    }
    drawCheckableIcon(painter, snapshot.rect(), iconForState(m_toState),
                      colorForState(m_toState), m_progress);
    return snapshot;
}

void CheckBox::startAnimation(Qt::CheckState state)
{
    if (state == m_toState && m_animation.state() != QAbstractAnimation::Running) return;

    const bool wasRunning = m_animation.state() == QAbstractAnimation::Running;
    if (wasRunning) {
        m_transitionPixmap = currentComposite();
        m_transitionUsesPixmap = true;
    } else {
        m_transitionPixmap = {};
        m_transitionUsesPixmap = false;
    }
    m_fromState = m_toState;
    m_toState = state;
    m_progress = 0.0;
    m_animation.stop();
    m_animation.setStartValue(0.0);
    m_animation.setEndValue(1.0);
    if (m_animation.duration() == 0) {
        resetAnimation();
    } else {
        m_animation.start();
    }
}

void CheckBox::resetAnimation()
{
    m_animation.stop();
    m_fromState = m_toState = checkState();
    m_progress = 1.0;
    m_transitionUsesPixmap = false;
    m_transitionPixmap = {};
}

void CheckBox::drawText(QPainter& painter, const QRect& textRect) const
{
    const QPalette::ColorGroup group = isEnabled() ? QPalette::Active : QPalette::Disabled;
    painter.setPen(palette().color(group, QPalette::ButtonText));
    painter.setFont(font());
    painter.drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, text());
}

QSize CheckBox::sizeHint() const
{
    const QFontMetrics metrics(font());
    return QSize(m_iconSize.width() + m_iconSpacing + metrics.horizontalAdvance(text()) + 4,
                 qMax(m_iconSize.height(), metrics.height()) + 4);
}

void CheckBox::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    const qreal enabledOpacity = isEnabled() ? 1.0 : 0.45;
    const int iconX = layoutDirection() == Qt::RightToLeft ? width() - m_iconSize.width() : 0;
    const QRect iconRect(iconX, (height() - m_iconSize.height()) / 2,
                         m_iconSize.width(), m_iconSize.height());
    if (m_animation.state() == QAbstractAnimation::Running) {
        if (m_transitionUsesPixmap) {
            drawCheckableIcon(painter, iconRect, m_transitionPixmap, {},
                              (1.0 - m_progress) * enabledOpacity);
        } else {
            drawCheckableIcon(painter, iconRect, iconForState(m_fromState),
                              colorForState(m_fromState), (1.0 - m_progress) * enabledOpacity);
        }
        drawCheckableIcon(painter, iconRect, iconForState(m_toState),
                          colorForState(m_toState), m_progress * enabledOpacity);
    } else {
        drawCheckableIcon(painter, iconRect, iconForState(checkState()),
                          colorForState(checkState()), enabledOpacity);
    }

    const int textX = layoutDirection() == Qt::RightToLeft ? 0 : iconRect.right() + 1 + m_iconSpacing;
    const int textWidth = width() - m_iconSize.width() - m_iconSpacing - 1;
    const QRect textRect(textX, 0, textWidth, height());
    drawText(painter, textRect);
}

void CheckBox::changeEvent(QEvent* event)
{
    QCheckBox::changeEvent(event);
    switch (event->type()) {
    case QEvent::EnabledChange:
    case QEvent::FontChange:
    case QEvent::PaletteChange:
    case QEvent::StyleChange:
        resetAnimation();
        updateGeometry();
        update();
        break;
    default:
        break;
    }
}

} // namespace animatedwidgets
