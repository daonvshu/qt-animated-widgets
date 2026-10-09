#pragma once

#include <QResource>

// Q_INIT_RESOURCE must refer to the generated resource function in the global namespace.
class PopupMenuResources {
public:
    static void initialize() {
        Q_INIT_RESOURCE(popups);
    }
};
