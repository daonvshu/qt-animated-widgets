#include "helpdocumentationpage.h"

HelpDocumentationPage::HelpDocumentationPage(const QVariant& data, QWidget* parent)
    : WorkspaceSectionPage(data, parent)
{
    setSectionText(QStringLiteral("Help documentation"), QStringLiteral("Learn about widget properties, generated code and style editor plugins."));
}
