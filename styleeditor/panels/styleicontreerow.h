#pragma once

#include <QWidget>

class QLabel;
class QModelIndex;

class StyleIconTreeRow : public QWidget {
public:
    explicit StyleIconTreeRow(QWidget* parent = nullptr);

    void bind(const QModelIndex& index);
    void unbind();

private:
    void createLayout();

    QLabel* m_iconLabel = nullptr;
    QLabel* m_nameLabel = nullptr;
};
