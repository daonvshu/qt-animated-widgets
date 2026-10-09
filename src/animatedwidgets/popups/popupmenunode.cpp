#include "animatedwidgets/popups/popupmenunode.h"
#include "animatedwidgets/popups/popupmenuitem.h"
#include "animatedwidgets/popups/popupmenugroup.h"

namespace animatedwidgets {

PopupMenuNode::PopupMenuNode(const PopupMenuItem& item)
    : m_item(new PopupMenuItem(item))
{
}

PopupMenuNode::PopupMenuNode(const PopupMenuGroup& group)
    : m_group(new PopupMenuGroup(group))
{
}

const PopupMenuItem* PopupMenuNode::item() const {
    return m_item.data();
}

const PopupMenuGroup* PopupMenuNode::group() const {
    return m_group.data();
}

PopupMenuItem PopupMenuNode::toItem() const {
    if (m_item)
        return *m_item;

    PopupMenuItem item;
    if (m_group) {
        item.text = m_group->title;
        item.values = m_group->values;
        item.icon = qvariant_cast<QIcon>(m_group->values.value(Qt::DecorationRole));
        item.shortcut = m_group->values.value(PopupMenuItem::ShortcutRole).toString();
        item.submenuMultiSelection = m_group->multiSelection;
        for (const auto& child : m_group->items)
            item.children.append(child);
        item.children.append(m_group->children);
    }
    return item;
}

} // namespace animatedwidgets
