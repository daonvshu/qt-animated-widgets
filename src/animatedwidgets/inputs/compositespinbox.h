#pragma once

#include "animatedwidgets/namespacealias.h"

#include "animatedwidgets/inputs/compositeinputbase.h"

namespace animatedwidgets {

class SpinBox;

class CompositeSpinBox : public CompositeInputBase {
    Q_OBJECT
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(int minimum READ minimum WRITE setMinimum)
    Q_PROPERTY(int maximum READ maximum WRITE setMaximum)
    Q_PROPERTY(int singleStep READ singleStep WRITE setSingleStep)
    Q_PROPERTY(QString prefix READ prefix WRITE setPrefix)
    Q_PROPERTY(QString suffix READ suffix WRITE setSuffix)
    Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly)
    Q_PROPERTY(bool stepButtonsVisible READ stepButtonsVisible WRITE setStepButtonsVisible)
    Q_PROPERTY(bool iconHint READ iconHint WRITE setIconHint)
    Q_PROPERTY(int contentPadding READ contentPadding WRITE setContentPadding)

public:
    explicit CompositeSpinBox(QWidget* parent = nullptr);

    SpinBox* editor() const;

    int value() const;
    void setValue(int value);
    int minimum() const;
    void setMinimum(int minimum);
    int maximum() const;
    void setMaximum(int maximum);
    int singleStep() const;
    void setSingleStep(int step);
    QString prefix() const;
    void setPrefix(const QString& prefix);
    QString suffix() const;
    void setSuffix(const QString& suffix);
    bool isReadOnly() const;
    void setReadOnly(bool readOnly);
    bool stepButtonsVisible() const;
    void setStepButtonsVisible(bool visible);
    bool iconHint() const;
    void setIconHint(bool enabled);
    int contentPadding() const;
    void setContentPadding(int padding);

    QSize sizeHint() const override;

signals:
    void valueChanged(int value);

private:
    SpinBox* m_editor = nullptr;
};

} // namespace animatedwidgets

#ifndef ANIMATEDWIDGETS_DESIGNER_PLUGIN
using CompositeSpinBox = animatedwidgets::CompositeSpinBox;
#endif
