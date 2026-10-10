#pragma once

#include "animatedwidgets/popups/popupmenuitemdelegate.h"

namespace animatedwidgets {

class CascaderItemDelegate : public PopupMenuItemDelegate {
    Q_OBJECT

public:
    explicit CascaderItemDelegate(QObject* parent = nullptr);

    void setPathEndIndex(const QModelIndex& index);

protected:
    bool hasChildItems(const QModelIndex& index) const override;
    QVariant itemCheckState(const QModelIndex& index) const override;
    bool isItemSelected(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    QPersistentModelIndex m_pathEndIndex;
};

} // namespace animatedwidgets
