#include "animatedwidgets/inputs/doublespinbox.h"

#include "animatedwidgets/buttons/hovericonbutton.h"

#include <QLineEdit>
#include <QPainter>
#include <QResizeEvent>

DoubleSpinBox::DoubleSpinBox(QWidget* parent)
    : QDoubleSpinBox(parent)
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
        button->setHoverBackgroundColor(QColor(QStringLiteral("#e9f1ff")));
        button->setFocusPolicy(Qt::NoFocus);
        button->setAutoRepeat(true);
        button->setAutoRepeatDelay(350);
        button->setAutoRepeatInterval(80);
    }

    connect(m_upButton, &QPushButton::clicked, this, [this] { stepUp(); });
    connect(m_downButton, &QPushButton::clicked, this, [this] { stepDown(); });
    connect(this, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this] { updateStepButtons(); });
    updateStepButtons();
}

void DoubleSpinBox::setHasError(bool error) {
    if (hasError() == error)
        return;
    SpinBoxAppearance::setHasError(error);
    emit hasErrorChanged(error);
}

QSize DoubleSpinBox::stepButtonSize() const {
    return m_stepButtonSize;
}

void DoubleSpinBox::setStepButtonSize(const QSize& size) {
    const QSize bounded(qMax(1, size.width()), qMax(1, size.height()));
    if (m_stepButtonSize == bounded)
        return;
    m_stepButtonSize = bounded;
    setTrailingControlWidth(m_stepButtonsVisible ? m_stepButtonSize.width() + m_stepButtonRightMargin + 8 : 0);
    updateStepButtonGeometry();
}

int DoubleSpinBox::stepButtonSpacing() const {
    return m_stepButtonSpacing;
}

void DoubleSpinBox::setStepButtonSpacing(int spacing) {
    m_stepButtonSpacing = qMax(0, spacing);
    updateGeometry();
    updateStepButtonGeometry();
}

int DoubleSpinBox::stepButtonRightMargin() const {
    return m_stepButtonRightMargin;
}

void DoubleSpinBox::setStepButtonRightMargin(int margin) {
    m_stepButtonRightMargin = qMax(0, margin);
    setTrailingControlWidth(m_stepButtonsVisible ? m_stepButtonSize.width() + m_stepButtonRightMargin + 8 : 0);
    updateStepButtonGeometry();
}

QSize DoubleSpinBox::stepIconSize() const {
    return m_upButton->iconSize();
}

void DoubleSpinBox::setStepIconSize(const QSize& size) {
    const QSize bounded(qMax(1, size.width()), qMax(1, size.height()));
    m_upButton->setIconSize(bounded);
    m_downButton->setIconSize(bounded);
}

QIcon DoubleSpinBox::stepUpIcon() const {
    return m_upButton->icon();
}

void DoubleSpinBox::setStepUpIcon(const QIcon& icon) {
    m_upButton->setIcon(icon);
}

QIcon DoubleSpinBox::stepDownIcon() const {
    return m_downButton->icon();
}

void DoubleSpinBox::setStepDownIcon(const QIcon& icon) {
    m_downButton->setIcon(icon);
}

bool DoubleSpinBox::stepButtonsVisible() const {
    return m_stepButtonsVisible;
}

void DoubleSpinBox::setStepButtonsVisible(bool visible) {
    if (m_stepButtonsVisible == visible)
        return;
    m_stepButtonsVisible = visible;
    m_upButton->setVisible(visible);
    m_downButton->setVisible(visible);
    setTrailingControlWidth(visible ? m_stepButtonSize.width() + m_stepButtonRightMargin + 8 : 0);
    updateGeometry();
}

QSize DoubleSpinBox::sizeHint() const {
    QSize hint = QDoubleSpinBox::sizeHint();
    hint.setWidth(qMax(hint.width(), qMax(96, (m_stepButtonsVisible ? m_stepButtonSize.width() + m_stepButtonRightMargin : 0) + 2 * contentPadding() + 40)));
    hint.setHeight(qMax(hint.height(), m_stepButtonsVisible ? 2 * m_stepButtonSize.height() + m_stepButtonSpacing + 4 : 34));
    return hint;
}

bool DoubleSpinBox::event(QEvent* event) {
    const bool handled = QDoubleSpinBox::event(event);
    handleEvent(event);
    return handled;
}

void DoubleSpinBox::paintEvent(QPaintEvent*) {
    updateStepButtons();
    QPainter painter(this);
    paint(painter);
}

void DoubleSpinBox::resizeEvent(QResizeEvent* event) {
    QDoubleSpinBox::resizeEvent(event);
    updateStepButtonGeometry();
}

void DoubleSpinBox::updateStepButtonGeometry() {
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

void DoubleSpinBox::updateStepButtons() {
    const StepEnabled steps = stepEnabled();
    m_upButton->setEnabled(isEnabled() && (steps & StepUpEnabled));
    m_downButton->setEnabled(isEnabled() && (steps & StepDownEnabled));
}
