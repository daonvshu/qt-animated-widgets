#include "animatedwidgets/popups/defaultpopupmenumodel.h"

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

    const auto& item = m_items.at(index.row());
    if (role == Qt::DisplayRole || role == Qt::EditRole)
        return item.text;
    if (role == Qt::DecorationRole)
        return item.icon;
    if (role == PopupMenuItem::TypeRole)
        return item.type;
    if (role == PopupMenuItem::SeparatorRole)
        return item.type == PopupMenuItem::Separator;
    if (role == PopupMenuItem::ShortcutRole)
        return item.shortcut;
    if (role == Qt::CheckStateRole && item.type == PopupMenuItem::Option)
        return m_selectedIndexes.contains(index.row()) ? Qt::Checked : Qt::Unchecked;
    return role == Qt::CheckStateRole ? QVariant() : item.values.value(role);
}

Qt::ItemFlags DefaultPopupMenuModel::flags(const QModelIndex& index) const {
    if (!ownsIndex(index))
        return Qt::NoItemFlags;

    if (m_items.at(index.row()).type != PopupMenuItem::Option)
        return Qt::NoItemFlags;
    if (!m_items.at(index.row()).enabled)
        return Qt::ItemIsSelectable;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool DefaultPopupMenuModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!ownsIndex(index))
        return false;

    auto& item = m_items[index.row()];
    if (role != Qt::CheckStateRole) {
        if (role == PopupMenuItem::TypeRole || role == PopupMenuItem::SeparatorRole)
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
        const QVector<int> roles = role == Qt::DisplayRole || role == Qt::EditRole
            ? QVector<int>{Qt::DisplayRole, Qt::EditRole} : QVector<int>{role};
        emit dataChanged(index, index, roles);
        return true;
    }
    if (item.type != PopupMenuItem::Option)
        return false;

    bool valid = false;
    const int state = value.toInt(&valid);
    if (!valid || (state != Qt::Checked && state != Qt::Unchecked))
        return false;

    QList<int> rows = m_selectedIndexes;
    if (state == Qt::Unchecked)
        rows.removeAll(index.row());
    else if (m_selectionMode == SingleSelection)
        rows = {index.row()};
    else if (!rows.contains(index.row()))
        rows.append(index.row());
    applySelection(rows);
    return true;
}

QStringList DefaultPopupMenuModel::strings() const {
    QStringList result;
    for (const auto& item : m_items)
        result.append(item.text);
    return result;
}

void DefaultPopupMenuModel::setStrings(const QStringList& strings) {
    bool unchanged = m_items.size() == strings.size();
    if (unchanged) {
        for (int row = 0; row < rowCount(); ++row) {
            const auto& item = m_items.at(row);
            if (item.type != PopupMenuItem::Option || !item.icon.isNull() || !item.shortcut.isEmpty()
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

void DefaultPopupMenuModel::setCurrentIndex(int row) {
    if (row == -1)
        applySelection({});
    else if (row >= 0 && row < rowCount())
        applySelection({row});
}

QList<int> DefaultPopupMenuModel::selectedIndexes() const {
    return m_selectedIndexes;
}

void DefaultPopupMenuModel::setSelectedIndexes(const QList<int>& rows) {
    applySelection(rows);
}

void DefaultPopupMenuModel::setItemEnabled(int row, bool enabled) {
    if (row < 0 || row >= rowCount() || enabled == m_items.at(row).enabled)
        return;

    m_items[row].enabled = enabled;
    emit dataChanged(index(row), index(row), {});
}

void DefaultPopupMenuModel::triggerItem(const QModelIndex& index) {
    if (!ownsIndex(index) || !flags(index).testFlag(Qt::ItemIsEnabled))
        return;

    const bool checked = m_selectedIndexes.contains(index.row());
    setData(index, m_selectionMode == MultiSelection && checked ? Qt::Unchecked : Qt::Checked,
            Qt::CheckStateRole);
}

bool DefaultPopupMenuModel::ownsIndex(const QModelIndex& index) const {
    return index.isValid() && index.model() == this && index.column() == 0
        && !index.parent().isValid() && index.row() >= 0 && index.row() < rowCount();
}

void DefaultPopupMenuModel::applySelection(const QList<int>& rows) {
    QList<int> selected;
    for (int row : rows) {
        if (row >= 0 && row < rowCount() && m_items.at(row).type == PopupMenuItem::Option
            && !selected.contains(row)) {
            selected.append(row);
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
