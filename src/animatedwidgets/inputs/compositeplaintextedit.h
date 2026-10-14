#pragma once

#include "animatedwidgets/namespacealias.h"

#include "animatedwidgets/inputs/compositeinputbase.h"

namespace animatedwidgets {

class PlainTextEdit;

class CompositePlainTextEdit : public CompositeInputBase {
    Q_OBJECT
    Q_PROPERTY(QString plainText READ plainText WRITE setPlainText NOTIFY textChanged)
    Q_PROPERTY(QString placeholderText READ placeholderText WRITE setPlaceholderText)
    Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly)
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor)
    Q_PROPERTY(QColor disabledTextColor READ disabledTextColor WRITE setDisabledTextColor)
    Q_PROPERTY(int contentPadding READ contentPadding WRITE setContentPadding)

public:
    explicit CompositePlainTextEdit(QWidget* parent = nullptr);

    PlainTextEdit* editor() const;
    QWidget* footerWidget() const;
    void setFooterWidget(QWidget* widget);

    QString plainText() const;
    void setPlainText(const QString& text);
    QString placeholderText() const;
    void setPlaceholderText(const QString& text);
    bool isReadOnly() const;
    void setReadOnly(bool readOnly);

    QColor textColor() const;
    void setTextColor(const QColor& color);
    QColor disabledTextColor() const;
    void setDisabledTextColor(const QColor& color);

    int contentPadding() const;
    void setContentPadding(int padding);

    QSize sizeHint() const override;

signals:
    void textChanged();

private:
    PlainTextEdit* m_editor = nullptr;
};

} // namespace animatedwidgets

#ifndef ANIMATEDWIDGETS_DESIGNER_PLUGIN
using CompositePlainTextEdit = animatedwidgets::CompositePlainTextEdit;
#endif
