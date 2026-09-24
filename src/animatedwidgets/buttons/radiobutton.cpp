#include "animatedwidgets/buttons/radiobutton.h"

#include "animatedwidgets/buttons/checkableiconpainter.h"

#include <QEvent>
#include <QPainter>

namespace animatedwidgets {

RadioButton::RadioButton(QWidget* parent)
    : QRadioButton(parent), m_animation(this)
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
    connect(this, &QRadioButton::toggled, this, [this](bool checked) {
        startAnimation(stateFromChecked(checked));
    });
}

QPixmap RadioButton::checkedIcon() const { return m_checkedIcon; }
void RadioButton::setCheckedIcon(const QPixmap& icon)
{
    m_checkedIcon = icon;
    resetAnimation();
    updateGeometry();
    update();
}

QPixmap RadioButton::uncheckedIcon() const { return m_uncheckedIcon; }
void RadioButton::setUncheckedIcon(const QPixmap& icon)
{
    m_uncheckedIcon = icon;
    resetAnimation();
    updateGeometry();
    update();
}

QColor RadioButton::checkedIconColor() const { return m_checkedIconColor; }
void RadioButton::setCheckedIconColor(const QColor& color)
{
    m_checkedIconColor = color;
    resetAnimation();
    update();
}

QColor RadioButton::uncheckedIconColor() const { return m_uncheckedIconColor; }
void RadioButton::setUncheckedIconColor(const QColor& color)
{
    m_uncheckedIconColor = color;
    resetAnimation();
    update();
}

QSize RadioButton::iconSize() const { return m_iconSize; }
void RadioButton::setIconSize(const QSize& size)
{
    m_iconSize = size.expandedTo(QSize(1, 1));
    resetAnimation();
    updateGeometry();
    update();
}

int RadioButton::iconSpacing() const { return m_iconSpacing; }
void RadioButton::setIconSpacing(int spacing)
{
    m_iconSpacing = qMax(0, spacing);
    updateGeometry();
    update();
}

int RadioButton::animationDuration() const { return m_animation.duration(); }
void RadioButton::setAnimationDuration(int duration)
{
    const int normalizedDuration = qMax(0, duration);
    m_animation.setDuration(normalizedDuration);
    if (normalizedDuration == 0) resetAnimation();
}

RadioButton::IconState RadioButton::stateFromChecked(bool checked) const
{
    return checked ? IconState::Checked : IconState::Unchecked;
}

QPixmap RadioButton::iconForState(IconState state) const
{
    return state == IconState::Checked ? m_checkedIcon : m_uncheckedIcon;
}

QColor RadioButton::colorForState(IconState state) const
{
    return state == IconState::Checked ? m_checkedIconColor : m_uncheckedIconColor;
}

QPixmap RadioButton::currentComposite() const
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

void RadioButton::startAnimation(IconState state)
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

void RadioButton::resetAnimation()
{
    m_animation.stop();
    m_fromState = m_toState = stateFromChecked(isChecked());
    m_progress = 1.0;
    m_transitionUsesPixmap = false;
    m_transitionPixmap = {};
}

void RadioButton::drawText(QPainter& painter, const QRect& textRect) const
{
    const QPalette::ColorGroup group = isEnabled() ? QPalette::Active : QPalette::Disabled;
    painter.setPen(palette().color(group, QPalette::ButtonText));
    painter.setFont(font());
    painter.drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, text());
}

QSize RadioButton::sizeHint() const
{
    const QFontMetrics metrics(font());
    return QSize(m_iconSize.width() + m_iconSpacing + metrics.horizontalAdvance(text()) + 4,
                 qMax(m_iconSize.height(), metrics.height()) + 4);
}

void RadioButton::paintEvent(QPaintEvent*)
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
        drawCheckableIcon(painter, iconRect, iconForState(stateFromChecked(isChecked())),
                          colorForState(stateFromChecked(isChecked())), enabledOpacity);
    }

    const int textX = layoutDirection() == Qt::RightToLeft ? 0 : iconRect.right() + 1 + m_iconSpacing;
    const int textWidth = width() - m_iconSize.width() - m_iconSpacing - 1;
    const QRect textRect(textX, 0, textWidth, height());
    drawText(painter, textRect);
}

void RadioButton::changeEvent(QEvent* event)
{
    QRadioButton::changeEvent(event);
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
