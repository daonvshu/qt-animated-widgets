#pragma once

#include <QWidget>

namespace Ui {
class CalendarsPage;
}

class CalendarsPage : public QWidget {
public:
    explicit CalendarsPage(QWidget* parent = nullptr);

private:
    void setupSelectors(const Ui::CalendarsPage& ui);
    void setupPopups(const Ui::CalendarsPage& ui);
};
