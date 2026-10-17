#include "animatedwidgets/switches/switch.h"
#include "animatedwidgets/utils/iconpainter.h"

#include <QEvent>
#include <QFontMetricsF>
#include <QPainter>
#include <QShowEvent>
#include <QStyle>
#include <QStyleOptionFocusRect>
#include <QtMath>

namespace animatedwidgets {

Switch::Switch(QWidget* parent)
    : QCheckBox(parent)
    , m_position(this)
    , m_trackColor(this)
    , m_thumbColor(this)
{
    setTristate(false);
    for (auto* transition : {&m_position, &m_trackColor, &m_thumbColor}) {
        transition->setDuration(180);
        connect(transition, &ValueTransition::valueChanged, this, [this] { update(); });
    }
    connect(this, &QCheckBox::toggled, this, [this] { updateTargets(); });
    updateTargets(true);
}

QColor Switch::uncheckedTrackColor() const {
    return m_uncheckedTrackColor;
}

void Switch::setUncheckedTrackColor(const QColor& color) {
    m_uncheckedTrackColor = color;
    updateTargets(true);
}

QColor Switch::checkedTrackColor() const {
    return m_checkedTrackColor;
}

void Switch::setCheckedTrackColor(const QColor& color) {
    m_checkedTrackColor = color;
    updateTargets(true);
}

QColor Switch::disabledTrackColor() const {
    return m_disabledTrackColor;
}

void Switch::setDisabledTrackColor(const QColor& color) {
    m_disabledTrackColor = color;
    updateTargets(true);
}

QColor Switch::uncheckedThumbColor() const {
    return m_uncheckedThumbColor;
}

void Switch::setUncheckedThumbColor(const QColor& color) {
    m_uncheckedThumbColor = color;
    updateTargets(true);
}

QColor Switch::checkedThumbColor() const {
    return m_checkedThumbColor;
}

void Switch::setCheckedThumbColor(const QColor& color) {
    m_checkedThumbColor = color;
    updateTargets(true);
}

QColor Switch::disabledThumbColor() const {
    return m_disabledThumbColor;
}

void Switch::setDisabledThumbColor(const QColor& color) {
    m_disabledThumbColor = color;
    updateTargets(true);
}

QSize Switch::trackSize() const {
    return m_trackSize;
}

void Switch::setTrackSize(const QSize& size) {
    const int height = qMax(2, size.height());
    m_trackSize = QSize(qMax(height, size.width()), height);
    updateGeometry();
    update();
}

int Switch::thumbMargin() const {
    return m_thumbMargin;
}

void Switch::setThumbMargin(int margin) {
    m_thumbMargin = qMax(0, margin);
    update();
}

int Switch::handleRadius() const {
    return m_handleRadius > 0 ? m_handleRadius : qMax(1, (m_trackSize.height() - 2 * m_thumbMargin) / 2);
}

void Switch::setHandleRadius(int radius) {
    m_handleRadius = qMax(0, radius);
    update();
}

QIcon Switch::handleIcon() const {
    return m_handleIcon;
}

void Switch::setHandleIcon(const QIcon& icon) {
    m_handleIcon = icon;
    update();
}

bool Switch::iconHint() const {
    return m_iconHint;
}

void Switch::setIconHint(bool enabled) {
    if (m_iconHint == enabled)
        return;
    m_iconHint = enabled;
    update();
}

int Switch::textSpacing() const {
    return m_textSpacing;
}

void Switch::setTextSpacing(int spacing) {
    m_textSpacing = qMax(0, spacing);
    updateGeometry();
    update();
}

int Switch::animationDuration() const {
    return m_position.duration();
}

void Switch::setAnimationDuration(int duration) {
    for (auto* transition : {&m_position, &m_trackColor, &m_thumbColor}) {
        transition->setDuration(duration);
    }
    if (duration <= 0)
        updateTargets(true);
}

QSize Switch::sizeHint() const {
    const bool hasText = !text().isEmpty();
    const QFontMetricsF metrics(font());
    const int width = m_trackSize.width() + (hasText ? m_textSpacing + qCeil(metrics.horizontalAdvance(text())) : 0);
    return QSize(width + 4, qMax(m_trackSize.height(), hasText ? qCeil(metrics.height()) : 0) + 4);
}

void Switch::updateTargets(bool immediate) {
    m_position.setTarget(isChecked() ? 1.0 : 0.0, immediate);
    m_trackColor.setTarget(!isEnabled() ? m_disabledTrackColor : (isChecked() ? m_checkedTrackColor : m_uncheckedTrackColor), immediate);
    m_thumbColor.setTarget(!isEnabled() ? m_disabledThumbColor : (isChecked() ? m_checkedThumbColor : m_uncheckedThumbColor), immediate);
}

bool Switch::hitButton(const QPoint& pos) const {
    return rect().contains(pos);
}

void Switch::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    const QRect bounds = rect();
    const int trackX = layoutDirection() == Qt::RightToLeft ? bounds.right() - m_trackSize.width() - 1 : bounds.left() + 2;
    const QRectF track(trackX, bounds.center().y() - m_trackSize.height() / 2.0, m_trackSize.width(), m_trackSize.height());
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_trackColor.current().value<QColor>());
    painter.drawRoundedRect(track, track.height() / 2, track.height() / 2);

    const qreal margin = qMin(qreal(m_thumbMargin), (track.height() - 2) / 2);
    const qreal radius = qMin(qreal(handleRadius()), (track.height() - 2 * margin) / 2);
    const qreal diameter = 2 * radius;
    const qreal travel = qMax(qreal(0), track.width() - 2 * (margin + radius));
    const qreal progress = qBound(qreal(0), m_position.current().toReal(), qreal(1));
    const qreal offset = (layoutDirection() == Qt::RightToLeft ? 1.0 - progress : progress) * travel;
    const QRectF handle(track.left() + margin + offset, track.center().y() - radius, diameter, diameter);
    if (m_handleIcon.isNull()) {
        painter.setBrush(m_thumbColor.current().value<QColor>());
        painter.drawEllipse(handle);
    } else {
        const QIcon::Mode mode = isEnabled() ? QIcon::Normal : QIcon::Disabled;
        const QIcon::State state = isChecked() ? QIcon::On : QIcon::Off;
        IconPainter::draw(painter, handle.toRect(), m_handleIcon, m_iconHint ? m_thumbColor.current().value<QColor>() : QColor(), mode, state);
    }

    if (!text().isEmpty()) {
        const QRect textRect = layoutDirection() == Qt::RightToLeft
                ? QRect(bounds.left() + 2, bounds.top(), qMax(0, trackX - m_textSpacing - bounds.left() - 2), bounds.height())
                : QRect(int(track.right()) + m_textSpacing, bounds.top(), qMax(0, bounds.right() - int(track.right()) - m_textSpacing), bounds.height());
        const int alignment = Qt::AlignVCenter | (layoutDirection() == Qt::RightToLeft ? Qt::AlignRight : Qt::AlignLeft);
        style()->drawItemText(&painter, textRect, alignment | Qt::TextShowMnemonic, palette(), isEnabled(), text(), QPalette::WindowText);
    }

    if (hasFocus()) {
        QStyleOptionFocusRect option;
        option.initFrom(this);
        option.rect = track.toAlignedRect().adjusted(-2, -2, 2, 2);
        option.backgroundColor = palette().color(QPalette::Window);
        style()->drawPrimitive(QStyle::PE_FrameFocusRect, &option, &painter, this);
    }
}

void Switch::changeEvent(QEvent* event) {
    QCheckBox::changeEvent(event);
    switch (event->type()) {
        case QEvent::EnabledChange:
            updateTargets(true);
            break;
        case QEvent::FontChange:
        case QEvent::StyleChange:
            updateGeometry();
            update();
            break;
        default:
            break;
    }
}

void Switch::showEvent(QShowEvent* event) {
    QCheckBox::showEvent(event);
    updateTargets(true);
}

void Switch::nextCheckState() {
    setChecked(!isChecked());
}

} // namespace animatedwidgets
