#include "animatedwidgets/popups/groupedpopupmenumodel.h"
#include "animatedwidgets/popups/popupmenurowtype.h"
#include "animatedwidgets/utils/popupmenuitemdata.h"

#include <algorithm>

namespace animatedwidgets {

GroupedPopupMenuModel::GroupedPopupMenuModel(QObject* parent)
    : QAbstractListModel(parent)
{
    qRegisterMetaType<QList<QPair<int, int>>>("QList<QPair<int,int>>");
}

int GroupedPopupMenuModel::rowCount(const QModelIndex& parent) const {
    return parent.isValid() ? 0 : static_cast<int>(m_entries.size());
}

QVariant GroupedPopupMenuModel::data(const QModelIndex& index, int role) const {
    if (!ownsIndex(index))
        return QVariant();

    const Entry& entry = m_entries.at(index.row());
    if (role == PopupMenuItem::GroupIndexRole)
        return entry.groupIndex;
    if (entry.sourceItemIndex < 0)
        return groupData(entry.groupIndex, role);

    if (role == Qt::CheckStateRole) {
        return entry.itemIndex < 0 ? QVariant()
            : QVariant(m_selectedIndexes.contains({entry.groupIndex, entry.itemIndex}) ? Qt::Checked : Qt::Unchecked);
    }
    return PopupMenuItemData::data(m_groups.at(entry.groupIndex).items.at(entry.sourceItemIndex), role);
}

Qt::ItemFlags GroupedPopupMenuModel::flags(const QModelIndex& index) const {
    if (!ownsIndex(index) || m_entries.at(index.row()).itemIndex < 0)
        return Qt::NoItemFlags;
    const Entry& entry = m_entries.at(index.row());
    return m_groups.at(entry.groupIndex).items.at(entry.sourceItemIndex).enabled
        ? Qt::ItemIsEnabled | Qt::ItemIsSelectable : Qt::ItemIsSelectable;
}

bool GroupedPopupMenuModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!ownsIndex(index))
        return false;

    const Entry& entry = m_entries.at(index.row());
    if (entry.sourceItemIndex < 0)
        return setGroupData(entry.groupIndex, value, role);
    if (role != Qt::CheckStateRole) {
        auto& item = m_groups[entry.groupIndex].items[entry.sourceItemIndex];
        if (!PopupMenuItemData::setData(item, value, role))
            return false;
        const QVector<int> roles = role == Qt::DisplayRole || role == Qt::EditRole
            ? QVector<int>{Qt::DisplayRole, Qt::EditRole} : QVector<int>{role};
        emit dataChanged(index, index, roles);
        return true;
    }

    bool valid = false;
    const int state = value.toInt(&valid);
    if (entry.itemIndex < 0 || !valid || (state != Qt::Checked && state != Qt::Unchecked))
        return false;

    const QPair<int, int> itemIndex(entry.groupIndex, entry.itemIndex);
    QList<QPair<int, int>> selected = m_selectedIndexes;
    if (state == Qt::Unchecked)
        selected.removeAll(itemIndex);
    else if (!selected.contains(itemIndex))
        selected.append(itemIndex);
    applySelection(selected);
    return true;
}

QList<PopupMenuGroup> GroupedPopupMenuModel::groups() const {
    return m_groups;
}

QVariant GroupedPopupMenuModel::groupData(int groupIndex, int role) const {
    if (groupIndex < 0 || groupIndex >= m_groups.size())
        return QVariant();

    const PopupMenuGroup& group = m_groups.at(groupIndex);
    if (role == Qt::DisplayRole || role == Qt::EditRole)
        return group.title;
    if (role == PopupMenuItem::TypeRole)
        return static_cast<int>(PopupMenuRowType::GroupTitle);
    if (role == PopupMenuItem::GroupIndexRole)
        return groupIndex;
    if (role == PopupMenuItem::SeparatorRole)
        return false;
    if (role == Qt::CheckStateRole)
        return QVariant();
    return group.values.value(role);
}

bool GroupedPopupMenuModel::setGroupData(int groupIndex, const QVariant& value, int role) {
    if (groupIndex < 0 || groupIndex >= m_groups.size()
        || role == Qt::DisplayRole || role == Qt::EditRole || role == Qt::CheckStateRole
        || role == PopupMenuItem::TypeRole || role == PopupMenuItem::GroupIndexRole
        || role == PopupMenuItem::SeparatorRole)
        return false;

    auto& values = m_groups[groupIndex].values;
    if (value.isValid() ? values.value(role) == value : !values.contains(role))
        return true;
    if (value.isValid())
        values.insert(role, value);
    else
        values.remove(role);

    for (int row = 0; row < m_entries.size(); ++row) {
        const Entry& entry = m_entries.at(row);
        if (entry.groupIndex == groupIndex && entry.sourceItemIndex < 0) {
            emit dataChanged(index(row), index(row), {role});
            break;
        }
    }
    emit groupDataChanged(groupIndex, role);
    return true;
}

