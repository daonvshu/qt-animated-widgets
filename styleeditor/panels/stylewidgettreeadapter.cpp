#include "stylewidgettreeadapter.h"

#include "stylewidgettreemodel.h"
#include "stylewidgettreerow.h"

QWidget* StyleWidgetTreeAdapter::createWidget(viv::WidgetType, QWidget* parent) {
    auto* row = new StyleWidgetTreeRow(parent);
    connect(row, &StyleWidgetTreeRow::actionRequested, this, [this](const QPersistentModelIndex& index) {
        if (!index.isValid())
            return;

        if (index.parent().isValid())
            emit editRequested(index.data(StyleWidgetTreeModel::ItemIdRole).toString());
        else
            emit addRequested(index.data(StyleWidgetTreeModel::CategoryRole).toString());
    });
    return row;
}

void StyleWidgetTreeAdapter::bindWidget(QWidget* widget, const QModelIndex& index) {
    static_cast<StyleWidgetTreeRow*>(widget)->bind(index);
}

void StyleWidgetTreeAdapter::bindWidget(viv::WidgetType, QWidget* widget, const QModelIndex& index) {
    bindWidget(widget, index);
}

void StyleWidgetTreeAdapter::unbindWidget(QWidget* widget, const QModelIndex&) {
    static_cast<StyleWidgetTreeRow*>(widget)->unbind();
}

void StyleWidgetTreeAdapter::unbindWidget(viv::WidgetType, QWidget* widget, const QModelIndex& index) {
    unbindWidget(widget, index);
}

QSize StyleWidgetTreeAdapter::estimatedSize(const QModelIndex&) const {
    return QSize(240, 48);
}

QSize StyleWidgetTreeAdapter::estimatedSize(viv::WidgetType, const QModelIndex& index) const {
    return estimatedSize(index);
}
