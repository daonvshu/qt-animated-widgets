#include "animatedwidgets/inputs/compositelineedit.h"

#include "animatedwidgets/inputs/iconlineedit.h"

CompositeLineEdit::CompositeLineEdit(QWidget* parent)
    : CompositeInputBase(parent)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setHorizontalFrameInset(1);
    m_editor = new IconLineEdit(this);
    m_editor->setBorderVisible(false);
    m_editor->setNormalBackgroundColor(Qt::transparent);
    m_editor->setHoverBackgroundColor(Qt::transparent);
    m_editor->setFocusBackgroundColor(Qt::transparent);
    m_editor->setErrorBackgroundColor(Qt::transparent);
    m_editor->setDisabledBackgroundColor(Qt::transparent);
    m_editor->setAnimationDuration(0);
    setEditorWidget(m_editor);
    setContentPadding(m_editor->contentPadding());

    connect(m_editor, &QLineEdit::textChanged, this, &CompositeLineEdit::textChanged);
    connect(m_editor, &LineEdit::hasErrorChanged, this, &CompositeInputBase::setHasError);
    connect(this, &CompositeInputBase::hasErrorChanged, m_editor, &LineEdit::setHasError);
}

IconLineEdit* CompositeLineEdit::editor() const {
    return m_editor;
}

QString CompositeLineEdit::text() const {
    return m_editor->text();
}

void CompositeLineEdit::setText(const QString& text) {
    m_editor->setText(text);
}

QString CompositeLineEdit::placeholderText() const {
    return m_editor->placeholderText();
}

void CompositeLineEdit::setPlaceholderText(const QString& text) {
    m_editor->setPlaceholderText(text);
}

bool CompositeLineEdit::isReadOnly() const {
    return m_editor->isReadOnly();
}

void CompositeLineEdit::setReadOnly(bool readOnly) {
    m_editor->setReadOnly(readOnly);
}

QColor CompositeLineEdit::backgroundColor() const {
    return normalBackgroundColor();
}

void CompositeLineEdit::setBackgroundColor(const QColor& color) {
    setNormalBackgroundColor(color);
}

int CompositeLineEdit::contentPadding() const {
    return m_editor->contentPadding();
}

void CompositeLineEdit::setContentPadding(int padding) {
    m_editor->setContentPadding(padding);
    const int inset = m_editor->contentPadding();
    setSlotMargins(QMargins(inset, 0, inset, 0), QMargins(inset, 0, inset, 0));
    updateGeometry();
}

QSize CompositeLineEdit::sizeHint() const {
    QSize hint = CompositeInputBase::sizeHint();
    hint.setHeight(qMax(36, hint.height()));
    return hint;
}
