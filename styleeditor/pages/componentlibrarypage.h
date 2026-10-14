#pragma once

#include "workspacesectionpage.h"

class ComponentLibraryPage : public WorkspaceSectionPage {
    Q_OBJECT

public:
    Q_INVOKABLE explicit ComponentLibraryPage(const QVariant& data, QWidget* parent = nullptr);
};

Q_DECLARE_METATYPE(ComponentLibraryPage*)
