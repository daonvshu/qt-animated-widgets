#pragma once

#include "ui_stylewidgettreerow.h"

#include <QPersistentModelIndex>
#include <QWidget>

class StyleWidgetTreeRow : public QWidget {
    Q_OBJECT

public:
    explicit StyleWidgetTreeRow(QWidget* parent = nullptr);

    void bind(const QModelIndex& index);
    void unbind();

signals:
    void actionRequested(QPersistentModelIndex index);

private slots:
    void on_actionButton_clicked();

private:
    void createLayout();

    Ui::StyleWidgetTreeRow ui;

    QPersistentModelIndex m_index;
};
