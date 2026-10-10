#pragma once

class QAbstractScrollArea;

namespace animatedwidgets {

class SmoothScrolling {
public:
    // Installs one area-owned controller when the optional dependency is available.
    static void install(QAbstractScrollArea* area);
};

} // namespace animatedwidgets
