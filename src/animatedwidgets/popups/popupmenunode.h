#pragma once

#include <QList>
#include <QMetaType>
#include <QSharedPointer>

namespace animatedwidgets {

class PopupMenuItem;
class PopupMenuGroup;

class PopupMenuNode {
public:
    PopupMenuNode() = default;
    PopupMenuNode(const PopupMenuItem& item);
    PopupMenuNode(const PopupMenuGroup& group);

    const PopupMenuItem* item() const;
    const PopupMenuGroup* group() const;
    PopupMenuItem toItem() const;

private:
    QSharedPointer<const PopupMenuItem> m_item;
    QSharedPointer<const PopupMenuGroup> m_group;
};

} // namespace animatedwidgets

Q_DECLARE_METATYPE(QList<animatedwidgets::PopupMenuNode>)
