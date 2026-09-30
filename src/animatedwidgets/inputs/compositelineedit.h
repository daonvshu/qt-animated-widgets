#pragma once

#include "animatedwidgets/inputs/compositeinputbase.h"

namespace animatedwidgets {

class IconLineEdit;

class CompositeLineEdit : public CompositeInputBase {
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QString placeholderText READ placeholderText WRITE setPlaceholderText)
    Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly)
    Q_PROPERTY(QColor background READ background WRITE setBackground)
    Q_PROPERTY(int contentPadding READ contentPadding WRITE setContentPadding)

public:
    explicit CompositeLineEdit(QWidget* parent = nullptr);

    IconLineEdit* editor() const;

    QString text() const;
    void setText(const QString& text);
    QString placeholderText() const;
    void setPlaceholderText(const QString& text);
    bool isReadOnly() const;
    void setReadOnly(bool readOnly);

    QColor background() const;
    void setBackground(const QColor& color);

    int contentPadding() const;
    void setContentPadding(int padding);

    QSize sizeHint() const override;

signals:
    void textChanged(const QString& text);

private:
    IconLineEdit* m_editor = nullptr;
};

} // namespace animatedwidgets

#ifndef ANIMATEDWIDGETS_DESIGNER_PLUGIN
using CompositeLineEdit = animatedwidgets::CompositeLineEdit;
#endif
