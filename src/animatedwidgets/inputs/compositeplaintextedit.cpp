#include "animatedwidgets/inputs/compositeplaintextedit.h"

#include "animatedwidgets/inputs/plaintextedit.h"

CompositePlainTextEdit::CompositePlainTextEdit(QWidget* parent)
    : CompositeInputBase(parent)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    m_editor = new PlainTextEdit(this);
    m_editor->setBorderVisible(false);
    m_editor->setNormalBackgroundColor(Qt::transparent);
    m_editor->setHoverBackgroundColor(Qt::transparent);
    m_editor->setFocusBackgroundColor(Qt::transparent);
    m_editor->setErrorBackgroundColor(Qt::transparent);
    m_editor->setDisabledBackgroundColor(Qt::transparent);
    m_editor->setAnimationDuration(0);
    setEditorWidget(m_editor);
    setContentPadding(m_editor->contentPadding());

    connect(m_editor, &QPlainTextEdit::textChanged, this, &CompositePlainTextEdit::textChanged);
    connect(m_editor, &PlainTextEdit::hasErrorChanged, this, &CompositeInputBase::setHasError);
    connect(this, &CompositeInputBase::hasErrorChanged, m_editor, &PlainTextEdit::setHasError);
}

PlainTextEdit* CompositePlainTextEdit::editor() const {
    return m_editor;
}

QWidget* CompositePlainTextEdit::footerWidget() const {
    return bottomWidget();
}

void CompositePlainTextEdit::setFooterWidget(QWidget* widget) {
    setBottomWidget(widget);
}

QString CompositePlainTextEdit::plainText() const {
    return m_editor->toPlainText();
}

void CompositePlainTextEdit::setPlainText(const QString& text) {
    m_editor->setPlainText(text);
}

QString CompositePlainTextEdit::placeholderText() const {
    return m_editor->placeholderText();
}

void CompositePlainTextEdit::setPlaceholderText(const QString& text) {
    m_editor->setPlaceholderText(text);
}

bool CompositePlainTextEdit::isReadOnly() const {
    return m_editor->isReadOnly();
}

void CompositePlainTextEdit::setReadOnly(bool readOnly) {
    m_editor->setReadOnly(readOnly);
}

QColor CompositePlainTextEdit::textColor() const {
    return m_editor->textColor();
}

void CompositePlainTextEdit::setTextColor(const QColor& color) {
    m_editor->setTextColor(color);
}

QColor CompositePlainTextEdit::disabledTextColor() const {
    return m_editor->disabledTextColor();
}

void CompositePlainTextEdit::setDisabledTextColor(const QColor& color) {
    m_editor->setDisabledTextColor(color);
}

int CompositePlainTextEdit::contentPadding() const {
    return m_editor->contentPadding();
}

void CompositePlainTextEdit::setContentPadding(int padding) {
    m_editor->setContentPadding(padding);
    const int inset = m_editor->contentPadding();
    setSlotMargins(QMargins(inset, qMax(4, inset / 2), inset, 0), QMargins(inset, 0, inset, qMax(4, inset / 2)));
    updateGeometry();
}

QSize CompositePlainTextEdit::sizeHint() const {
    QSize hint = CompositeInputBase::sizeHint();
    hint.setHeight(qMax(hint.height(), 110));
    return hint;
}
