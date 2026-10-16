#pragma once

#include "animatedwidgets/namespacealias.h"

#include "animatedwidgets/inputs/compositeinputbase.h"
#include "animatedwidgets/popups/popupmenu.h"
#include "animatedwidgets/popups/popupmenuitem.h"
#include "animatedwidgets/tags/tagstyle.h"

#include <QModelIndex>
#include <QStringList>
#include <functional>

class QHBoxLayout;
class QLabel;
class QScrollArea;

namespace animatedwidgets {

class DefaultPopupMenuModel;
class HoverIconButton;
class LineEdit;
class PopupMenuFilterModel;
class TagWidget;

class MultiSelectComboBox : public CompositeInputBase {
    Q_OBJECT
    Q_PROPERTY(bool searchable READ isSearchable WRITE setSearchable)
    Q_PROPERTY(QString placeholderText READ placeholderText WRITE setPlaceholderText)
    Q_PROPERTY(QString searchText READ searchText WRITE setSearchText NOTIFY searchTextChanged)
    Q_PROPERTY(int contentPadding READ contentPadding WRITE setContentPadding)
    Q_PROPERTY(int tagSpacing READ tagSpacing WRITE setTagSpacing)
    Q_PROPERTY(QColor tagBackground READ tagBackground WRITE setTagBackground)
    Q_PROPERTY(QColor tagBorder READ tagBorder WRITE setTagBorder)
    Q_PROPERTY(QColor tagContent READ tagContent WRITE setTagContent)
    Q_PROPERTY(int tagCornerRadius READ tagCornerRadius WRITE setTagCornerRadius)
    Q_PROPERTY(int tagBorderWidth READ tagBorderWidth WRITE setTagBorderWidth)
    Q_PROPERTY(int tagContentPadding READ tagContentPadding WRITE setTagContentPadding)
    Q_PROPERTY(QSize tagIconSize READ tagIconSize WRITE setTagIconSize)
    Q_PROPERTY(int tagIconSpacing READ tagIconSpacing WRITE setTagIconSpacing)
    Q_PROPERTY(bool tagCloseButtonVisible READ tagCloseButtonVisible WRITE setTagCloseButtonVisible)
    Q_PROPERTY(QIcon tagCloseIcon READ tagCloseIcon WRITE setTagCloseIcon)
    Q_PROPERTY(QSize tagCloseIconSize READ tagCloseIconSize WRITE setTagCloseIconSize)
    Q_PROPERTY(int tagCloseButtonSize READ tagCloseButtonSize WRITE setTagCloseButtonSize)
    Q_PROPERTY(bool clearButtonVisible READ clearButtonVisible WRITE setClearButtonVisible)
    Q_PROPERTY(bool arrowVisible READ arrowVisible WRITE setArrowVisible)
    Q_PROPERTY(QIcon arrowIcon READ arrowIcon WRITE setArrowIcon)
    Q_PROPERTY(bool iconHint READ iconHint WRITE setIconHint)
    Q_PROPERTY(QSize arrowIconSize READ arrowIconSize WRITE setArrowIconSize)
    Q_PROPERTY(int popupWidth READ popupWidth WRITE setPopupWidth)
    Q_PROPERTY(int maxVisibleItems READ maxVisibleItems WRITE setMaxVisibleItems)
    Q_PROPERTY(QString popupScrollBarStyleSheet READ popupScrollBarStyleSheet WRITE setPopupScrollBarStyleSheet)

public:
    using TagStyleFactory = std::function<TagStyle(const QModelIndex&)>;

    explicit MultiSelectComboBox(QWidget* parent = nullptr);
    ~MultiSelectComboBox() override;

    void setStrings(const QStringList& strings);
    void setItems(const QList<PopupMenuItem>& items);
    DefaultPopupMenuModel* model() const;
    PopupMenuFilterModel* filterModel() const;
    LineEdit* editor() const;
    HoverIconButton* clearButton() const;
    HoverIconButton* arrowButton() const;
    PopupMenu* currentPopup() const;

    QList<int> selectedIndexes() const;
    void setSelectedIndexes(const QList<int>& indexes);

