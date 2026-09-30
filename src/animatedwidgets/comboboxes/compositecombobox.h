#pragma once

#include "animatedwidgets/inputs/compositeinputbase.h"

namespace animatedwidgets {

class ComboBox;

class CompositeComboBox : public CompositeInputBase {
    Q_OBJECT
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(QString currentText READ currentText WRITE setCurrentText NOTIFY currentTextChanged)
    Q_PROPERTY(bool editable READ isEditable WRITE setEditable)
    Q_PROPERTY(bool arrowVisible READ arrowVisible WRITE setArrowVisible)
    Q_PROPERTY(int contentPadding READ contentPadding WRITE setContentPadding)

public:
    explicit CompositeComboBox(QWidget* parent = nullptr);

    ComboBox* editor() const;

    int currentIndex() const;
    void setCurrentIndex(int index);
    QString currentText() const;
    void setCurrentText(const QString& text);
    bool isEditable() const;
    void setEditable(bool editable);
    bool arrowVisible() const;
    void setArrowVisible(bool visible);
    int contentPadding() const;
    void setContentPadding(int padding);

    QSize sizeHint() const override;

signals:
    void currentIndexChanged(int index);
    void currentTextChanged(const QString& text);

private:
    ComboBox* m_editor = nullptr;
};

} // namespace animatedwidgets

#ifndef ANIMATEDWIDGETS_DESIGNER_PLUGIN
using CompositeComboBox = animatedwidgets::CompositeComboBox;
#endif
