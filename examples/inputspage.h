#pragma once

#include <QWidget>

namespace Ui {
class InputsPage;
}

class InputsPage : public QWidget {
public:
    explicit InputsPage(QWidget* parent = nullptr);

private:
    void initializeBasicInputs(Ui::InputsPage& ui);
    void initializeCompositeLineEdit(Ui::InputsPage& ui);
    void initializeCompositePlainTextEdit(Ui::InputsPage& ui);
    void initializeNumericInputs(Ui::InputsPage& ui);
};
