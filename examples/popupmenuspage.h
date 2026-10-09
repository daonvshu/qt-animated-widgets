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
    void showGroupedMenu(QWidget* anchor);
    void showModulesMenu(QWidget* anchor);
    void showSearchMenu(QWidget* anchor);
    void showIconMenu(QWidget* anchor);
    void showVariant(animatedwidgets::PopupMenu* popup, QWidget* anchor);

    std::unique_ptr<Ui::PopupMenusPage> m_ui;
    QPointer<animatedwidgets::PopupMenu> m_popup;
};
