#pragma once

#include "animatedwidgets/inputs/lineedit.h"

#include <QColor>
#include <QPointer>
#include <QStringList>

class QStandardItemModel;

namespace animatedwidgets {

class PopupMenu;

// 带输入建议的搜索输入框：业务侧只需调用 setSuggestions 填充候选项
class SearchLineEdit : public LineEdit {
    Q_OBJECT
    Q_PROPERTY(int maximumVisibleItems READ maximumVisibleItems WRITE setMaximumVisibleItems)
    Q_PROPERTY(bool highlightEnabled READ highlightEnabled WRITE setHighlightEnabled)
    Q_PROPERTY(QColor highlightColor READ highlightColor WRITE setHighlightColor)
    Q_PROPERTY(bool highlightBold READ highlightBold WRITE setHighlightBold)
    Q_PROPERTY(QColor popupBackground READ popupBackground WRITE setPopupBackground)
    Q_PROPERTY(QColor popupBorder READ popupBorder WRITE setPopupBorder)
    Q_PROPERTY(QColor itemText READ itemText WRITE setItemText)
    Q_PROPERTY(QColor itemHoverBackground READ itemHoverBackground WRITE setItemHoverBackground)
    Q_PROPERTY(QColor itemSelectedBackground READ itemSelectedBackground WRITE setItemSelectedBackground)

public:
    explicit SearchLineEdit(QWidget* parent = nullptr);

    // 用候选项填充提示层；空列表会收起提示层
    void setSuggestions(const QStringList& suggestions);
    QStringList suggestions() const;
    void clearSuggestions();
    bool suggestionsVisible() const;

    int maximumVisibleItems() const;
    void setMaximumVisibleItems(int count);

    bool highlightEnabled() const;
    void setHighlightEnabled(bool enabled);
    QColor highlightColor() const;
    void setHighlightColor(const QColor& color);
    bool highlightBold() const;
    void setHighlightBold(bool bold);

    QColor popupBackground() const;
    void setPopupBackground(const QColor& color);
    QColor popupBorder() const;
    void setPopupBorder(const QColor& color);
    QColor itemText() const;
    void setItemText(const QColor& color);
    QColor itemHoverBackground() const;
    void setItemHoverBackground(const QColor& color);
    QColor itemSelectedBackground() const;
    void setItemSelectedBackground(const QColor& color);

signals:
    // 用户选中候选项后发出，文本已经写入输入框
    void suggestionActivated(const QString& suggestion);

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
    void hideEvent(QHideEvent* event) override;

private:
    void ensurePopup();
    void rebuildSuggestionModel();
    void hideSuggestions();
    void moveCurrentSuggestion(int delta);
    bool activateCurrentSuggestion();
    void applyPopupStyle();

    QStringList suggestionItems;
    QPointer<PopupMenu> popup;
    QPointer<QStandardItemModel> suggestionModel;
    int maximumVisibleItemsValue = 8;
    bool highlightEnabledValue = true;
    QColor highlightColorValue = QColor(QStringLiteral("#1778ff"));
    bool highlightBoldValue = true;
    QColor popupBackgroundColor = QColor(Qt::white);
    QColor popupBorderColor = QColor(QStringLiteral("#d5dae0"));
    QColor itemTextColor = QColor(QStringLiteral("#20242a"));
    QColor itemHoverBackgroundColor = QColor(QStringLiteral("#F2F3F5"));
    QColor itemSelectedBackgroundColor = QColor(QStringLiteral("#E3F3FF"));
};

} // namespace animatedwidgets
