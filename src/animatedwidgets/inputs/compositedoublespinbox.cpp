#include "animatedwidgets/inputs/compositedoublespinbox.h"

#include "animatedwidgets/inputs/doublespinbox.h"

namespace animatedwidgets {

CompositeDoubleSpinBox::CompositeDoubleSpinBox(QWidget* parent)
    : CompositeInputBase(parent)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setHorizontalFrameInset(1);

    m_editor = new DoubleSpinBox(this);
    m_editor->setBorderVisible(false);
    m_editor->setBackgroundNormal(Qt::transparent);
    m_editor->setBackgroundHover(Qt::transparent);
    m_editor->setBackgroundFocus(Qt::transparent);
    m_editor->setBackgroundError(Qt::transparent);
    m_editor->setBackgroundDisabled(Qt::transparent);
    m_editor->setAnimationDuration(0);
    setEditorWidget(m_editor);
    setContentPadding(m_editor->contentPadding());

    connect(m_editor, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &CompositeDoubleSpinBox::valueChanged);
    connect(m_editor, &DoubleSpinBox::hasErrorChanged, this, &CompositeInputBase::setHasError);
    connect(this, &CompositeInputBase::hasErrorChanged, m_editor, &DoubleSpinBox::setHasError);
}

DoubleSpinBox* CompositeDoubleSpinBox::editor() const {
    return m_editor;
}

double CompositeDoubleSpinBox::value() const {
    return m_editor->value();
}

void CompositeDoubleSpinBox::setValue(double value) {
    m_editor->setValue(value);
}

double CompositeDoubleSpinBox::minimum() const {
    return m_editor->minimum();
}

void CompositeDoubleSpinBox::setMinimum(double minimum) {
    m_editor->setMinimum(minimum);
}

double CompositeDoubleSpinBox::maximum() const {
    return m_editor->maximum();
}

void CompositeDoubleSpinBox::setMaximum(double maximum) {
    m_editor->setMaximum(maximum);
}

double CompositeDoubleSpinBox::singleStep() const {
    return m_editor->singleStep();
}

void CompositeDoubleSpinBox::setSingleStep(double step) {
    m_editor->setSingleStep(step);
}

int CompositeDoubleSpinBox::decimals() const {
    return m_editor->decimals();
}

void CompositeDoubleSpinBox::setDecimals(int decimals) {
    m_editor->setDecimals(decimals);
}

QString CompositeDoubleSpinBox::prefix() const {
    return m_editor->prefix();
}

void CompositeDoubleSpinBox::setPrefix(const QString& prefix) {
    m_editor->setPrefix(prefix);
}

QString CompositeDoubleSpinBox::suffix() const {
    return m_editor->suffix();
}

void CompositeDoubleSpinBox::setSuffix(const QString& suffix) {
    m_editor->setSuffix(suffix);
}

bool CompositeDoubleSpinBox::isReadOnly() const {
    return m_editor->isReadOnly();
}

void CompositeDoubleSpinBox::setReadOnly(bool readOnly) {
    m_editor->setReadOnly(readOnly);
}

bool CompositeDoubleSpinBox::stepButtonsVisible() const {
    return m_editor->stepButtonsVisible();
}

void CompositeDoubleSpinBox::setStepButtonsVisible(bool visible) {
    m_editor->setStepButtonsVisible(visible);
    updateGeometry();
}

bool CompositeDoubleSpinBox::iconHint() const {
    return m_editor->iconHint();
}

void CompositeDoubleSpinBox::setIconHint(bool enabled) {
    m_editor->setIconHint(enabled);
}

int CompositeDoubleSpinBox::contentPadding() const {
    return m_editor->contentPadding();
}

void CompositeDoubleSpinBox::setContentPadding(int padding) {
    m_editor->setContentPadding(padding);
    const int inset = m_editor->contentPadding();
    setSlotMargins(QMargins(inset, 0, inset, 0), QMargins(inset, 0, inset, 0));
    updateGeometry();
}

QSize CompositeDoubleSpinBox::sizeHint() const {
    QSize hint = CompositeInputBase::sizeHint();
    hint.setHeight(qMax(36, hint.height()));
    return hint;
}

} // namespace animatedwidgets
