#pragma once

#include "workspacesectionpage.h"

class ExportProjectPage : public WorkspaceSectionPage {
    Q_OBJECT

public:
    Q_INVOKABLE explicit ExportProjectPage(const QVariant& data, QWidget* parent = nullptr);
};

Q_DECLARE_METATYPE(ExportProjectPage*)
