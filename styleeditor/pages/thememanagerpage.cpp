#include "thememanagerpage.h"

ThemeManagerPage::ThemeManagerPage(const QVariant& data, QWidget* parent)
    : WorkspaceSectionPage(data, parent)
{
    setSectionText(QStringLiteral("Theme manager"), QStringLiteral("Manage shared colors, fonts, spacing and other theme values."));
}
