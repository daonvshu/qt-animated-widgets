#pragma once

#include "animatedwidgets/popups/basepopupdlg.h"

#include <QPointer>
#include <QStringList>

#include <functional>

class QAbstractItemDelegate;
class QAbstractItemModel;
class QAbstractItemView;

namespace animatedwidgets {

class DefaultPopupMenuStyle;
class DefaultPopupMenuModel;
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

    QAbstractItemDelegate* itemDelegate() const;
    void setItemDelegate(QAbstractItemDelegate* delegate);

    PopupMenuStyle* popupStyle() const;
    void setPopupStyle(PopupMenuStyle* style);
    void refreshStyle();
    void setSurfaceMetrics(int radius, int cornerRadius);
    PopupMenuListStyle* listStyle() const;
    void setListStyle(PopupMenuListStyle* style);

    QAbstractItemModel* model() const;
    void setModel(QAbstractItemModel* model);
    DefaultPopupMenuModel* defaultModel() const;
    void setStrings(const QStringList& strings);

    bool autoClose() const;
    void setAutoClose(bool enabled);

    void setPopupSize(const QSize& size);
    void setAdaptiveHeight(int width);
    void showAt(const QPoint& globalPos);

signals:
    void itemTriggered(const QModelIndex& index);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    void createDefaultView();
    void applyStyle();
    void connectViewSignals();

    QPointer<QAbstractItemView> m_view;
    DefaultPopupMenuModel* m_defaultModel = nullptr;
    QMetaObject::Connection m_modelTriggerConnection;
    QPointer<QAbstractItemDelegate> m_delegate;
    PopupMenuStyle* m_style = nullptr;
    PopupMenuListStyle* m_listStyle = nullptr;
    ViewFactory m_viewFactory;
    bool m_autoClose = true;
};

} // namespace animatedwidgets
