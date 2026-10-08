#pragma once

#include <QAbstractListModel>
#include <QStringList>
#include <QSet>

namespace animatedwidgets {

class DefaultPopupMenuModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum SelectionMode { SingleSelection, MultiSelection };
    Q_ENUM(SelectionMode)

    explicit DefaultPopupMenuModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

    QStringList strings() const;
    void setStrings(const QStringList& strings);

    SelectionMode selectionMode() const;
    void setSelectionMode(SelectionMode mode);

    int currentIndex() const;
    void setCurrentIndex(int row);
    QList<int> selectedIndexes() const;
    void setSelectedIndexes(const QList<int>& rows);

    void setItemEnabled(int row, bool enabled);

public slots:
    void triggerItem(const QModelIndex& index);

signals:
    void currentIndexChanged(int index);
    void selectedIndexesChanged(const QList<int>& indexes);

private:
    bool ownsIndex(const QModelIndex& index) const;
    void applySelection(const QList<int>& rows);
    void notifySelection();

    QStringList m_strings;
    QList<int> m_selectedIndexes;
    QSet<int> m_disabledRows;
    SelectionMode m_selectionMode = SingleSelection;
};

} // namespace animatedwidgets
