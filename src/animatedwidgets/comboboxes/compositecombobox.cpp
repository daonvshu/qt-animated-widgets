#include "animatedwidgets/comboboxes/compositecombobox.h"

#include "animatedwidgets/comboboxes/combobox.h"

CompositeComboBox::CompositeComboBox(QWidget* parent)
    : CompositeInputBase(parent)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setHorizontalFrameInset(1);

    m_editor = new ComboBox(this);
    m_editor->setBorderVisible(false);
    m_editor->setNormalBackgroundColor(Qt::transparent);
    m_editor->setHoverBackgroundColor(Qt::transparent);
    m_editor->setFocusBackgroundColor(Qt::transparent);
    m_editor->setErrorBackgroundColor(Qt::transparent);
    m_editor->setDisabledBackgroundColor(Qt::transparent);
    m_editor->setAnimationDuration(0);
    setEditorWidget(m_editor);
    setContentPadding(m_editor->contentPadding());

    connect(m_editor, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CompositeComboBox::currentIndexChanged);
    connect(m_editor, &QComboBox::currentTextChanged, this, &CompositeComboBox::currentTextChanged);
    connect(m_editor, &ComboBox::hasErrorChanged, this, &CompositeInputBase::setHasError);
    connect(this, &CompositeInputBase::hasErrorChanged, m_editor, &ComboBox::setHasError);
}

ComboBox* CompositeComboBox::editor() const {
    return m_editor;
}

int CompositeComboBox::currentIndex() const {
    return m_editor->currentIndex();
}

void CompositeComboBox::setCurrentIndex(int index) {
    m_editor->setCurrentIndex(index);
}

QString CompositeComboBox::currentText() const {
    return m_editor->currentText();
}

void CompositeComboBox::setCurrentText(const QString& text) {
    m_editor->setCurrentText(text);
}

bool CompositeComboBox::isEditable() const {
    return m_editor->isEditable();
}

void CompositeComboBox::setEditable(bool editable) {
    m_editor->setEditable(editable);
}

bool CompositeComboBox::arrowVisible() const {
    return m_editor->arrowVisible();
}

void CompositeComboBox::setArrowVisible(bool visible) {
    m_editor->setArrowVisible(visible);
}

int CompositeComboBox::contentPadding() const {
    return m_editor->contentPadding();
}

void CompositeComboBox::setContentPadding(int padding) {
    m_editor->setContentPadding(padding);
    const int inset = m_editor->contentPadding();
    setSlotMargins(QMargins(inset, 0, inset, 0), QMargins(inset, 0, inset, 0));
    updateGeometry();
}

QSize CompositeComboBox::sizeHint() const {
    QSize hint = CompositeInputBase::sizeHint();
    hint.setHeight(qMax(36, hint.height()));
    return hint;
}
