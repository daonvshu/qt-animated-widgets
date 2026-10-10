#include "animatedwidgets/popups/cascadercolumnview.h"

#include "animatedwidgets/utils/smoothscrolling.h"
#include "animatedwidgets/utils/treemodelutils.h"

#include <QAbstractItemModel>
#include <QKeyEvent>
#include <QListView>
#include <QScrollBar>
#include <QSignalBlocker>

namespace animatedwidgets {

CascaderColumnView::CascaderColumnView(QWidget* parent)
    : QAbstractItemView(parent)
{
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

CascaderColumnView::~CascaderColumnView() {
    for (const auto& connection : m_modelConnections)
        disconnect(connection);
    for (const auto& column : m_columns) {
        if (column) {
            disconnect(column, nullptr, this, nullptr);
            if (column->selectionModel())
                disconnect(column->selectionModel(), nullptr, this, nullptr);
        }
    }
}

void CascaderColumnView::setModel(QAbstractItemModel* model) {
    for (const auto& connection : m_modelConnections)
        disconnect(connection);
    m_modelConnections.clear();
    for (const auto& column : m_columns) {
        if (column) {
            disconnect(column, nullptr, this, nullptr);
            if (column->selectionModel())
                disconnect(column->selectionModel(), nullptr, this, nullptr);
            column->hide();
            column->deleteLater();
        }
    }
    m_columns.clear();

    QAbstractItemView::setModel(model);
    if (model) {
        m_modelConnections.append(connect(model, &QAbstractItemModel::rowsInserted, this, &CascaderColumnView::rebuildColumns));
        m_modelConnections.append(connect(model, &QAbstractItemModel::rowsRemoved, this, &CascaderColumnView::rebuildColumns));
        m_modelConnections.append(connect(model, &QAbstractItemModel::modelReset, this, &CascaderColumnView::rebuildColumns));
        m_modelConnections.append(connect(model, &QAbstractItemModel::layoutChanged, this, &CascaderColumnView::rebuildColumns));
        m_modelConnections.append(connect(model, &QAbstractItemModel::dataChanged, this, &CascaderColumnView::rebuildColumns));
    }
    rebuildColumns();
}

void CascaderColumnView::setRootIndex(const QModelIndex& index) {
    QAbstractItemView::setRootIndex(index);
    rebuildColumns();
}

void CascaderColumnView::setColumnWidth(int width) {
    m_columnWidth = qMax(80, width);
    layoutColumns();
}

int CascaderColumnView::contentWidth() const {
    const int count = static_cast<int>(m_columns.size());
    return count * m_columnWidth + qMax(0, count - 1) * ColumnSpacing;
}

QList<QModelIndex> CascaderColumnView::columnRoots() const {
    return model() ? TreeModelUtils::columnRoots(currentIndex(), rootIndex()) : QList<QModelIndex>();
}

QList<QListView*> CascaderColumnView::columns() const {
    QList<QListView*> result;
    for (const auto& column : m_columns)
        if (column)
            result.append(column);
    return result;
}

void CascaderColumnView::rebuildColumns() {
    if (m_rebuilding)
        return;
    m_rebuilding = true;
    const auto roots = columnRoots();
    const auto path = TreeModelUtils::path(currentIndex(), rootIndex());

    while (m_columns.size() > roots.size()) {
        auto* column = m_columns.takeLast().data();
        if (column) {
            disconnect(column, nullptr, this, nullptr);
            disconnect(column->selectionModel(), nullptr, this, nullptr);
            column->hide();
            column->deleteLater();
        }
    }

    for (int level = 0; level < roots.size(); ++level) {
        QListView* column = level < m_columns.size() ? m_columns.at(level).data() : nullptr;
        if (!column) {
            column = new QListView(viewport());
            column->setModel(model());
            column->setFocusPolicy(Qt::NoFocus);
            column->setEditTriggers(QAbstractItemView::NoEditTriggers);
            column->setSelectionMode(QAbstractItemView::SingleSelection);
            column->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            column->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
            column->setMouseTracking(true);
            column->viewport()->setMouseTracking(true);
            SmoothScrolling::install(column);
            m_columns.append(column);

            // Rebuild after release so resizing and repositioning cannot interrupt the click.
            connect(column, &QAbstractItemView::clicked, this, [this](const QModelIndex& index) {
                if (!index.flags().testFlag(Qt::ItemIsEnabled))
                    return;
                setCurrentIndex(index);
                emit columnTriggered(index);
            });
            emit columnCreated(column);
        }

        QSignalBlocker blocker(column->selectionModel());
        const bool rootChanged = column->rootIndex() != roots.at(level);
        if (rootChanged)
            column->setRootIndex(roots.at(level));
        const QModelIndex current = level < path.size() ? path.at(level) : QModelIndex();
        if (column->currentIndex() != current)
            column->setCurrentIndex(current);
        if (rootChanged)
            column->scrollToTop();
        column->show();
    }

    layoutColumns();
    m_rebuilding = false;
    emit columnsChanged();
}

void CascaderColumnView::layoutColumns() {
    horizontalScrollBar()->setPageStep(viewport()->width());
    horizontalScrollBar()->setRange(0, qMax(0, contentWidth() - viewport()->width()));
    for (int level = 0; level < m_columns.size(); ++level) {
        if (m_columns.at(level))
            m_columns.at(level)->setGeometry(level * (m_columnWidth + ColumnSpacing) - horizontalOffset(), 0,
                                             m_columnWidth, viewport()->height());
    }
}

QModelIndex CascaderColumnView::indexAt(const QPoint& point) const {
    for (const auto& column : m_columns) {
        if (column && column->geometry().contains(point))
            return column->indexAt(column->viewport()->mapFrom(viewport(), point));
    }
    return QModelIndex();
}

QRect CascaderColumnView::visualRect(const QModelIndex& index) const {
    for (const auto& column : m_columns) {
        if (column && column->rootIndex() == index.parent()) {
            QRect rect = column->visualRect(index);
            rect.moveTopLeft(column->viewport()->mapTo(viewport(), rect.topLeft()));
            return rect;
        }
    }
    return QRect();
}

void CascaderColumnView::scrollTo(const QModelIndex& index, ScrollHint hint) {
    for (int level = 0; level < m_columns.size(); ++level) {
        auto* column = m_columns.at(level).data();
        if (!column || column->rootIndex() != index.parent())
            continue;
        const int left = level * (m_columnWidth + ColumnSpacing);
        const int right = left + m_columnWidth;
        if (left < horizontalOffset())
            horizontalScrollBar()->setValue(left);
        else if (right > horizontalOffset() + viewport()->width())
            horizontalScrollBar()->setValue(right - viewport()->width());
        column->scrollTo(index, hint);
        return;
    }
}

void CascaderColumnView::currentChanged(const QModelIndex& current, const QModelIndex& previous) {
    QAbstractItemView::currentChanged(current, previous);
    rebuildColumns();
    scrollTo(current);
}

void CascaderColumnView::resizeEvent(QResizeEvent* event) {
    QAbstractItemView::resizeEvent(event);
    layoutColumns();
}

void CascaderColumnView::scrollContentsBy(int, int) {
    layoutColumns();
}

void CascaderColumnView::keyPressEvent(QKeyEvent* event) {
    const int key = event->key();
    if (key == Qt::Key_Escape) {
        emit dismissRequested();
        return;
    }
    if (key == Qt::Key_Return || key == Qt::Key_Enter || key == Qt::Key_Space) {
        if (currentIndex().flags().testFlag(Qt::ItemIsEnabled))
            emit columnTriggered(currentIndex());
        return;
    }
    QAbstractItemView::keyPressEvent(event);
}

QModelIndex CascaderColumnView::moveCursor(CursorAction action, Qt::KeyboardModifiers) {
    const QModelIndex current = currentIndex();
    if (!model())
        return QModelIndex();
    if (action == MoveLeft)
        return current.parent() == rootIndex() ? current : current.parent();

    const bool enter = action == MoveRight;
    const QModelIndex parent = enter ? current : current.isValid() ? current.parent() : rootIndex();
    if (enter && (!current.isValid() || TreeModelUtils::isLeaf(current)))
        return current;
    const int direction = action == MoveUp || action == MoveEnd ? -1 : 1;
    int row = enter || !current.isValid() || action == MoveHome ? 0
        : action == MoveEnd ? model()->rowCount(parent) - 1 : current.row() + direction;
    for (; row >= 0 && row < model()->rowCount(parent); row += direction) {
        const QModelIndex index = model()->index(row, 0, parent);
        if (index.flags().testFlag(Qt::ItemIsEnabled))
            return index;
    }
    return current;
}

int CascaderColumnView::horizontalOffset() const {
    return horizontalScrollBar()->value();
}

int CascaderColumnView::verticalOffset() const {
    return 0;
}

bool CascaderColumnView::isIndexHidden(const QModelIndex& index) const {
    return !visualRect(index).isValid();
}

void CascaderColumnView::setSelection(const QRect&, QItemSelectionModel::SelectionFlags) {
}

QRegion CascaderColumnView::visualRegionForSelection(const QItemSelection& selection) const {
    QRegion region;
    for (const auto& range : selection)
        for (const QModelIndex& index : range.indexes())
            region += visualRect(index);
    return region;
}

} // namespace animatedwidgets
