#pragma once

#include <QObject>
#include <QPointer>

class QMainWindow;
class QWidget;
class StyleThemeScope;

namespace animatedwidgets {
class PopupMenu;
class ShadowWidget;
}

class StyleThemeManager : public QObject {
    Q_OBJECT

public:
    explicit StyleThemeManager(QMainWindow* window, StyleThemeScope* scope);

    QString theme() const;
    bool setTheme(const QString& theme);

    static void prepareWidget(QWidget* widget);
    static void preparePopup(animatedwidgets::PopupMenu* popup);

signals:
    void themeChanged(const QString& theme);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    static StyleThemeManager* forWidget(QWidget* widget);

    void prepareSubtree(QWidget* widget);
    void applySubtreeColors(QWidget* widget);
    void applyWidgetColors(QWidget* widget);
    void applyShadowStyle(animatedwidgets::ShadowWidget* popup);
    void applyPopupColors(animatedwidgets::PopupMenu* popup);

    QPointer<QMainWindow> m_window;
    QPointer<StyleThemeScope> m_scope;
    QString m_theme;
    QString m_scrollBarStyleSheet;
};
