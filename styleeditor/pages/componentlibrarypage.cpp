#include "componentlibrarypage.h"

ComponentLibraryPage::ComponentLibraryPage(const QVariant& data, QWidget* parent)
    : WorkspaceSectionPage(data, parent)
{
    setSectionText(QStringLiteral("Component library"), QStringLiteral("Browse widget types supplied by plugins and reusable style presets."));
}
