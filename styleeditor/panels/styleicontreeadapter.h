#pragma once

#include <virtualitemviews/widgetadapter.h>

class StyleIconTreeAdapter : public viv::WidgetAdapter {
public:
    QWidget* createWidget(viv::WidgetType type, QWidget* parent) override;

    // Support package versions both with and without the recycler type argument.
    void bindWidget(QWidget* widget, const QModelIndex& index);
    void bindWidget(viv::WidgetType type, QWidget* widget, const QModelIndex& index);
    void unbindWidget(QWidget* widget, const QModelIndex& index);
    void unbindWidget(viv::WidgetType type, QWidget* widget, const QModelIndex& index);
    QSize estimatedSize(const QModelIndex& index) const;
    QSize estimatedSize(viv::WidgetType type, const QModelIndex& index) const;
};
