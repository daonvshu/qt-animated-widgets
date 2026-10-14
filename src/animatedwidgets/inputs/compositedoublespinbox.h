#pragma once

#include "animatedwidgets/namespacealias.h"

#include "animatedwidgets/inputs/compositeinputbase.h"

namespace animatedwidgets {

class DoubleSpinBox;

class CompositeDoubleSpinBox : public CompositeInputBase {
    Q_OBJECT
    Q_PROPERTY(double value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(double minimum READ minimum WRITE setMinimum)
    Q_PROPERTY(double maximum READ maximum WRITE setMaximum)
    Q_PROPERTY(double singleStep READ singleStep WRITE setSingleStep)
    Q_PROPERTY(int decimals READ decimals WRITE setDecimals)
    Q_PROPERTY(QString prefix READ prefix WRITE setPrefix)
    Q_PROPERTY(QString suffix READ suffix WRITE setSuffix)
    Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly)
    Q_PROPERTY(bool stepButtonsVisible READ stepButtonsVisible WRITE setStepButtonsVisible)
    Q_PROPERTY(int contentPadding READ contentPadding WRITE setContentPadding)

public:
    explicit CompositeDoubleSpinBox(QWidget* parent = nullptr);

    DoubleSpinBox* editor() const;

    double value() const;
    void setValue(double value);
    double minimum() const;
    void setMinimum(double minimum);
    double maximum() const;
    void setMaximum(double maximum);
    double singleStep() const;
    void setSingleStep(double step);
    int decimals() const;
    void setDecimals(int decimals);
    QString prefix() const;
    void setPrefix(const QString& prefix);
    QString suffix() const;
    void setSuffix(const QString& suffix);
    bool isReadOnly() const;
    void setReadOnly(bool readOnly);
    bool stepButtonsVisible() const;
    void setStepButtonsVisible(bool visible);
    int contentPadding() const;
    void setContentPadding(int padding);

    QSize sizeHint() const override;

signals:
    void valueChanged(double value);

private:
    DoubleSpinBox* m_editor = nullptr;
};

} // namespace animatedwidgets

#ifndef ANIMATEDWIDGETS_DESIGNER_PLUGIN
using CompositeDoubleSpinBox = animatedwidgets::CompositeDoubleSpinBox;
#endif
