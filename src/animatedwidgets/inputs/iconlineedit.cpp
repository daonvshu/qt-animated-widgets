#include "animatedwidgets/inputs/iconlineedit.h"

#include "animatedwidgets/buttons/hovericonbutton.h"

#include <QEvent>
#include <QResizeEvent>

namespace animatedwidgets {

IconLineEdit::IconLineEdit(QWidget* parent)
    : LineEdit(parent)
{
    Q_INIT_RESOURCE(inputs);
    m_leadingButton = new HoverIconButton(this);
    m_trailingButton = new HoverIconButton(this);
    for (auto* button : {m_leadingButton, m_trailingButton}) {
        button->resize(26, 26);
        button->setVisible(false);
        button->setFocusPolicy(Qt::NoFocus);
        button->setIconSize(QSize(16, 16));
        button->setAnimationDuration(animationDuration());
        button->setContentNormal(QColor(QStringLiteral("#64748b")));
        button->setAnimateContentColor(true);
    }
    m_leadingButton->setBackgroundHover(QColor(0, 0, 0, 0));
    m_trailingButton->setCursor(Qt::PointingHandCursor);
    m_trailingButton->setContentHover(QColor(QStringLiteral("#3478f6")));
    m_trailingButton->setBackgroundHover(QColor(QStringLiteral("#e9effb")));
    m_trailingButton->setAnimateContentColor(true);
    connect(m_leadingButton, &QPushButton::clicked, this, &IconLineEdit::leadingIconClicked);
    connect(m_trailingButton, &QPushButton::clicked, this, &IconLineEdit::trailingIconClicked);
    IconLineEdit::updateContentMargins();
}

QIcon IconLineEdit::leadingIcon() const {
    return m_leadingIcon;
}

void IconLineEdit::setLeadingIcon(const QIcon& icon) {
    m_leadingIcon = icon;
    m_leadingButton->setIcon(icon);
    m_leadingButton->setVisible(m_leadingVisible && !icon.isNull());
    updateContentMargins();
    updateGeometry();
}

QIcon IconLineEdit::trailingIcon() const {
    return m_trailingIcon;
}

void IconLineEdit::setTrailingIcon(const QIcon& icon) {
    m_trailingIcon = icon;
    m_trailingButton->setIcon(icon);
    m_trailingButton->setVisible(m_trailingVisible && !icon.isNull());
    updateContentMargins();
    updateGeometry();
}

QSize IconLineEdit::leadingIconSize() const {
    return m_leadingIconSize;
}

void IconLineEdit::setLeadingIconSize(const QSize& size) {
    const QSize bounded(qMax(1, size.width()), qMax(1, size.height()));
    if (m_leadingIconSize == bounded)
        return;
    m_leadingIconSize = bounded;
    updateContentMargins();
    updateGeometry();
}

QSize IconLineEdit::trailingIconSize() const {
    return m_trailingIconSize;
}

void IconLineEdit::setTrailingIconSize(const QSize& size) {
    const QSize bounded(qMax(1, size.width()), qMax(1, size.height()));
    if (m_trailingIconSize == bounded)
        return;
    m_trailingIconSize = bounded;
    updateContentMargins();
    updateGeometry();
}

bool IconLineEdit::leadingIconVisible() const {
    return m_leadingVisible;
}

void IconLineEdit::setLeadingIconVisible(bool visible) {
    m_leadingVisible = visible;
    m_leadingButton->setVisible(visible && !m_leadingIcon.isNull());
    updateContentMargins();
    updateGeometry();
}

bool IconLineEdit::trailingIconVisible() const {
    return m_trailingVisible;
}

void IconLineEdit::setTrailingIconVisible(bool visible) {
    m_trailingVisible = visible;
    m_trailingButton->setVisible(visible && !m_trailingIcon.isNull());
    updateContentMargins();
    updateGeometry();
}

int IconLineEdit::iconSpacing() const {
    return m_iconSpacing;
}

void IconLineEdit::setIconSpacing(int spacing) {
    m_iconSpacing = qMax(0, spacing);
    updateContentMargins();
}

QColor IconLineEdit::leadingIconNormalColor() const {
    return m_leadingButton->contentNormal();
}

void IconLineEdit::setLeadingIconNormalColor(const QColor& color) {
    m_leadingButton->setContentNormal(color);
}

QColor IconLineEdit::leadingIconHoverColor() const {
    return m_leadingButton->contentHover();
}

void IconLineEdit::setLeadingIconHoverColor(const QColor& color) {
    m_leadingButton->setContentHover(color);
}

QColor IconLineEdit::leadingIconPressedColor() const {
    return m_leadingButton->contentPressed();
}

void IconLineEdit::setLeadingIconPressedColor(const QColor& color) {
    m_leadingButton->setContentPressed(color);
}

QColor IconLineEdit::leadingIconDisabledColor() const {
    return m_leadingButton->contentDisabled();
}

void IconLineEdit::setLeadingIconDisabledColor(const QColor& color) {
    m_leadingButton->setContentDisabled(color);
}

QColor IconLineEdit::leadingIconBackgroundHover() const {
    return m_leadingButton->backgroundHover();
}

void IconLineEdit::setLeadingIconBackgroundHover(const QColor& color) {
    m_leadingButton->setBackgroundHover(color);
}

QColor IconLineEdit::leadingIconBackgroundPressed() const {
    return m_leadingButton->backgroundPressed();
}

void IconLineEdit::setLeadingIconBackgroundPressed(const QColor& color) {
    m_leadingButton->setBackgroundPressed(color);
}

QColor IconLineEdit::leadingIconBackgroundDisabled() const {
    return m_leadingButton->backgroundDisabled();
}

void IconLineEdit::setLeadingIconBackgroundDisabled(const QColor& color) {
    m_leadingButton->setBackgroundDisabled(color);
}

QColor IconLineEdit::trailingIconNormalColor() const {
    return m_trailingButton->contentNormal();
}

void IconLineEdit::setTrailingIconNormalColor(const QColor& color) {
    m_trailingButton->setContentNormal(color);
}

QColor IconLineEdit::trailingIconHoverColor() const {
    return m_trailingButton->contentHover();
}

void IconLineEdit::setTrailingIconHoverColor(const QColor& color) {
    m_trailingButton->setContentHover(color);
}

QColor IconLineEdit::trailingIconPressedColor() const {
    return m_trailingButton->contentPressed();
}

void IconLineEdit::setTrailingIconPressedColor(const QColor& color) {
    m_trailingButton->setContentPressed(color);
}

QColor IconLineEdit::trailingIconDisabledColor() const {
    return m_trailingButton->contentDisabled();
}

void IconLineEdit::setTrailingIconDisabledColor(const QColor& color) {
    m_trailingButton->setContentDisabled(color);
}

QColor IconLineEdit::trailingIconBackgroundHover() const {
    return m_trailingButton->backgroundHover();
}

void IconLineEdit::setTrailingIconBackgroundHover(const QColor& color) {
    m_trailingButton->setBackgroundHover(color);
}

QColor IconLineEdit::trailingIconBackgroundPressed() const {
    return m_trailingButton->backgroundPressed();
}

void IconLineEdit::setTrailingIconBackgroundPressed(const QColor& color) {
    m_trailingButton->setBackgroundPressed(color);
}

QColor IconLineEdit::trailingIconBackgroundDisabled() const {
    return m_trailingButton->backgroundDisabled();
}

void IconLineEdit::setTrailingIconBackgroundDisabled(const QColor& color) {
    m_trailingButton->setBackgroundDisabled(color);
}

HoverIconButton* IconLineEdit::leadingButton() const {
    return m_leadingButton;
}

HoverIconButton* IconLineEdit::trailingButton() const {
    return m_trailingButton;
}

void IconLineEdit::setAnimationDuration(int duration) {
    LineEdit::setAnimationDuration(duration);
    m_leadingButton->setAnimationDuration(animationDuration());
    m_trailingButton->setAnimationDuration(animationDuration());
}

QSize IconLineEdit::sizeHint() const {
    QSize hint = LineEdit::sizeHint();
    int iconHeight = 0;
    if (m_leadingVisible && !m_leadingIcon.isNull())
        iconHeight = qMax(iconHeight, qMax(m_leadingIconSize.width(), m_leadingIconSize.height()));
    if (m_trailingVisible && !m_trailingIcon.isNull())
        iconHeight = qMax(iconHeight, qMax(m_trailingIconSize.width(), m_trailingIconSize.height()));
    hint.setHeight(qMax(hint.height(), iconHeight + 18));
    return hint;
}

void IconLineEdit::resizeEvent(QResizeEvent* event) {
    LineEdit::resizeEvent(event);
    updateContentMargins();
}

bool IconLineEdit::event(QEvent* event) {
    const bool handled = LineEdit::event(event);
    if (event->type() == QEvent::LayoutDirectionChange && m_leadingButton && m_trailingButton) {
        updateContentMargins();
    }
    return handled;
}

QSize IconLineEdit::effectiveIconSize(const QSize& requested) const {
    const int available = qMax(0, height() - 18);
    if (available == 0)
        return QSize(0, 0);
    if (requested.width() <= available && requested.height() <= available)
        return requested;
    const QSize scaled = requested.scaled(available, available, Qt::KeepAspectRatio);
    return QSize(qMax(1, scaled.width()), qMax(1, scaled.height()));
}

void IconLineEdit::updateIconGeometry() {
    const QSize sizes[] = {effectiveIconSize(m_leadingIconSize), effectiveIconSize(m_trailingIconSize)};
    HoverIconButton* buttons[] = {m_leadingButton, m_trailingButton};
    for (int i = 0; i < 2; ++i) {
        const QSize iconSize = sizes[i];
        const int desiredSide = qMax(iconSize.width(), iconSize.height()) + 10;
        const int buttonWidth = qMin(desiredSide, iconSize.width() + 2 * contentPadding());
        const int buttonHeight = qMin(desiredSide, qMax(0, height() - 8));
        const int inset = (buttonWidth - iconSize.width()) / 2;
        const bool onLeft = (i == 0) == (layoutDirection() == Qt::LeftToRight);
        const int x = onLeft ? contentPadding() - inset : width() - contentPadding() - iconSize.width() - inset;
        buttons[i]->setIconSize(iconSize);
        buttons[i]->setGeometry(x, (height() - buttonHeight) / 2, buttonWidth, buttonHeight);
    }
}

void IconLineEdit::updateContentMargins() {
    if (m_leadingButton && m_trailingButton)
        updateIconGeometry();
    const QSize leadingSize = effectiveIconSize(m_leadingIconSize);
    const QSize trailingSize = effectiveIconSize(m_trailingIconSize);
    const int leadingMargin = contentPadding() + (m_leadingButton && !m_leadingButton->isHidden() ? leadingSize.width() + m_iconSpacing : 0);
    const int trailingMargin = contentPadding() + (m_trailingButton && !m_trailingButton->isHidden() ? trailingSize.width() + m_iconSpacing : 0);
    if (layoutDirection() == Qt::RightToLeft) {
        setTextMargins(trailingMargin, 0, leadingMargin, 0);
    } else {
        setTextMargins(leadingMargin, 0, trailingMargin, 0);
    }
}

} // namespace animatedwidgets
