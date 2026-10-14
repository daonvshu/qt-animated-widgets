#include "exportprojectpage.h"

ExportProjectPage::ExportProjectPage(const QVariant& data, QWidget* parent)
    : WorkspaceSectionPage(data, parent)
{
    setSectionText(QStringLiteral("Export project"), QStringLiteral("Export the styles in the current document as reusable C++ configurations."));
}
