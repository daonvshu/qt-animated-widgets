#pragma once

#include "animatedwidgets/popups/basepopupdlg.h"
#include "animatedwidgets/popups/popupmenuitem.h"
#include "animatedwidgets/popups/popupmenugroup.h"

#include <QPointer>
#include <QHash>
#include <QPersistentModelIndex>
#include <QStringList>

#include <functional>

class QAbstractItemDelegate;
class QAbstractItemModel;
class QAbstractItemView;

namespace animatedwidgets {

class DefaultPopupMenuStyle;
class DefaultPopupMenuModel;
class GroupedPopupMenuModel;
class PopupMenuItemDelegate;
class PopupMenuStyle;
class PopupMenuListStyle;

class PopupMenu : public BasePopupDlg {
    Q_OBJECT
    Q_PROPERTY(bool iconHint READ iconHint WRITE setIconHint)

public:
    using ViewFactory = std::function<QAbstractItemView*(QWidget*)>;

    explicit PopupMenu(QWidget* parent = nullptr);
    ~PopupMenu() override;

    QAbstractItemView* view() const;
    void setViewFactory(ViewFactory factory);
    void setView(QAbstractItemView* view);

    QWidget* topWidget() const;
    // Takes ownership; replacing or clearing the slot deletes its previous widget.
    void setTopWidget(QWidget* widget);
    QWidget* bottomWidget() const;
    // Takes ownership; replacing or clearing the slot deletes its previous widget.
    void setBottomWidget(QWidget* widget);

    QAbstractItemDelegate* itemDelegate() const;
    void setItemDelegate(QAbstractItemDelegate* delegate);

    PopupMenuStyle* popupStyle() const;
    void setPopupStyle(PopupMenuStyle* style);
    void refreshStyle();
    void setSurfaceMetrics(int radius, int cornerRadius);
    PopupMenuListStyle* listStyle() const;
    void setListStyle(PopupMenuListStyle* style);

    bool iconHint() const;
    virtual void setIconHint(bool enabled);

    QString scrollBarStyleSheet() const;
    void setScrollBarStyleSheet(const QString& styleSheet);

    QAbstractItemModel* model() const;
    virtual void setModel(QAbstractItemModel* model);
    DefaultPopupMenuModel* defaultModel() const;
    GroupedPopupMenuModel* groupedModel() const;
    void setStrings(const QStringList& strings);
    void setItems(const QList<PopupMenuItem>& items);
    void setGroups(const QList<PopupMenuGroup>& groups);
    void setChildren(const QList<PopupMenuNode>& children);

    PopupMenu* activeSubmenu() const;

    bool autoClose() const;
    void setAutoClose(bool enabled);

    virtual void setPopupSize(const QSize& size);
    virtual void setAdaptiveHeight(int width, int maximumVisibleItems = 0);
    void showAt(const QPoint& globalPos);

signals:
    void itemTriggered(const QModelIndex& index);
    // Path components exclude separators and headings; index belongs to the leaf's menu model.
    void itemPathTriggered(const QList<int>& path, const QModelIndex& index);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
    void hideEvent(QHideEvent* event) override;
    virtual bool includeListInAdaptiveHeight() const;

private:
    void createDefaultView();
    void applyStyle();
    void connectViewSignals();
    void connectCurrentIndexSignal();
    void setSlotWidget(QPointer<QWidget>& slot, QWidget* widget, bool top);
    void activateItem(const QModelIndex& index);
    void openSubmenu(const QModelIndex& index, bool keyboard = false);
    void hideSubmenus();
    void clearSubmenus();
    void updateHoveredItem(const QPoint& globalPos);
    void setHoveredIndex(const QModelIndex& index);
    bool filterMenuChainEvent(QObject* watched, QEvent* event);
    PopupMenu* rootMenu();
    PopupMenu* menuAt(const QPoint& globalPos);
    QRect submenuTransitionRect() const;
    QList<int> itemPath(const QModelIndex& index) const;

    QPointer<QAbstractItemView> m_view;
    QPointer<QWidget> m_topWidget;
    QPointer<QWidget> m_bottomWidget;
    DefaultPopupMenuModel* m_defaultModel = nullptr;
    GroupedPopupMenuModel* m_groupedModel = nullptr;
    QMetaObject::Connection m_modelTriggerConnection;
    QList<QMetaObject::Connection> m_submenuModelConnections;
    QList<QMetaObject::Connection> m_viewConnections;
    QMetaObject::Connection m_currentIndexConnection;
    QPointer<QAbstractItemDelegate> m_delegate;
    PopupMenuStyle* m_style = nullptr;
    PopupMenuListStyle* m_listStyle = nullptr;
    ViewFactory m_viewFactory;
    QString m_scrollBarStyleSheet;
    bool m_autoClose = true;

    QPointer<PopupMenu> m_parentMenu;
    QPointer<PopupMenu> m_activeSubmenu;
    QPersistentModelIndex m_submenuIndex;
    QPersistentModelIndex m_hoveredIndex;
    QHash<QPersistentModelIndex, QPointer<PopupMenu>> m_submenus;

    QPointer<PopupMenu> m_pressedMenu;
    QPersistentModelIndex m_pressedIndex;
    bool m_submenuMousePress = false;
};

} // namespace animatedwidgets
