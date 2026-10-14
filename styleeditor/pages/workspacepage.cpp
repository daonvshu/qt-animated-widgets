#include "workspacepage.h"

WorkspacePage::WorkspacePage(const QVariant& data, QWidget* parent)
    : AbstractRouterWidget(data, parent)
{
}

void WorkspacePage::loadDocument(const StyleDocument&) {
}

void WorkspacePage::collectDocument(StyleDocument&) const {
}
