#pragma once

#include <QAbstractListModel>
#include <QPair>

#include "animatedwidgets/popups/popupmenugroup.h"

namespace animatedwidgets {

class GroupedPopupMenuModel : public QAbstractListModel {
    Q_OBJECT

public:
    explicit GroupedPopupMenuModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

    QList<PopupMenuGroup> groups() const;
    void setGroups(const QList<PopupMenuGroup>& groups);

    // Group roles remain accessible even when the group has no visible heading.
    QVariant groupData(int groupIndex, int role = Qt::DisplayRole) const;
    bool setGroupData(int groupIndex, const QVariant& value, int role = Qt::UserRole);

    int currentIndex(int groupIndex) const;
    void setCurrentIndex(int groupIndex, int itemIndex);
    QList<QPair<int, int>> selectedIndexes() const;
    void setSelectedIndexes(const QList<QPair<int, int>>& indexes);

    void setItemEnabled(int groupIndex, int itemIndex, bool enabled);

    // Converts between a display row and group/item indexes that exclude separators.
    QPair<int, int> logicalIndex(const QModelIndex& index) const;
    QModelIndex indexForItem(int groupIndex, int itemIndex) const;

public slots:
    void triggerItem(const QModelIndex& index);

signals:
    // Emitted when a stored group role changes, including groups without a heading.
    void groupDataChanged(int groupIndex, int role);

    // Emitted for each changed single-selection group; itemIndex is -1 when cleared.
    void currentIndexChanged(int groupIndex, int itemIndex);
    void selectedIndexesChanged(const QList<QPair<int, int>>& indexes);

private:
    struct Entry {
        int groupIndex = -1;
        int sourceItemIndex = -1;
        int itemIndex = -1;
    };

    bool ownsIndex(const QModelIndex& index) const;
    void applySelection(const QList<QPair<int, int>>& indexes);

    QList<PopupMenuGroup> m_groups;
    QList<Entry> m_entries;
    QList<QPair<int, int>> m_selectedIndexes;
};

} // namespace animatedwidgets
