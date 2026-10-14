#pragma once

#include "workspacesectionpage.h"

class ThemeManagerPage : public WorkspaceSectionPage {
    Q_OBJECT

public:
    Q_INVOKABLE explicit ThemeManagerPage(const QVariant& data, QWidget* parent = nullptr);
};

Q_DECLARE_METATYPE(ThemeManagerPage*)
