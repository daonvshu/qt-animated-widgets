#include "animatedwidgets/utils/popupmenuitemdata.h"
#include "animatedwidgets/popups/popupmenurowtype.h"

namespace animatedwidgets {

QVariant PopupMenuItemData::data(const PopupMenuItem& item, int role) {
    if (role == Qt::DisplayRole || role == Qt::EditRole)
        return item.text;
    if (role == Qt::DecorationRole)
        return item.icon;
    if (role == PopupMenuItem::TypeRole)
        return item.separator ? static_cast<int>(PopupMenuRowType::Separator) : static_cast<int>(PopupMenuRowType::Option);
    if (role == PopupMenuItem::SeparatorRole)
        return item.separator;
    if (role == PopupMenuItem::ShortcutRole)
        return item.shortcut;
    return item.values.value(role);
}

bool PopupMenuItemData::setData(PopupMenuItem& item, const QVariant& value, int role) {
    if (role == PopupMenuItem::TypeRole || role == PopupMenuItem::SeparatorRole
        || role == PopupMenuItem::GroupIndexRole || role == Qt::CheckStateRole)
        return false;

    if (role == Qt::DisplayRole || role == Qt::EditRole)
        item.text = value.toString();
    else if (role == Qt::DecorationRole)
        item.icon = qvariant_cast<QIcon>(value);
    else if (role == PopupMenuItem::ShortcutRole)
        item.shortcut = value.toString();
    else if (value.isValid())
        item.values.insert(role, value);
    else
        item.values.remove(role);
    return true;
}

} // namespace animatedwidgets
