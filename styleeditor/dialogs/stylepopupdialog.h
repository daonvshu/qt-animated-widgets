#pragma once

#include "animatedwidgets/popups/basepopupdlg.h"

class QKeyEvent;

class StylePopupDialog : public animatedwidgets::BasePopupDlg {
    Q_OBJECT

public:
    explicit StylePopupDialog(QWidget* parent = nullptr);

    void showDialog();

signals:
    void finished(int result);

protected:
    void finish(int result);
    void keyPressEvent(QKeyEvent* event) override;

private:
    int m_result = 0;
    bool m_finishing = false;
};
