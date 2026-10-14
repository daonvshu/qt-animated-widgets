#pragma once

#include "animatedwidgets/styleeditor/api/styleproperty.h"
#include "animatedwidgets/buttons/borderedbutton.h"
#include "ui_stylepropertyrow.h"

class StylePropertyRow : public QWidget {
    Q_OBJECT

public:
    explicit StylePropertyRow(const animatedwidgets::styleeditor::StyleProperty& property, QWidget* parent = nullptr);
    void setColor(const QColor& color);

signals:
    void valueEdited(const QVariant& value);
    void colorRequested(const QColor& color);
    void resetRequested();

private slots:
    void on_resetButton_clicked();

private:
    void createLayout();
    void refreshColor();

    Ui::StylePropertyRow ui;

    animatedwidgets::styleeditor::StyleProperty m_property;
    animatedwidgets::BorderedButton* m_colorButton = nullptr;
};
