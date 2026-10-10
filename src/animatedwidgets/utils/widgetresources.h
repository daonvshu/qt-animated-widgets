#pragma once

#include <QResource>

// Resource entry points are generated in the global namespace.
class WidgetResources {
public:
    static void initializeInputs() {
        Q_INIT_RESOURCE(inputs);
    }

    static void initializeComboBoxes() {
        Q_INIT_RESOURCE(comboboxes);
    }

    static void initializeCalendars() {
        Q_INIT_RESOURCE(calendars);
    }
};
