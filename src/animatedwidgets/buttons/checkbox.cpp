#include "animatedwidgets/buttons/checkbox.h"

#include "animatedwidgets/utils/buttonpainter.h"

#include <QEvent>
#include <QPainter>

namespace animatedwidgets {

CheckBox::CheckBox(QWidget* parent)
    : QCheckBox(parent)
    , m_animator(this)
{
    connect(&m_animator, &CheckableIconAnimator::changed, this, [this] { update(); });
    connect(this, QOverload<int>::of(&QCheckBox::stateChanged), this, [this](int state) { m_animator.setState(state); });
}

QPixmap CheckBox::checkedIcon() const {
    return m_animator.icon(CheckableIconAnimator::Checked);
}

void CheckBox::setCheckedIcon(const QPixmap& icon) {
    m_animator.setIcon(CheckableIconAnimator::Checked, icon);
    updateGeometry();
}

void CheckBox::setCheckedIcon(const QIcon& icon) {
    m_animator.setIcon(CheckableIconAnimator::Checked, icon);
    updateGeometry();
}

QPixmap CheckBox::uncheckedIcon() const {
    return m_animator.icon(CheckableIconAnimator::Unchecked);
}

void CheckBox::setUncheckedIcon(const QPixmap& icon) {
    m_animator.setIcon(CheckableIconAnimator::Unchecked, icon);
    updateGeometry();
}

void CheckBox::setUncheckedIcon(const QIcon& icon) {
    m_animator.setIcon(CheckableIconAnimator::Unchecked, icon);
    updateGeometry();
}

QPixmap CheckBox::partiallyCheckedIcon() const {
    return m_animator.icon(CheckableIconAnimator::PartiallyChecked);
}

void CheckBox::setPartiallyCheckedIcon(const QPixmap& icon) {
    m_animator.setIcon(CheckableIconAnimator::PartiallyChecked, icon);
    updateGeometry();
}

void CheckBox::setPartiallyCheckedIcon(const QIcon& icon) {
    m_animator.setIcon(CheckableIconAnimator::PartiallyChecked, icon);
    updateGeometry();
}

bool CheckBox::iconHint() const {
    return m_animator.iconHint();
}

void CheckBox::setIconHint(bool enabled) {
    m_animator.setIconHint(enabled);
}

QColor CheckBox::checkedIconColor() const {
    return m_animator.color(CheckableIconAnimator::Checked);
}

void CheckBox::setCheckedIconColor(const QColor& color) {
    m_animator.setColor(CheckableIconAnimator::Checked, color);
}

QColor CheckBox::uncheckedIconColor() const {
    return m_animator.color(CheckableIconAnimator::Unchecked);
}

void CheckBox::setUncheckedIconColor(const QColor& color) {
    m_animator.setColor(CheckableIconAnimator::Unchecked, color);
}

QColor CheckBox::partiallyCheckedIconColor() const {
    return m_animator.color(CheckableIconAnimator::PartiallyChecked);
}

void CheckBox::setPartiallyCheckedIconColor(const QColor& color) {
    m_animator.setColor(CheckableIconAnimator::PartiallyChecked, color);
}

QSize CheckBox::iconSize() const {
    return m_animator.iconSize();
}

void CheckBox::setIconSize(const QSize& size) {
    m_animator.setIconSize(size);
    updateGeometry();
}

int CheckBox::iconSpacing() const {
    return m_iconSpacing;
}

void CheckBox::setIconSpacing(int spacing) {
    m_iconSpacing = qMax(0, spacing);
    updateGeometry();
    update();
}

int CheckBox::animationDuration() const {
    return m_animator.duration();
}

void CheckBox::setAnimationDuration(int duration) {
    m_animator.setDuration(duration);
}

QSize CheckBox::sizeHint() const {
    return ButtonPainter::checkableSizeHint(m_animator.iconSize(), m_iconSpacing, QFontMetrics(font()), text());
}

void CheckBox::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    const QRectF bounds(rect());
    const QSize iconSize = m_animator.iconSize();
    m_animator.paint(painter, ButtonPainter::checkableIndicatorRect(bounds, iconSize, layoutDirection()), isEnabled() ? 1.0 : 0.45);

    const QPalette::ColorGroup group = isEnabled() ? QPalette::Active : QPalette::Disabled;
    painter.setPen(palette().color(group, QPalette::ButtonText));
    painter.setFont(font());
    painter.drawText(ButtonPainter::checkableTextRect(bounds, iconSize, m_iconSpacing, layoutDirection()), Qt::AlignLeft | Qt::AlignVCenter, text());
}

void CheckBox::changeEvent(QEvent* event) {
    QCheckBox::changeEvent(event);
    switch (event->type()) {
        case QEvent::EnabledChange:
        case QEvent::FontChange:
        case QEvent::PaletteChange:
        case QEvent::StyleChange:
            m_animator.reset();
            updateGeometry();
            update();
            break;
        default:
            break;
    }
}

} // namespace animatedwidgets
