#include "animatedwidgets/popups/defaultpopupmenumodel.h"
#include "animatedwidgets/utils/popupmenuitemdata.h"

#include <algorithm>

namespace animatedwidgets {

DefaultPopupMenuModel::DefaultPopupMenuModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

int DefaultPopupMenuModel::rowCount(const QModelIndex& parent) const {
    return parent.isValid() ? 0 : static_cast<int>(m_items.size());
}

QVariant DefaultPopupMenuModel::data(const QModelIndex& index, int role) const {
    if (!ownsIndex(index))
        return QVariant();

    if (role == Qt::CheckStateRole) {
        const int itemIndex = logicalIndex(index);
        return itemIndex < 0 ? QVariant()
            : QVariant(m_selectedIndexes.contains(itemIndex) ? Qt::Checked : Qt::Unchecked);
    }
    return PopupMenuItemData::data(m_items.at(index.row()), role);
}

Qt::ItemFlags DefaultPopupMenuModel::flags(const QModelIndex& index) const {
    if (!ownsIndex(index) || m_items.at(index.row()).separator)
        return Qt::NoItemFlags;
    return m_items.at(index.row()).enabled ? Qt::ItemIsEnabled | Qt::ItemIsSelectable : Qt::ItemIsSelectable;
}

bool DefaultPopupMenuModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!ownsIndex(index))
        return false;

    if (role != Qt::CheckStateRole) {
        if (!PopupMenuItemData::setData(m_items[index.row()], value, role))
            return false;
        const QVector<int> roles = role == Qt::DisplayRole || role == Qt::EditRole
            ? QVector<int>{Qt::DisplayRole, Qt::EditRole} : QVector<int>{role};
        emit dataChanged(index, index, roles);
        return true;
    }

    const int itemIndex = logicalIndex(index);
    bool valid = false;
    const int state = value.toInt(&valid);
    if (itemIndex < 0 || !valid || (state != Qt::Checked && state != Qt::Unchecked))
        return false;

    QList<int> selected = m_selectedIndexes;
    if (state == Qt::Unchecked)
        selected.removeAll(itemIndex);
    else if (m_selectionMode == SingleSelection)
        selected = {itemIndex};
    else if (!selected.contains(itemIndex))
        selected.append(itemIndex);
    applySelection(selected);
    return true;
}

QStringList DefaultPopupMenuModel::strings() const {
    QStringList result;
    for (int row : m_itemRows)
        result.append(m_items.at(row).text);
    return result;
}

void DefaultPopupMenuModel::setStrings(const QStringList& strings) {
    bool unchanged = m_items.size() == strings.size();
    if (unchanged) {
        for (int row = 0; row < rowCount(); ++row) {
            const auto& item = m_items.at(row);
            if (item.separator || !item.icon.isNull() || !item.shortcut.isEmpty()
                || !item.values.isEmpty() || item.text != strings.at(row)) {
                unchanged = false;
                break;
            }
        }
    }
    if (unchanged)
        return;

    QList<PopupMenuItem> items;
    for (const auto& text : strings) {
        PopupMenuItem item;
        item.text = text;
        items.append(item);
    }
    setItems(items);
}

QList<PopupMenuItem> DefaultPopupMenuModel::items() const {
    return m_items;
}

void DefaultPopupMenuModel::setItems(const QList<PopupMenuItem>& items) {
    const bool hadSelection = !m_selectedIndexes.isEmpty();
    beginResetModel();
    m_items = items;
    m_rowItems.clear();
    m_itemRows.clear();
    for (int row = 0; row < rowCount(); ++row) {
        m_rowItems.append(m_items.at(row).separator ? -1 : static_cast<int>(m_itemRows.size()));
        if (!m_items.at(row).separator)
            m_itemRows.append(row);
    }
    m_selectedIndexes.clear();
    endResetModel();
    if (hadSelection)
        notifySelection();
}

DefaultPopupMenuModel::SelectionMode DefaultPopupMenuModel::selectionMode() const {
    return m_selectionMode;
}

void DefaultPopupMenuModel::setSelectionMode(SelectionMode mode) {
    if (mode == m_selectionMode || (mode != SingleSelection && mode != MultiSelection))
        return;

    m_selectionMode = mode;
    if (mode == SingleSelection && m_selectedIndexes.size() > 1)
        applySelection({m_selectedIndexes.first()});
    else
        notifySelection();
}

int DefaultPopupMenuModel::currentIndex() const {
    return m_selectedIndexes.isEmpty() ? -1 : m_selectedIndexes.first();
}

void DefaultPopupMenuModel::setCurrentIndex(int itemIndex) {
    if (itemIndex == -1)
        applySelection({});
    else if (rowForLogicalIndex(itemIndex) >= 0)
        applySelection({itemIndex});
}

QList<int> DefaultPopupMenuModel::selectedIndexes() const {
    return m_selectedIndexes;
}

void DefaultPopupMenuModel::setSelectedIndexes(const QList<int>& indexes) {
    applySelection(indexes);
}

void DefaultPopupMenuModel::setItemEnabled(int itemIndex, bool enabled) {
    const int row = rowForLogicalIndex(itemIndex);
    if (row < 0 || enabled == m_items.at(row).enabled)
        return;

    m_items[row].enabled = enabled;
    emit dataChanged(index(row), index(row), {});
}

void DefaultPopupMenuModel::triggerItem(const QModelIndex& index) {
    if (!ownsIndex(index) || !flags(index).testFlag(Qt::ItemIsEnabled))
        return;

    const bool checked = m_selectedIndexes.contains(logicalIndex(index));
    setData(index, m_selectionMode == MultiSelection && checked ? Qt::Unchecked : Qt::Checked,
            Qt::CheckStateRole);
}

int DefaultPopupMenuModel::logicalIndex(const QModelIndex& index) const {
    return ownsIndex(index) ? logicalIndexForRow(index.row()) : -1;
}

QModelIndex DefaultPopupMenuModel::indexForItem(int itemIndex) const {
    const int row = rowForLogicalIndex(itemIndex);
    return row < 0 ? QModelIndex() : index(row);
}

int DefaultPopupMenuModel::logicalIndexForRow(int row) const {
    return row >= 0 && row < m_rowItems.size() ? m_rowItems.at(row) : -1;
}

int DefaultPopupMenuModel::rowForLogicalIndex(int itemIndex) const {
    return itemIndex >= 0 && itemIndex < m_itemRows.size() ? m_itemRows.at(itemIndex) : -1;
}

bool DefaultPopupMenuModel::ownsIndex(const QModelIndex& index) const {
    return index.isValid() && index.model() == this && index.column() == 0
        && !index.parent().isValid() && index.row() >= 0 && index.row() < rowCount();
}

void DefaultPopupMenuModel::applySelection(const QList<int>& indexes) {
    QList<int> selected;
    for (int itemIndex : indexes) {
        if (rowForLogicalIndex(itemIndex) >= 0 && !selected.contains(itemIndex)) {
            selected.append(itemIndex);
            if (m_selectionMode == SingleSelection)
                break;
        }
    }
    std::sort(selected.begin(), selected.end());
    if (selected == m_selectedIndexes)
        return;

    m_selectedIndexes = selected;
    if (rowCount() > 0)
        emit dataChanged(index(0), index(rowCount() - 1), {Qt::CheckStateRole});
    notifySelection();
}

void DefaultPopupMenuModel::notifySelection() {
    if (m_selectionMode == SingleSelection)
        emit currentIndexChanged(currentIndex());
    else
        emit selectedIndexesChanged(m_selectedIndexes);
}

} // namespace animatedwidgets
