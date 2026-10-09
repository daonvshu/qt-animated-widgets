#pragma once

#include "animatedwidgets/comboboxes/combobox.h"

namespace animatedwidgets {

class SearchComboBox : public ComboBox {
    Q_OBJECT
    Q_PROPERTY(bool highlightEnabled READ highlightEnabled WRITE setHighlightEnabled)
    Q_PROPERTY(QColor highlightColor READ highlightColor WRITE setHighlightColor)
    Q_PROPERTY(bool highlightBold READ highlightBold WRITE setHighlightBold)

public:
    explicit SearchComboBox(QWidget* parent = nullptr);

    bool highlightEnabled() const;
    void setHighlightEnabled(bool enabled);
    QColor highlightColor() const;
    void setHighlightColor(const QColor& color);
    bool highlightBold() const;
    void setHighlightBold(bool bold);

    QWidget* emptyWidget() const;
    // Takes ownership; nullptr restores the default empty-state label.
    void setEmptyWidget(QWidget* widget);

protected:
    bool event(QEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;
    void configurePopup(ComboListPopup* popup) override;
    void popupClosed() override;

private:
    void syncSearchEditor();
    void applyHighlight();
    void restoreSelectionText();

    QPointer<QLineEdit> m_searchEditor;
    QPointer<ComboListPopup> m_searchPopup;
    QPointer<QWidget> m_emptyWidget;
    QString m_query;
    QColor m_highlightColor = QColor(QStringLiteral("#1778ff"));
    bool m_highlightEnabled = true;
    bool m_highlightBold = true;
    bool m_editing = false;
};

} // namespace animatedwidgets

#ifndef ANIMATEDWIDGETS_DESIGNER_PLUGIN
using SearchComboBox = animatedwidgets::SearchComboBox;
#endif
