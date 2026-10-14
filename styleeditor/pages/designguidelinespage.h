#pragma once

#include "workspacesectionpage.h"

class DesignGuidelinesPage : public WorkspaceSectionPage {
    Q_OBJECT

public:
    Q_INVOKABLE explicit DesignGuidelinesPage(const QVariant& data, QWidget* parent = nullptr);
};

Q_DECLARE_METATYPE(DesignGuidelinesPage*)
