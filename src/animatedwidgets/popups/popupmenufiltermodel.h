#pragma once

#include <QSortFilterProxyModel>
#include <QColor>

namespace animatedwidgets {

class PopupMenuFilterModel : public QSortFilterProxyModel {
    Q_OBJECT

public:
    explicit PopupMenuFilterModel(QObject* parent = nullptr);

    QString searchText() const;
    void setSearchText(const QString& text);
    void setSourceModel(QAbstractItemModel* model) override;

    bool highlightEnabled() const;
    void setHighlightEnabled(bool enabled);
    QColor highlightColor() const;
    void setHighlightColor(const QColor& color);
    bool highlightBold() const;
    void setHighlightBold(bool bold);

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

protected:
    bool filterAcceptsRow(int row, const QModelIndex& parent) const override;

private:
    bool matchesOption(int row, const QModelIndex& parent) const;
    void notifyHighlightChanged();
    QString m_searchText;
    QColor m_highlightColor = QColor(QStringLiteral("#1778ff"));
    bool m_highlightEnabled = true;
    bool m_highlightBold = true;
    QList<QMetaObject::Connection> m_sourceConnections;
};

} // namespace animatedwidgets
