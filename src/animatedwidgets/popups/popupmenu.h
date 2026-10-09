#pragma once

#include "animatedwidgets/popups/basepopupdlg.h"
#include "animatedwidgets/popups/popupmenuitem.h"
#include "animatedwidgets/popups/popupmenugroup.h"

#include <QPointer>
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

    QString scrollBarStyleSheet() const;
    void setScrollBarStyleSheet(const QString& styleSheet);

    QAbstractItemModel* model() const;
    virtual void setModel(QAbstractItemModel* model);
    DefaultPopupMenuModel* defaultModel() const;
    GroupedPopupMenuModel* groupedModel() const;
    void setStrings(const QStringList& strings);
    void setItems(const QList<PopupMenuItem>& items);
    void setGroups(const QList<PopupMenuGroup>& groups);

    bool autoClose() const;
    void setAutoClose(bool enabled);

    virtual void setPopupSize(const QSize& size);
    virtual void setAdaptiveHeight(int width, int maximumVisibleItems = 0);
    void showAt(const QPoint& globalPos);

signals:
    void itemTriggered(const QModelIndex& index);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
    virtual bool includeListInAdaptiveHeight() const;

private:
    void createDefaultView();
    void applyStyle();
    void connectViewSignals();
    void setSlotWidget(QPointer<QWidget>& slot, QWidget* widget, bool top);

    QPointer<QAbstractItemView> m_view;
    QPointer<QWidget> m_topWidget;
    QPointer<QWidget> m_bottomWidget;
    DefaultPopupMenuModel* m_defaultModel = nullptr;
    GroupedPopupMenuModel* m_groupedModel = nullptr;
    QMetaObject::Connection m_modelTriggerConnection;
    QPointer<QAbstractItemDelegate> m_delegate;
    PopupMenuStyle* m_style = nullptr;
    PopupMenuListStyle* m_listStyle = nullptr;
    ViewFactory m_viewFactory;
    QString m_scrollBarStyleSheet;
    bool m_autoClose = true;
};

} // namespace animatedwidgets
