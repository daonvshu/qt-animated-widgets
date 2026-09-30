#include "animatedwidgets/inputs/compositespinbox.h"

#include "animatedwidgets/inputs/spinbox.h"

CompositeSpinBox::CompositeSpinBox(QWidget* parent)
    : CompositeInputBase(parent)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setHorizontalFrameInset(1);

    m_editor = new SpinBox(this);
    m_editor->setBorderVisible(false);
    m_editor->setNormalBackgroundColor(Qt::transparent);
    m_editor->setHoverBackgroundColor(Qt::transparent);
    m_editor->setFocusBackgroundColor(Qt::transparent);
    m_editor->setErrorBackgroundColor(Qt::transparent);
    m_editor->setDisabledBackgroundColor(Qt::transparent);
    m_editor->setAnimationDuration(0);
    setEditorWidget(m_editor);
    setContentPadding(m_editor->contentPadding());

    connect(m_editor, QOverload<int>::of(&QSpinBox::valueChanged), this, &CompositeSpinBox::valueChanged);
    connect(m_editor, &SpinBox::hasErrorChanged, this, &CompositeInputBase::setHasError);
    connect(this, &CompositeInputBase::hasErrorChanged, m_editor, &SpinBox::setHasError);
}

SpinBox* CompositeSpinBox::editor() const {
    return m_editor;
}

int CompositeSpinBox::value() const {
    return m_editor->value();
}

void CompositeSpinBox::setValue(int value) {
    m_editor->setValue(value);
}

int CompositeSpinBox::minimum() const {
    return m_editor->minimum();
}

void CompositeSpinBox::setMinimum(int minimum) {
    m_editor->setMinimum(minimum);
}

int CompositeSpinBox::maximum() const {
    return m_editor->maximum();
}

void CompositeSpinBox::setMaximum(int maximum) {
    m_editor->setMaximum(maximum);
}

int CompositeSpinBox::singleStep() const {
    return m_editor->singleStep();
}

void CompositeSpinBox::setSingleStep(int step) {
    m_editor->setSingleStep(step);
}

QString CompositeSpinBox::prefix() const {
    return m_editor->prefix();
}

void CompositeSpinBox::setPrefix(const QString& prefix) {
    m_editor->setPrefix(prefix);
}

QString CompositeSpinBox::suffix() const {
    return m_editor->suffix();
}

void CompositeSpinBox::setSuffix(const QString& suffix) {
    m_editor->setSuffix(suffix);
}

bool CompositeSpinBox::isReadOnly() const {
    return m_editor->isReadOnly();
}

void CompositeSpinBox::setReadOnly(bool readOnly) {
    m_editor->setReadOnly(readOnly);
}

bool CompositeSpinBox::stepButtonsVisible() const {
    return m_editor->stepButtonsVisible();
}

void CompositeSpinBox::setStepButtonsVisible(bool visible) {
    m_editor->setStepButtonsVisible(visible);
    updateGeometry();
}

int CompositeSpinBox::contentPadding() const {
    return m_editor->contentPadding();
}

void CompositeSpinBox::setContentPadding(int padding) {
    m_editor->setContentPadding(padding);
    const int inset = m_editor->contentPadding();
    setSlotMargins(QMargins(inset, 0, inset, 0), QMargins(inset, 0, inset, 0));
    updateGeometry();
}

QSize CompositeSpinBox::sizeHint() const {
    QSize hint = CompositeInputBase::sizeHint();
    hint.setHeight(qMax(36, hint.height()));
    return hint;
}