    bool isSearchable() const;
    void setSearchable(bool searchable);
    QString placeholderText() const;
    void setPlaceholderText(const QString& text);
    QString searchText() const;
    void setSearchText(const QString& text);

    int contentPadding() const;
    void setContentPadding(int padding);
    int tagSpacing() const;
    void setTagSpacing(int spacing);
    TagStyle tagStyle() const;
    void setTagStyle(const TagStyle& style);
    // Receives a source-model index; an empty factory restores the shared tag style.
    void setTagStyleFactory(TagStyleFactory factory);
    void refreshTags();

    QColor tagBackground() const;
    void setTagBackground(const QColor& color);
    QColor tagBorder() const;
    void setTagBorder(const QColor& color);
    QColor tagContent() const;
    void setTagContent(const QColor& color);
    int tagCornerRadius() const;
    void setTagCornerRadius(int radius);
    int tagBorderWidth() const;
    void setTagBorderWidth(int width);
    int tagContentPadding() const;
    void setTagContentPadding(int padding);
    QSize tagIconSize() const;
    void setTagIconSize(const QSize& size);
    int tagIconSpacing() const;
    void setTagIconSpacing(int spacing);

    bool tagCloseButtonVisible() const;
    void setTagCloseButtonVisible(bool visible);
    QIcon tagCloseIcon() const;
    void setTagCloseIcon(const QIcon& icon);
    QSize tagCloseIconSize() const;
    void setTagCloseIconSize(const QSize& size);
    int tagCloseButtonSize() const;
    void setTagCloseButtonSize(int size);
    bool clearButtonVisible() const;
    void setClearButtonVisible(bool visible);
    bool arrowVisible() const;
    void setArrowVisible(bool visible);
    QIcon arrowIcon() const;
    void setArrowIcon(const QIcon& icon);
    bool iconHint() const;
    void setIconHint(bool enabled);
    QSize arrowIconSize() const;
    void setArrowIconSize(const QSize& size);

    int popupWidth() const;
    void setPopupWidth(int width);
    int maxVisibleItems() const;
    void setMaxVisibleItems(int count);
    QString popupScrollBarStyleSheet() const;
    void setPopupScrollBarStyleSheet(const QString& styleSheet);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

public slots:
    void clearSelection();
    void showPopup();
    void hidePopup();

signals:
    void selectedIndexesChanged(const QList<int>& indexes);
    void searchTextChanged(const QString& text);
    void popupAboutToShow(PopupMenu* popup);

protected:
    bool event(QEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    void updateContentLayout();
    void ensureEditorVisible();
    void schedulePopupLayout();
    void updatePopupLayout();
    void movePopupSelection(int direction);
    void removeTag(int itemIndex);

    DefaultPopupMenuModel* m_model = nullptr;
    PopupMenuFilterModel* m_filterModel = nullptr;
    QScrollArea* m_scrollArea = nullptr;
    QWidget* m_content = nullptr;
    QWidget* m_body = nullptr;
    QHBoxLayout* m_contentLayout = nullptr;
    QHBoxLayout* m_bodyLayout = nullptr;
    LineEdit* m_editor = nullptr;
    QLabel* m_placeholder = nullptr;
    HoverIconButton* m_clearButton = nullptr;
    HoverIconButton* m_arrowButton = nullptr;
    QList<TagWidget*> m_tags;
    QPointer<PopupMenu> m_popup;
    QPointer<QLabel> m_emptyLabel;

    TagStyle m_tagStyle;
    TagStyleFactory m_tagStyleFactory;
    QString m_scrollBarStyleSheet;
    int m_contentPadding = 6;
    int m_tagSpacing = 4;
    int m_popupWidth = 0;
    int m_maxVisibleItems = 8;
    bool m_searchable = true;
    bool m_clearButtonVisible = true;
    bool m_arrowVisible = true;
    bool m_iconHint = false;
    bool m_popupLayoutPending = false;
    bool m_preediting = false;
};

} // namespace animatedwidgets

#ifndef ANIMATEDWIDGETS_DESIGNER_PLUGIN
using MultiSelectComboBox = animatedwidgets::MultiSelectComboBox;
#endif
