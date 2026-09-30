#include "animatedwidgets/comboboxes/combolistpopup.h"
#include "animatedwidgets/popups/fadeslidepopupanimation.h"

#include <QAbstractItemModel>
#include <QEvent>
#include <QKeyEvent>
#include <QListView>
#include <QVBoxLayout>

namespace animatedwidgets {

ComboListPopup::ComboListPopup(QWidget* parent)
    : BasePopupDlg(parent)
{
    setObjectName(QStringLiteral("comboListPopup"));
    setStyleSheet(QStringLiteral("#comboListPopup { background: white; border-radius: 8px; }"));
    if (auto* animation = qobject_cast<FadeSlidePopupAnimation*>(popupAnimation()))
        animation->setSlideDistance(20);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(shadowCornerRadius, shadowCornerRadius, shadowCornerRadius, shadowCornerRadius);
    layout->setSpacing(0);

    m_list = new QListView(this);
    m_list->setStyleSheet(QStringLiteral("QListView { background: transparent; border: none; outline: none; }"
                                         "QListView::item { min-height: 30px; padding: 2px 8px; }"
                                         "QListView::item:hover { background: #e8f0ff; }"
                                         "QListView::item:selected { background: #dbe9ff; color: #20242a; }"));
    m_list->setFrameShape(QFrame::NoFrame);
    m_list->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_list->setSelectionMode(QAbstractItemView::SingleSelection);
    m_list->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_list->setMouseTracking(true);
    m_list->installEventFilter(this);
    setFocusProxy(m_list);
    layout->addWidget(m_list);

    connect(m_list, &QListView::clicked, this, [this](const QModelIndex& index) {
        if (index.isValid() && index.flags().testFlag(Qt::ItemIsEnabled))
            emit itemChosen(index.row());
    });
}

void ComboListPopup::setItems(QAbstractItemModel* model, const QModelIndex& root, int column, int currentRow, int visibleRows, int width) {
    m_list->setModel(model);
    m_list->setRootIndex(root);
    m_list->setModelColumn(column);

    const int rowCount = model ? model->rowCount(root) : 0;
    const int rowHeight = qMax(32, m_list->sizeHintForRow(0));
    const int rows = qMax(1, qMin(rowCount, qMax(1, visibleRows)));
    setContentFixedSize(qMax(80, width), rows * rowHeight + 2 * shadowCornerRadius);

    if (currentRow >= 0 && currentRow < rowCount) {
        const QModelIndex current = model->index(currentRow, column, root);
        m_list->setCurrentIndex(current);
        m_list->scrollTo(current);
    }
}

bool ComboListPopup::eventFilter(QObject* watched, QEvent* event) {
    if (watched == m_list && event->type() == QEvent::KeyPress) {
        auto* key = static_cast<QKeyEvent*>(event);
        if (key->key() == Qt::Key_Escape) {
            close();
            return true;
        }
        if (key->key() == Qt::Key_Return || key->key() == Qt::Key_Enter || key->key() == Qt::Key_Space) {
            const QModelIndex current = m_list->currentIndex();
            if (current.isValid() && current.flags().testFlag(Qt::ItemIsEnabled))
                emit itemChosen(current.row());
            return true;
        }
    }
    return BasePopupDlg::eventFilter(watched, event);
}

} // namespace animatedwidgets
