#pragma once

#include <QAbstractItemView>
#include <QList>
#include <QPointer>

class QListView;

namespace animatedwidgets {

class CascaderColumnView : public QAbstractItemView {
    Q_OBJECT

public:
    explicit CascaderColumnView(QWidget* parent = nullptr);
    ~CascaderColumnView() override;

    void setModel(QAbstractItemModel* model) override;
    void setRootIndex(const QModelIndex& index) override;
    void setColumnWidth(int width);
    int contentWidth() const;
    QList<QModelIndex> columnRoots() const;
    QList<QListView*> columns() const;

    QModelIndex indexAt(const QPoint& point) const override;
    QRect visualRect(const QModelIndex& index) const override;
    void scrollTo(const QModelIndex& index, ScrollHint hint = EnsureVisible) override;

signals:
    void columnTriggered(const QModelIndex& index);
    void columnCreated(QAbstractItemView* column);
    void columnsChanged();
    void dismissRequested();

protected:
    void currentChanged(const QModelIndex& current, const QModelIndex& previous) override;
    void resizeEvent(QResizeEvent* event) override;
    void scrollContentsBy(int dx, int dy) override;
    void keyPressEvent(QKeyEvent* event) override;
    QModelIndex moveCursor(CursorAction action, Qt::KeyboardModifiers modifiers) override;
    int horizontalOffset() const override;
    int verticalOffset() const override;
    bool isIndexHidden(const QModelIndex& index) const override;
    void setSelection(const QRect& rect, QItemSelectionModel::SelectionFlags flags) override;
    QRegion visualRegionForSelection(const QItemSelection& selection) const override;

private:
    void rebuildColumns();
    void layoutColumns();

    QList<QPointer<QListView>> m_columns;
    QList<QMetaObject::Connection> m_modelConnections;
    int m_columnWidth = 180;
    static constexpr int ColumnSpacing = 2;
    bool m_rebuilding = false;
};

} // namespace animatedwidgets
