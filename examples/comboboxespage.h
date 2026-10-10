#pragma once

#include <QWidget>

namespace Ui {
class ComboBoxesPage;
}

class ComboBoxesPage : public QWidget {
public:
    explicit ComboBoxesPage(QWidget* parent = nullptr);

private:
    void setupTreeExamples(const Ui::ComboBoxesPage& ui);
    void setupCreationExamples(const Ui::ComboBoxesPage& ui);
    void setupCascaderExamples(const Ui::ComboBoxesPage& ui);
};
