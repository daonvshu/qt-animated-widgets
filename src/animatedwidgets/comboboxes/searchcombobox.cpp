#include "animatedwidgets/comboboxes/searchcombobox.h"
#include "animatedwidgets/comboboxes/combolistpopup.h"
#include "animatedwidgets/popups/popupmenufiltermodel.h"

#include <QAbstractItemView>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QTimer>

namespace animatedwidgets {

SearchComboBox::SearchComboBox(QWidget* parent)
    : ComboBox(parent)
{
    setEditable(true);
    setInsertPolicy(QComboBox::NoInsert);
    setCompleter(nullptr);
    syncSearchEditor();

    connect(this, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this] {
        m_editing = false;
        m_query.clear();
        restoreSelectionText();
        if (m_searchPopup)
            m_searchPopup->setSearchText(QString());
    });
}

bool SearchComboBox::highlightEnabled() const {
    return m_highlightEnabled;
}

void SearchComboBox::setHighlightEnabled(bool enabled) {
    m_highlightEnabled = enabled;
    applyHighlight();
}

QColor SearchComboBox::highlightColor() const {
    return m_highlightColor;
}

void SearchComboBox::setHighlightColor(const QColor& color) {
    if (!color.isValid())
        return;
    m_highlightColor = color;
    applyHighlight();
}

bool SearchComboBox::highlightBold() const {
    return m_highlightBold;
}

void SearchComboBox::setHighlightBold(bool bold) {
    m_highlightBold = bold;
    applyHighlight();
}

QWidget* SearchComboBox::emptyWidget() const {
    return m_emptyWidget;
}

void SearchComboBox::setEmptyWidget(QWidget* widget) {
    if (widget == m_emptyWidget || widget == this || widget == lineEdit()
        || (widget && (widget->isAncestorOf(this) || (m_searchPopup
            && (widget == m_searchPopup || widget->isAncestorOf(m_searchPopup) || m_searchPopup->isAncestorOf(widget))))))
        return;
    hidePopup();
    if (m_emptyWidget)
        m_emptyWidget->deleteLater();
    m_emptyWidget = widget;
    if (widget) {
        widget->setParent(this);
        widget->hide();
    }
}

void SearchComboBox::applyHighlight() {
    if (!m_searchPopup)
        return;
    auto* filter = m_searchPopup->enableFiltering();
    filter->setHighlightEnabled(m_highlightEnabled);
    filter->setHighlightColor(m_highlightColor);
    filter->setHighlightBold(m_highlightBold);
}

void SearchComboBox::syncSearchEditor() {
    if (lineEdit() == m_searchEditor)
        return;
    if (m_searchEditor) {
        m_searchEditor->removeEventFilter(this);
        disconnect(m_searchEditor, nullptr, this, nullptr);
    }
    m_searchEditor = lineEdit();
    if (!m_searchEditor)
        return;

    setCompleter(nullptr);
    m_searchEditor->installEventFilter(this);
    connect(m_searchEditor.data(), &QLineEdit::textEdited, this, [this](const QString& text) {
        m_editing = true;
        m_query = text;
        QPointer<SearchComboBox> guard(this);
        if (popupOnEdit() && !currentPopup())
            showPopup();
        if (guard && m_searchPopup) {
            m_searchPopup->setSearchText(m_query);
            m_searchPopup->view()->setCurrentIndex(QModelIndex());
        }
    });
}

void SearchComboBox::configurePopup(ComboListPopup* popup) {
    ComboBox::configurePopup(popup);
    m_searchPopup = popup;
    popup->enableFiltering();
    if (m_emptyWidget)
        popup->setEmptyWidget(m_emptyWidget);
    applyHighlight();
    popup->setSearchText(m_editing ? m_query : QString());
    popup->setCurrentRow(currentIndex());
}

void SearchComboBox::restoreSelectionText() {
    if (lineEdit())
        lineEdit()->setText(currentIndex() >= 0 ? itemText(currentIndex()) : QString());
}

void SearchComboBox::popupClosed() {
    if (m_searchPopup && m_emptyWidget && m_searchPopup->emptyWidget() == m_emptyWidget) {
        m_searchPopup->releaseEmptyWidget();
        m_emptyWidget->setParent(this);
        m_emptyWidget->hide();
    }
    m_searchPopup = nullptr;
    m_editing = false;
    m_query.clear();
    restoreSelectionText();
}

bool SearchComboBox::event(QEvent* event) {
    const bool handled = ComboBox::event(event);
    if (event->type() == QEvent::ChildAdded || event->type() == QEvent::Show
        || event->type() == QEvent::LayoutRequest)
        syncSearchEditor();
    return handled;
}

bool SearchComboBox::eventFilter(QObject* watched, QEvent* event) {
    const bool editorEvent = watched == m_searchEditor || watched == this;
    if (!editorEvent)
        return ComboBox::eventFilter(watched, event);

    if (watched == m_searchEditor && event->type() == QEvent::MouseButtonRelease
        && static_cast<QMouseEvent*>(event)->button() == Qt::LeftButton) {
        // Finish the editor's mouse handling before showing the result window.
        QTimer::singleShot(0, this, [this] {
            if (isEnabled() && m_searchEditor && m_searchEditor->hasFocus()
                && !m_searchEditor->isReadOnly() && !currentPopup())
                showPopup();
        });
    }

    if (editorEvent && event->type() == QEvent::FocusOut) {
        QTimer::singleShot(0, this, [this] {
            if (m_searchEditor && !m_searchEditor->hasFocus() && currentPopup()) {
                hidePopup();
                return;
            }
            if (!currentPopup() && m_editing) {
                m_editing = false;
                m_query.clear();
                restoreSelectionText();
            }
        });
    }

    if (event->type() == QEvent::KeyPress) {
        const auto* key = static_cast<QKeyEvent*>(event);
        if (key->key() == Qt::Key_Escape && currentPopup()) {
            hidePopup();
            return true;
        }
        if (key->key() == Qt::Key_Down || key->key() == Qt::Key_Up) {
            QPointer<SearchComboBox> guard(this);
            if (!currentPopup())
                showPopup();
            if (guard && m_searchPopup)
                m_searchPopup->moveSelection(key->key() == Qt::Key_Down ? 1 : -1);
            return true;
        }
        if (key->key() == Qt::Key_Return || key->key() == Qt::Key_Enter) {
            if (m_searchPopup) {
                if (!m_searchPopup->view()->currentIndex().isValid())
                    m_searchPopup->moveSelection(1);
                m_searchPopup->triggerCurrentItem();
            } else {
                m_editing = false;
                m_query.clear();
                restoreSelectionText();
            }
            return true;
        }
        if (key->key() == Qt::Key_Tab || key->key() == Qt::Key_Backtab) {
            hidePopup();
            focusNextPrevChild(key->key() == Qt::Key_Tab);
            return true;
        }
    }

    return ComboBox::eventFilter(watched, event);
}

} // namespace animatedwidgets
