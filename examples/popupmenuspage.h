#pragma once

#include <QPointer>
#include <QWidget>

#include <memory>

namespace Ui {
class PopupMenusPage;
}

namespace animatedwidgets {
class PopupMenu;
}

class PopupMenusPage : public QWidget {
public:
    explicit PopupMenusPage(QWidget* parent = nullptr);
    ~PopupMenusPage() override;

private:
    void initializeColorEditors();
    void resetSettings();
    void applyDarkColors();
    bool colorsValid() const;
    void showMenu(QWidget* anchor);

    std::unique_ptr<Ui::PopupMenusPage> m_ui;
    QPointer<animatedwidgets::PopupMenu> m_popup;
};
