#pragma once

#include <abstractrouterwidget.h>

struct StyleDocument;
class StylePopupDialog;

class WorkspacePage : public AbstractRouterWidget {
    Q_OBJECT

public:
    explicit WorkspacePage(const QVariant& data, QWidget* parent = nullptr);

    // Loading must not report user edits; collection replaces only this page's content.
    virtual void loadDocument(const StyleDocument& document);
    virtual void collectDocument(StyleDocument& document) const;

signals:
    // The window collects the page content immediately after an edit.
    void contentChanged();

    // The window tracks page dialogs along with its document dialogs.
    void dialogRequested(StylePopupDialog* dialog);
};
