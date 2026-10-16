#pragma once

#include "animatedwidgets/styleeditor/api/styleproperty.h"

class QWidget;

class StylePropertyEditorUtils {
public:
    using ValueChanged = std::function<void(const QVariant&)>;

    static QWidget* createEditor(const animatedwidgets::styleeditor::StyleProperty& property, QWidget* parent, const ValueChanged& changed);

private:
    static QWidget* createAlignmentEditor(const animatedwidgets::styleeditor::StyleProperty& property, QWidget* parent, const ValueChanged& changed);
    static QWidget* createSizeEditor(const animatedwidgets::styleeditor::StyleProperty& property, QWidget* parent, const ValueChanged& changed);
};