void GroupedPopupMenuModel::setGroups(const QList<PopupMenuGroup>& groups) {
    QList<int> clearedSingleGroups;
    for (const auto& itemIndex : m_selectedIndexes)
        if (!m_groups.at(itemIndex.first).multiSelection)
            clearedSingleGroups.append(itemIndex.first);

    const bool hadSelection = !m_selectedIndexes.isEmpty();
    beginResetModel();
    m_groups = groups;
    m_entries.clear();
    for (int groupIndex = 0; groupIndex < m_groups.size(); ++groupIndex) {
        const PopupMenuGroup& group = m_groups.at(groupIndex);
        if (!group.title.isEmpty())
            m_entries.append({groupIndex, -1, -1});

        int itemIndex = 0;
        for (int sourceItem = 0; sourceItem < group.items.size(); ++sourceItem) {
            const bool separator = group.items.at(sourceItem).separator;
            m_entries.append({groupIndex, sourceItem, separator ? -1 : itemIndex++});
        }
    }
    m_selectedIndexes.clear();
    endResetModel();

    for (int groupIndex : clearedSingleGroups)
        emit currentIndexChanged(groupIndex, -1);
    if (hadSelection)
        emit selectedIndexesChanged(m_selectedIndexes);
}

int GroupedPopupMenuModel::currentIndex(int groupIndex) const {
    for (const auto& itemIndex : m_selectedIndexes)
        if (itemIndex.first == groupIndex)
            return itemIndex.second;
    return -1;
}

void GroupedPopupMenuModel::setCurrentIndex(int groupIndex, int itemIndex) {
    if (groupIndex < 0 || groupIndex >= m_groups.size()
        || (itemIndex != -1 && !indexForItem(groupIndex, itemIndex).isValid()))
        return;

    QList<QPair<int, int>> selected = m_selectedIndexes;
    for (int i = static_cast<int>(selected.size()) - 1; i >= 0; --i)
        if (selected.at(i).first == groupIndex)
            selected.removeAt(i);
    if (itemIndex >= 0)
        selected.append({groupIndex, itemIndex});
    applySelection(selected);
}

QList<QPair<int, int>> GroupedPopupMenuModel::selectedIndexes() const {
    return m_selectedIndexes;
}

void GroupedPopupMenuModel::setSelectedIndexes(const QList<QPair<int, int>>& indexes) {
    applySelection(indexes);
}

void GroupedPopupMenuModel::setItemEnabled(int groupIndex, int itemIndex, bool enabled) {
    const QModelIndex item = indexForItem(groupIndex, itemIndex);
    if (!item.isValid())
        return;

    const Entry& entry = m_entries.at(item.row());
    auto& data = m_groups[groupIndex].items[entry.sourceItemIndex];
    if (data.enabled == enabled)
        return;
    data.enabled = enabled;
    emit dataChanged(item, item, {});
}

QPair<int, int> GroupedPopupMenuModel::logicalIndex(const QModelIndex& index) const {
    if (!ownsIndex(index) || m_entries.at(index.row()).itemIndex < 0)
        return {-1, -1};
    const Entry& entry = m_entries.at(index.row());
    return {entry.groupIndex, entry.itemIndex};
}

QModelIndex GroupedPopupMenuModel::indexForItem(int groupIndex, int itemIndex) const {
    if (groupIndex < 0 || groupIndex >= m_groups.size() || itemIndex < 0)
        return QModelIndex();
    for (int row = 0; row < m_entries.size(); ++row) {
        const Entry& entry = m_entries.at(row);
        if (entry.groupIndex == groupIndex && entry.itemIndex == itemIndex)
            return index(row);
    }
    return QModelIndex();
}

void GroupedPopupMenuModel::triggerItem(const QModelIndex& index) {
    if (!ownsIndex(index) || !flags(index).testFlag(Qt::ItemIsEnabled))
        return;

    const auto itemIndex = logicalIndex(index);
    const bool toggleOff = m_groups.at(itemIndex.first).multiSelection && m_selectedIndexes.contains(itemIndex);
    setData(index, toggleOff ? Qt::Unchecked : Qt::Checked, Qt::CheckStateRole);
}

bool GroupedPopupMenuModel::ownsIndex(const QModelIndex& index) const {
    return index.isValid() && index.model() == this && index.column() == 0
        && !index.parent().isValid() && index.row() >= 0 && index.row() < rowCount();
}

void GroupedPopupMenuModel::applySelection(const QList<QPair<int, int>>& indexes) {
    QList<QPair<int, int>> selected;
    for (const auto& itemIndex : indexes) {
        if (!indexForItem(itemIndex.first, itemIndex.second).isValid())
            continue;
        if (!m_groups.at(itemIndex.first).multiSelection) {
            for (int i = static_cast<int>(selected.size()) - 1; i >= 0; --i)
                if (selected.at(i).first == itemIndex.first)
                    selected.removeAt(i);
        }
        if (!selected.contains(itemIndex))
            selected.append(itemIndex);
    }
    std::sort(selected.begin(), selected.end());
    if (selected == m_selectedIndexes)
        return;

    const QList<QPair<int, int>> previous = m_selectedIndexes;
    m_selectedIndexes = selected;
    if (!m_entries.isEmpty())
        emit dataChanged(index(0), index(rowCount() - 1), {Qt::CheckStateRole});

    for (int groupIndex = 0; groupIndex < m_groups.size(); ++groupIndex) {
        if (m_groups.at(groupIndex).multiSelection)
            continue;
        int previousItem = -1;
        for (const auto& itemIndex : previous)
            if (itemIndex.first == groupIndex)
                previousItem = itemIndex.second;
        const int currentItem = currentIndex(groupIndex);
        if (previousItem != currentItem)
            emit currentIndexChanged(groupIndex, currentItem);
    }
    emit selectedIndexesChanged(m_selectedIndexes);
}

} // namespace animatedwidgets
