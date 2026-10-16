#include "styleicontreeadapter.h"

#include "styleicontreerow.h"

QWidget* StyleIconTreeAdapter::createWidget(viv::WidgetType, QWidget* parent) {
    return new StyleIconTreeRow(parent);
}

void StyleIconTreeAdapter::bindWidget(QWidget* widget, const QModelIndex& index) {
    static_cast<StyleIconTreeRow*>(widget)->bind(index);
}

void StyleIconTreeAdapter::bindWidget(viv::WidgetType, QWidget* widget, const QModelIndex& index) {
    bindWidget(widget, index);
}

void StyleIconTreeAdapter::unbindWidget(QWidget* widget, const QModelIndex&) {
    static_cast<StyleIconTreeRow*>(widget)->unbind();
}

void StyleIconTreeAdapter::unbindWidget(viv::WidgetType, QWidget* widget, const QModelIndex& index) {
    unbindWidget(widget, index);
}

QSize StyleIconTreeAdapter::estimatedSize(const QModelIndex&) const {
    return QSize(240, 36);
}

QSize StyleIconTreeAdapter::estimatedSize(viv::WidgetType, const QModelIndex& index) const {
    return estimatedSize(index);
}
