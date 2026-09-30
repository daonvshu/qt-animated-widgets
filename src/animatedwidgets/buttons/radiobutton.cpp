#include "animatedwidgets/buttons/radiobutton.h"

#include "animatedwidgets/utils/buttonpainter.h"

#include <QEvent>
#include <QPainter>

namespace animatedwidgets {

RadioButton::RadioButton(QWidget* parent)
    : QRadioButton(parent)
    , m_animator(this)
{
    connect(&m_animator, &CheckableIconAnimator::changed, this, [this] { update(); });
    connect(this, &QRadioButton::toggled, this,
            [this](bool checked) { m_animator.setState(checked ? CheckableIconAnimator::Checked : CheckableIconAnimator::Unchecked); });
}

QPixmap RadioButton::checkedIcon() const {
    return m_animator.icon(CheckableIconAnimator::Checked);
}

void RadioButton::setCheckedIcon(const QPixmap& icon) {
    m_animator.setIcon(CheckableIconAnimator::Checked, icon);
    updateGeometry();
}

QPixmap RadioButton::uncheckedIcon() const {
    return m_animator.icon(CheckableIconAnimator::Unchecked);
}

void RadioButton::setUncheckedIcon(const QPixmap& icon) {
    m_animator.setIcon(CheckableIconAnimator::Unchecked, icon);
    updateGeometry();
}

QColor RadioButton::checkedIconColor() const {
    return m_animator.color(CheckableIconAnimator::Checked);
}

void RadioButton::setCheckedIconColor(const QColor& color) {
    m_animator.setColor(CheckableIconAnimator::Checked, color);
}

QColor RadioButton::uncheckedIconColor() const {
    return m_animator.color(CheckableIconAnimator::Unchecked);
}

void RadioButton::setUncheckedIconColor(const QColor& color) {
    m_animator.setColor(CheckableIconAnimator::Unchecked, color);
}

QSize RadioButton::iconSize() const {
    return m_animator.iconSize();
}

void RadioButton::setIconSize(const QSize& size) {
    m_animator.setIconSize(size);
    updateGeometry();
}

int RadioButton::iconSpacing() const {
    return m_iconSpacing;
}

void RadioButton::setIconSpacing(int spacing) {
    m_iconSpacing = qMax(0, spacing);
    updateGeometry();
    update();
}

int RadioButton::animationDuration() const {
    return m_animator.duration();
}

void RadioButton::setAnimationDuration(int duration) {
    m_animator.setDuration(duration);
}

QSize RadioButton::sizeHint() const {
    return ButtonPainter::checkableSizeHint(m_animator.iconSize(), m_iconSpacing, QFontMetrics(font()), text());
}

void RadioButton::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    const QSize iconSize = m_animator.iconSize();
    m_animator.paint(painter, ButtonPainter::checkableIndicatorRect(rect(), iconSize, layoutDirection()), isEnabled() ? 1.0 : 0.45);

    const QPalette::ColorGroup group = isEnabled() ? QPalette::Active : QPalette::Disabled;
    painter.setPen(palette().color(group, QPalette::ButtonText));
    painter.setFont(font());
    painter.drawText(ButtonPainter::checkableTextRect(rect(), iconSize, m_iconSpacing, layoutDirection()), Qt::AlignLeft | Qt::AlignVCenter, text());
}

void RadioButton::changeEvent(QEvent* event) {
    QRadioButton::changeEvent(event);
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
