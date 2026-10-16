#pragma once

#include "animatedwidgets/popups/popupmenu.h"
#include <QColor>

class QVBoxLayout;

namespace animatedwidgets {

class IconLineEdit;
class PopupMenuFilterModel;

class SearchPopupMenu : public PopupMenu {
    Q_OBJECT
    Q_PROPERTY(QString searchText READ searchText WRITE setSearchText NOTIFY searchTextChanged)
    Q_PROPERTY(int searchSpacing READ searchSpacing WRITE setSearchSpacing)
    Q_PROPERTY(bool highlightEnabled READ highlightEnabled WRITE setHighlightEnabled)
    Q_PROPERTY(QColor highlightColor READ highlightColor WRITE setHighlightColor)
    Q_PROPERTY(bool highlightBold READ highlightBold WRITE setHighlightBold)

public:
    explicit SearchPopupMenu(QWidget* parent = nullptr);

    IconLineEdit* searchEditor() const;
    void setIconHint(bool enabled) override;
    PopupMenuFilterModel* filterModel() const;
    QAbstractItemModel* sourceModel() const;
    void setModel(QAbstractItemModel* model) override;

    void setPopupSize(const QSize& size) override;
    void setAdaptiveHeight(int width, int maximumVisibleItems = 0) override;

    QWidget* emptyWidget() const;
    // Takes ownership; replacing or clearing the empty widget deletes the previous widget.
    void setEmptyWidget(QWidget* widget);

    QString searchText() const;
    void setSearchText(const QString& text);

    int searchSpacing() const;
    void setSearchSpacing(int spacing);

    bool highlightEnabled() const;
    void setHighlightEnabled(bool enabled);
    QColor highlightColor() const;
    void setHighlightColor(const QColor& color);
    bool highlightBold() const;
    void setHighlightBold(bool bold);

signals:
    void searchTextChanged(const QString& text);
    void sourceItemTriggered(const QModelIndex& index);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
    bool includeListInAdaptiveHeight() const override;

private:
    void scheduleAdaptiveHeight();
    void updateEmptyState();

    QPointer<IconLineEdit> m_searchEditor;
    QPointer<QVBoxLayout> m_searchHeaderLayout;
    PopupMenuFilterModel* m_filterModel = nullptr;
    QPointer<QWidget> m_emptyWidget;

    int m_searchSpacing = 8;

    int m_adaptiveWidth = 0;
    int m_maximumVisibleItems = 0;
    bool m_heightUpdatePending = false;
};

} // namespace animatedwidgets
