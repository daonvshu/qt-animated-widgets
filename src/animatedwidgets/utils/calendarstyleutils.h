#pragma once

class QWidget;

namespace animatedwidgets {

class CalendarStyle;

class CalendarStyleUtils {
public:
    static void applyChildren(QWidget* widget, const CalendarStyle& style);
};

} // namespace animatedwidgets
