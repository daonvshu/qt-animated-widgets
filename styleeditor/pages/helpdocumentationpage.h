#pragma once

#include "workspacesectionpage.h"

class HelpDocumentationPage : public WorkspaceSectionPage {
    Q_OBJECT

public:
    Q_INVOKABLE explicit HelpDocumentationPage(const QVariant& data, QWidget* parent = nullptr);
};

Q_DECLARE_METATYPE(HelpDocumentationPage*)
