#pragma once

#include "animatedwidgets/buttons/variantbuttons.h"
#include "animatedwidgets/comboboxes/combobox.h"
#include "animatedwidgets/comboboxes/compositecombobox.h"
#include "animatedwidgets/comboboxes/searchcombobox.h"
#include "animatedwidgets/comboboxes/treecombobox.h"
#include "animatedwidgets/comboboxes/creatablecombobox.h"
#include "animatedwidgets/comboboxes/multiselectcombobox.h"
#include "animatedwidgets/tags/tagwidget.h"
#include "animatedwidgets/inputs/compositedoublespinbox.h"
#include "animatedwidgets/inputs/compositelineedit.h"
#include "animatedwidgets/inputs/compositeplaintextedit.h"
#include "animatedwidgets/inputs/compositespinbox.h"
#include "animatedwidgets/inputs/doublespinbox.h"
#include "animatedwidgets/inputs/iconlineedit.h"
#include "animatedwidgets/inputs/lineedit.h"
#include "animatedwidgets/inputs/passwordlineedit.h"
#include "animatedwidgets/inputs/plaintextedit.h"
#include "animatedwidgets/inputs/spinbox.h"
#include "animatedwidgets/progressbars/progressbar.h"
#include "animatedwidgets/sliders/slider.h"
#include "animatedwidgets/switches/switch.h"

// Designer requires the plugin name and the created widget's meta-object name to match.
class FilledButton : public animatedwidgets::FilledButton {
    Q_OBJECT
public:
    using animatedwidgets::FilledButton::FilledButton;
};
class OutlineButton : public animatedwidgets::OutlineButton {
    Q_OBJECT
public:
    using animatedwidgets::OutlineButton::OutlineButton;
};
class BorderedButton : public animatedwidgets::BorderedButton {
    Q_OBJECT
public:
    using animatedwidgets::BorderedButton::BorderedButton;
};
class HoverButton : public animatedwidgets::HoverButton {
    Q_OBJECT
public:
    using animatedwidgets::HoverButton::HoverButton;
};
class FilledIconButton : public animatedwidgets::FilledIconButton {
    Q_OBJECT
public:
    using animatedwidgets::FilledIconButton::FilledIconButton;
};
class OutlineIconButton : public animatedwidgets::OutlineIconButton {
    Q_OBJECT
public:
    using animatedwidgets::OutlineIconButton::OutlineIconButton;
};
class BorderedIconButton : public animatedwidgets::BorderedIconButton {
    Q_OBJECT
public:
    using animatedwidgets::BorderedIconButton::BorderedIconButton;
};
class HoverIconButton : public animatedwidgets::HoverIconButton {
    Q_OBJECT
public:
    using animatedwidgets::HoverIconButton::HoverIconButton;
};
class RadioButton : public animatedwidgets::RadioButton {
    Q_OBJECT
public:
    using animatedwidgets::RadioButton::RadioButton;
};
class CheckBox : public animatedwidgets::CheckBox {
    Q_OBJECT
public:
    using animatedwidgets::CheckBox::CheckBox;
};
class Switch : public animatedwidgets::Switch {
    Q_OBJECT
public:
    using animatedwidgets::Switch::Switch;
};
class ProgressBar : public animatedwidgets::ProgressBar {
    Q_OBJECT
public:
    using animatedwidgets::ProgressBar::ProgressBar;
};
class Slider : public animatedwidgets::Slider {
    Q_OBJECT
public:
    using animatedwidgets::Slider::Slider;
};
class LineEdit : public animatedwidgets::LineEdit {
    Q_OBJECT
public:
    using animatedwidgets::LineEdit::LineEdit;
};
class IconLineEdit : public animatedwidgets::IconLineEdit {
    Q_OBJECT
public:
    using animatedwidgets::IconLineEdit::IconLineEdit;
};
class PasswordLineEdit : public animatedwidgets::PasswordLineEdit {
    Q_OBJECT
public:
    using animatedwidgets::PasswordLineEdit::PasswordLineEdit;
};
class CompositeLineEdit : public animatedwidgets::CompositeLineEdit {
    Q_OBJECT
public:
    using animatedwidgets::CompositeLineEdit::CompositeLineEdit;
};
class PlainTextEdit : public animatedwidgets::PlainTextEdit {
    Q_OBJECT
public:
    using animatedwidgets::PlainTextEdit::PlainTextEdit;
};
class CompositePlainTextEdit : public animatedwidgets::CompositePlainTextEdit {
    Q_OBJECT
public:
    using animatedwidgets::CompositePlainTextEdit::CompositePlainTextEdit;
};
class SpinBox : public animatedwidgets::SpinBox {
    Q_OBJECT
public:
    using animatedwidgets::SpinBox::SpinBox;
};
class DoubleSpinBox : public animatedwidgets::DoubleSpinBox {
    Q_OBJECT
public:
    using animatedwidgets::DoubleSpinBox::DoubleSpinBox;
};
class CompositeSpinBox : public animatedwidgets::CompositeSpinBox {
    Q_OBJECT
public:
    using animatedwidgets::CompositeSpinBox::CompositeSpinBox;
};
class CompositeDoubleSpinBox : public animatedwidgets::CompositeDoubleSpinBox {
    Q_OBJECT
public:
    using animatedwidgets::CompositeDoubleSpinBox::CompositeDoubleSpinBox;
};
class ComboBox : public animatedwidgets::ComboBox {
    Q_OBJECT
public:
    using animatedwidgets::ComboBox::ComboBox;
};
class CompositeComboBox : public animatedwidgets::CompositeComboBox {
    Q_OBJECT
public:
    using animatedwidgets::CompositeComboBox::CompositeComboBox;
};

class SearchComboBox : public animatedwidgets::SearchComboBox {
    Q_OBJECT
public:
    using animatedwidgets::SearchComboBox::SearchComboBox;
};

class MultiSelectComboBox : public animatedwidgets::MultiSelectComboBox {
    Q_OBJECT
public:
    using animatedwidgets::MultiSelectComboBox::MultiSelectComboBox;
};

class TreeComboBox : public animatedwidgets::TreeComboBox {
    Q_OBJECT
public:
    using animatedwidgets::TreeComboBox::TreeComboBox;
};

class CreatableComboBox : public animatedwidgets::CreatableComboBox {
    Q_OBJECT
public:
    using animatedwidgets::CreatableComboBox::CreatableComboBox;
};

class TagWidget : public animatedwidgets::TagWidget {
    Q_OBJECT
public:
    using animatedwidgets::TagWidget::TagWidget;
};
