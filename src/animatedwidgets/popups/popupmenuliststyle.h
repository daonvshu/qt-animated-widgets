#pragma once

#include <QMargins>

class QAbstractItemView;

namespace animatedwidgets {

class PopupMenuListStyle {
public:
    virtual ~PopupMenuListStyle() = default;

    virtual void apply(QAbstractItemView* view) const = 0;
    virtual QMargins margins() const = 0;
};

} // namespace animatedwidgets
