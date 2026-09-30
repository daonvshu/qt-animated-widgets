#include "animatedwidgets/inputs/spinbox.h"

#include "animatedwidgets/buttons/hovericonbutton.h"

#include <QLineEdit>
#include <QPainter>
#include <QResizeEvent>

namespace animatedwidgets {

SpinBox::SpinBox(QWidget* parent)
    : QSpinBox(parent)
    , SpinBoxAppearance(this)
{
    setFrame(false);
    setButtonSymbols(QAbstractSpinBox::NoButtons);
    setFocusPolicy(Qt::ClickFocus);
    setAttribute(Qt::WA_Hover, true);
    setEditor(lineEdit());

    m_upButton = new HoverIconButton(this);
    m_downButton = new HoverIconButton(this);
    m_upButton->setIcon(QIcon(QStringLiteral(":/animatedwidgets/inputs/stepup.svg")));
    m_downButton->setIcon(QIcon(QStringLiteral(":/animatedwidgets/inputs/stepdown.svg")));
    for (auto* button : {m_upButton, m_downButton}) {
        button->setIconSize(QSize(12, 12));
        button->setCornerRadius(3);
        button->setBackgroundHover(QColor(QStringLiteral("#e9f1ff")));
        button->setAnimateContentColor(true);
        button->setFocusPolicy(Qt::NoFocus);
        button->setAutoRepeat(true);
        button->setAutoRepeatDelay(350);
        button->setAutoRepeatInterval(80);
    }

    connect(m_upButton, &QPushButton::clicked, this, [this] { stepUp(); });
    connect(m_downButton, &QPushButton::clicked, this, [this] { stepDown(); });
    connect(this, QOverload<int>::of(&QSpinBox::valueChanged), this, [this] { updateStepButtons(); });
    updateStepButtons();
}

void SpinBox::setHasError(bool error) {
    if (hasError() == error)
        return;
    SpinBoxAppearance::setHasError(error);
    emit hasErrorChanged(error);
}

QSize SpinBox::stepButtonSize() const {
    return m_stepButtonSize;
}

void SpinBox::setStepButtonSize(const QSize& size) {
    const QSize bounded(qMax(1, size.width()), qMax(1, size.height()));
    if (m_stepButtonSize == bounded)
        return;
    m_stepButtonSize = bounded;
    setTrailingControlWidth(m_stepButtonsVisible ? m_stepButtonSize.width() + m_stepButtonRightMargin + 8 : 0);
    updateStepButtonGeometry();
}

int SpinBox::stepButtonSpacing() const {
    return m_stepButtonSpacing;
}

void SpinBox::setStepButtonSpacing(int spacing) {
    m_stepButtonSpacing = qMax(0, spacing);
    updateGeometry();
    updateStepButtonGeometry();
}

int SpinBox::stepButtonRightMargin() const {
    return m_stepButtonRightMargin;
}

void SpinBox::setStepButtonRightMargin(int margin) {
    m_stepButtonRightMargin = qMax(0, margin);
    setTrailingControlWidth(m_stepButtonsVisible ? m_stepButtonSize.width() + m_stepButtonRightMargin + 8 : 0);
    updateStepButtonGeometry();
}

QSize SpinBox::stepIconSize() const {
    return m_upButton->iconSize();
}

void SpinBox::setStepIconSize(const QSize& size) {
    const QSize bounded(qMax(1, size.width()), qMax(1, size.height()));
    m_upButton->setIconSize(bounded);
    m_downButton->setIconSize(bounded);
}

QIcon SpinBox::stepUpIcon() const {
    return m_upButton->icon();
}

void SpinBox::setStepUpIcon(const QIcon& icon) {
    m_upButton->setIcon(icon);
}

QIcon SpinBox::stepDownIcon() const {
    return m_downButton->icon();
}

void SpinBox::setStepDownIcon(const QIcon& icon) {
    m_downButton->setIcon(icon);
}

bool SpinBox::stepButtonsVisible() const {
    return m_stepButtonsVisible;
}

void SpinBox::setStepButtonsVisible(bool visible) {
    if (m_stepButtonsVisible == visible)
        return;
    m_stepButtonsVisible = visible;
    m_upButton->setVisible(visible);
    m_downButton->setVisible(visible);
    setTrailingControlWidth(visible ? m_stepButtonSize.width() + m_stepButtonRightMargin + 8 : 0);
    updateGeometry();
}

QColor SpinBox::stepButtonNormalColor() const {
    return m_upButton->contentNormal();
}

void SpinBox::setStepButtonNormalColor(const QColor& color) {
    m_upButton->setContentNormal(color);
    m_downButton->setContentNormal(color);
}

QColor SpinBox::stepButtonHoverColor() const {
    return m_upButton->contentHover();
}

void SpinBox::setStepButtonHoverColor(const QColor& color) {
    m_upButton->setContentHover(color);
    m_downButton->setContentHover(color);
}

QColor SpinBox::stepButtonPressedColor() const {
    return m_upButton->contentPressed();
}

void SpinBox::setStepButtonPressedColor(const QColor& color) {
    m_upButton->setContentPressed(color);
    m_downButton->setContentPressed(color);
}

QColor SpinBox::stepButtonDisabledColor() const {
    return m_upButton->contentDisabled();
}

void SpinBox::setStepButtonDisabledColor(const QColor& color) {
    m_upButton->setContentDisabled(color);
    m_downButton->setContentDisabled(color);
}

QColor SpinBox::stepButtonBackgroundHover() const {
    return m_upButton->backgroundHover();
}

void SpinBox::setStepButtonBackgroundHover(const QColor& color) {
    m_upButton->setBackgroundHover(color);
    m_downButton->setBackgroundHover(color);
}

QColor SpinBox::stepButtonBackgroundPressed() const {
    return m_upButton->backgroundPressed();
}

void SpinBox::setStepButtonBackgroundPressed(const QColor& color) {
    m_upButton->setBackgroundPressed(color);
    m_downButton->setBackgroundPressed(color);
}

QColor SpinBox::stepButtonBackgroundDisabled() const {
    return m_upButton->backgroundDisabled();
}

void SpinBox::setStepButtonBackgroundDisabled(const QColor& color) {
    m_upButton->setBackgroundDisabled(color);
    m_downButton->setBackgroundDisabled(color);
}

HoverIconButton* SpinBox::stepUpButton() const {
    return m_upButton;
}

HoverIconButton* SpinBox::stepDownButton() const {
    return m_downButton;
}

void SpinBox::setAnimationDuration(int duration) {
    SpinBoxAppearance::setAnimationDuration(duration);
    m_upButton->setAnimationDuration(animationDuration());
    m_downButton->setAnimationDuration(animationDuration());
}

QSize SpinBox::sizeHint() const {
    QSize hint = QSpinBox::sizeHint();
    hint.setWidth(qMax(hint.width(), qMax(96, (m_stepButtonsVisible ? m_stepButtonSize.width() + m_stepButtonRightMargin : 0) + 2 * contentPadding() + 40)));
    hint.setHeight(qMax(hint.height(), m_stepButtonsVisible ? 2 * m_stepButtonSize.height() + m_stepButtonSpacing + 4 : 34));
    return hint;
}

bool SpinBox::event(QEvent* event) {
    const bool handled = QSpinBox::event(event);
    handleEvent(event);
    return handled;
}

void SpinBox::paintEvent(QPaintEvent*) {
    updateStepButtons();
    QPainter painter(this);
    paint(painter);
}

void SpinBox::resizeEvent(QResizeEvent* event) {
    QSpinBox::resizeEvent(event);
    updateStepButtonGeometry();
}

void SpinBox::updateStepButtonGeometry() {
    if (!m_upButton)
        return;

    const int buttonWidth = qMin(m_stepButtonSize.width(), qMax(0, width() - m_stepButtonRightMargin));
    const int spacing = qMin(m_stepButtonSpacing, qMax(0, height() - 4));
    const int buttonHeight = qMin(m_stepButtonSize.height(), qMax(0, (height() - 4 - spacing) / 2));
    const int top = (height() - 2 * buttonHeight - spacing) / 2;
    const int left = width() - m_stepButtonRightMargin - buttonWidth;
    m_upButton->setGeometry(left, top, buttonWidth, buttonHeight);
    m_downButton->setGeometry(left, top + buttonHeight + spacing, buttonWidth, buttonHeight);
}

void SpinBox::updateStepButtons() {
    const StepEnabled steps = stepEnabled();
    m_upButton->setEnabled(isEnabled() && (steps & StepUpEnabled));
    m_downButton->setEnabled(isEnabled() && (steps & StepDownEnabled));
}

} // namespace animatedwidgets
