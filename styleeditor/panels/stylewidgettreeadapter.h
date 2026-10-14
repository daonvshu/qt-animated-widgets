#pragma once

#include <virtualitemviews/widgetadapter.h>

#include <QObject>

class StyleWidgetTreeAdapter : public QObject, public viv::WidgetAdapter {
    Q_OBJECT

public:
    QWidget* createWidget(viv::WidgetType type, QWidget* parent) override;

    // Support package versions both with and without the recycler type argument.
    void bindWidget(QWidget* widget, const QModelIndex& index);
    void bindWidget(viv::WidgetType type, QWidget* widget, const QModelIndex& index);
    void unbindWidget(QWidget* widget, const QModelIndex& index);
    void unbindWidget(viv::WidgetType type, QWidget* widget, const QModelIndex& index);
    QSize estimatedSize(const QModelIndex& index) const;
    QSize estimatedSize(viv::WidgetType type, const QModelIndex& index) const;

signals:
    void addRequested(const QString& category);
    void editRequested(const QString& itemId);
};
