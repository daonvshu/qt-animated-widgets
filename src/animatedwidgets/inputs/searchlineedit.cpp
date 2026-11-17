#include "animatedwidgets/inputs/searchlineedit.h"

#include "animatedwidgets/popups/defaultpopupmenuliststyle.h"
#include "animatedwidgets/popups/defaultpopupmenustyle.h"
#include "animatedwidgets/popups/popupmenu.h"
#include "animatedwidgets/popups/popupmenuitem.h"
#include "animatedwidgets/popups/popupmenurowtype.h"
#include "animatedwidgets/utils/textsearchutils.h"

#include <QAbstractItemModel>
#include <QAbstractItemView>
#include <QFocusEvent>
#include <QHideEvent>
#include <QKeyEvent>
#include <QStandardItem>
#include <QStandardItemModel>

namespace animatedwidgets {

SearchLineEdit::SearchLineEdit(QWidget* parent)
    : LineEdit(parent)
{
    // 输入内容清空时不再保留旧候选项，等待业务侧重新填充
    connect(this, &QLineEdit::textEdited, this, [this](const QString& text) {
        if (text.trimmed().isEmpty()) {
            hideSuggestions();
        }
    });
}

void SearchLineEdit::setSuggestions(const QStringList& suggestions) {
    suggestionItems = suggestions;
    if (suggestionItems.isEmpty() || !hasFocus() || !isVisible()) {
        hideSuggestions();
        return;
    }

    ensurePopup();
    if (!popup || !suggestionModel) {
        return;
    }

    rebuildSuggestionModel();
    popup->setAdaptiveHeight(qMax(200, width()), maximumVisibleItemsValue);
    popup->moveToTargetBottom(this, Qt::AlignLeft, 2);
    popup->show();
    popup->raise();
}

QStringList SearchLineEdit::suggestions() const {
    return suggestionItems;
}

void SearchLineEdit::clearSuggestions() {
    suggestionItems.clear();
    hideSuggestions();
}

bool SearchLineEdit::suggestionsVisible() const {
    return popup && popup->isVisible();
}

int SearchLineEdit::maximumVisibleItems() const {
    return maximumVisibleItemsValue;
}

void SearchLineEdit::setMaximumVisibleItems(int count) {
    maximumVisibleItemsValue = qMax(1, count);
    if (popup) {
        popup->setAdaptiveHeight(qMax(200, width()), maximumVisibleItemsValue);
    }
}

bool SearchLineEdit::highlightEnabled() const {
    return highlightEnabledValue;
}

void SearchLineEdit::setHighlightEnabled(bool enabled) {
    highlightEnabledValue = enabled;
    if (popup && popup->isVisible()) {
        rebuildSuggestionModel();
    }
}

QColor SearchLineEdit::highlightColor() const {
    return highlightColorValue;
}

void SearchLineEdit::setHighlightColor(const QColor& color) {
    if (!color.isValid()) {
        return;
    }
    highlightColorValue = color;
    if (popup && popup->isVisible()) {
        rebuildSuggestionModel();
    }
}

bool SearchLineEdit::highlightBold() const {
    return highlightBoldValue;
}

void SearchLineEdit::setHighlightBold(bool bold) {
    highlightBoldValue = bold;
    if (popup && popup->isVisible()) {
        rebuildSuggestionModel();
    }
}

QColor SearchLineEdit::popupBackground() const {
    return popupBackgroundColor;
}

void SearchLineEdit::setPopupBackground(const QColor& color) {
    popupBackgroundColor = color;
    applyPopupStyle();
}

QColor SearchLineEdit::popupBorder() const {
    return popupBorderColor;
}

void SearchLineEdit::setPopupBorder(const QColor& color) {
    popupBorderColor = color;
    applyPopupStyle();
}

QColor SearchLineEdit::itemText() const {
    return itemTextColor;
}

void SearchLineEdit::setItemText(const QColor& color) {
    itemTextColor = color;
    applyPopupStyle();
}

QColor SearchLineEdit::itemHoverBackground() const {
    return itemHoverBackgroundColor;
}

void SearchLineEdit::setItemHoverBackground(const QColor& color) {
    itemHoverBackgroundColor = color;
    applyPopupStyle();
}

QColor SearchLineEdit::itemSelectedBackground() const {
    return itemSelectedBackgroundColor;
}

void SearchLineEdit::setItemSelectedBackground(const QColor& color) {
    itemSelectedBackgroundColor = color;
    applyPopupStyle();
}

void SearchLineEdit::ensurePopup() {
    // 提示层关闭后会自行销毁，这里按需重建
    if (popup && !popup->isVisible()) {
        popup->deleteLater();
        popup = nullptr;
        suggestionModel = nullptr;
    }
    if (popup) {
        return;
    }

    popup = new PopupMenu(this);
    // 与 ComboBox 一致：去掉 Qt::Popup，改用不接受焦点的工具窗口
    popup->setWindowFlags((popup->windowFlags() & ~Qt::WindowFlags(Qt::WindowType_Mask))
                          | Qt::Tool | Qt::WindowDoesNotAcceptFocus);
    popup->setAttribute(Qt::WA_ShowWithoutActivating);
    applyPopupStyle();

    suggestionModel = new QStandardItemModel(popup);
    popup->setModel(suggestionModel);
    connect(popup, &PopupMenu::itemTriggered, this, [this](const QModelIndex& index) {
        const QString text = index.data(Qt::DisplayRole).toString();
        if (text.isEmpty()) {
            return;
        }
        setText(text);
        emit suggestionActivated(text);
    });
}

void SearchLineEdit::rebuildSuggestionModel() {
    if (!suggestionModel) {
        return;
    }
    suggestionModel->clear();
    const QString query = text().trimmed();
    for (const QString& suggestion : suggestionItems) {
        auto* item = new QStandardItem(suggestion);
        item->setEditable(false);
        item->setData(static_cast<int>(PopupMenuRowType::Option), PopupMenuItem::TypeRole);
        if (highlightEnabledValue && !query.isEmpty()) {
            bool matched = false;
            const QString html = TextSearchUtils::searchAndHighlight(
                    suggestion, query, highlightColorValue, highlightBoldValue, &matched);
            if (matched) {
                item->setData(html, PopupMenuItem::HighlightRole);
            }
        }
        suggestionModel->appendRow(item);
    }
}

void SearchLineEdit::applyPopupStyle() {
    if (!popup) {
        return;
    }
    auto* popupStyle = new DefaultPopupMenuStyle();
    popupStyle->popupBackground = popupBackgroundColor;
    popupStyle->popupBorder = popupBorderColor;
    auto* listStyle = new DefaultPopupMenuListStyle();
    listStyle->itemText = itemTextColor;
    listStyle->itemHoverBackground = itemHoverBackgroundColor;
    listStyle->itemSelectedBackground = itemSelectedBackgroundColor;
    popup->setPopupStyle(popupStyle);
    popup->setListStyle(listStyle);
}

void SearchLineEdit::hideSuggestions() {
    if (popup) {
        popup->hide();
    }
}

void SearchLineEdit::moveCurrentSuggestion(int delta) {
    if (!popup) {
        return;
    }
    QAbstractItemView* view = popup->view();
    if (!view || !view->model()) {
        return;
    }
    const int rows = view->model()->rowCount();
    if (rows <= 0) {
        return;
    }
    int row = view->currentIndex().isValid() ? view->currentIndex().row()
                                             : (delta > 0 ? -1 : rows);
    row = qBound(0, row + delta, rows - 1);
    const QModelIndex index = view->model()->index(row, 0);
    view->setCurrentIndex(index);
    view->scrollTo(index);
}

bool SearchLineEdit::activateCurrentSuggestion() {
    if (!popup) {
        return false;
    }
    QAbstractItemView* view = popup->view();
    if (!view || !view->model() || view->model()->rowCount() <= 0) {
        return false;
    }
    QModelIndex index = view->currentIndex();
    if (!index.isValid()) {
        index = view->model()->index(0, 0);
    }
    const QString text = index.data(Qt::DisplayRole).toString();
    if (text.isEmpty()) {
        return false;
    }
    hideSuggestions();
    setText(text);
    emit suggestionActivated(text);
    return true;
}

void SearchLineEdit::keyPressEvent(QKeyEvent* event) {
    if (popup && popup->isVisible()) {
        switch (event->key()) {
        case Qt::Key_Down:
            moveCurrentSuggestion(1);
            event->accept();
            return;
        case Qt::Key_Up:
            moveCurrentSuggestion(-1);
            event->accept();
            return;
        case Qt::Key_Return:
        case Qt::Key_Enter:
            if (activateCurrentSuggestion()) {
                event->accept();
                return;
            }
            break;
        case Qt::Key_Escape:
            hideSuggestions();
            event->accept();
            return;
        default:
            break;
        }
    }
    LineEdit::keyPressEvent(event);
}

void SearchLineEdit::focusOutEvent(QFocusEvent* event) {
    hideSuggestions();
    LineEdit::focusOutEvent(event);
}

void SearchLineEdit::hideEvent(QHideEvent* event) {
    hideSuggestions();
    LineEdit::hideEvent(event);
}

} // namespace animatedwidgets
