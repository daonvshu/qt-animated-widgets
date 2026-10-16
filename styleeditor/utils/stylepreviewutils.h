#pragma once

#include "animatedwidgets/styleeditor/api/styleproperty.h"

#include <QSize>

class QWidget;

class StylePreviewUtils {
public:
    static void applyState(QWidget* preview, int state, const QList<animatedwidgets::styleeditor::StyleProperty>& properties = {});
    static QSize resolveSize(const QSize& requested, const QSize& suggested, const QSize& hint,
                             bool widthAuto = false, bool heightAuto = false);

private:
    static int resolveDimension(int requested, int suggested, int hint, bool autoSize, int fallback);
};
