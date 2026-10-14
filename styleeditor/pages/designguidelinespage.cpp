#include "designguidelinespage.h"

DesignGuidelinesPage::DesignGuidelinesPage(const QVariant& data, QWidget* parent)
    : WorkspaceSectionPage(data, parent)
{
    setSectionText(QStringLiteral("Design guidelines"), QStringLiteral("Review conventions for widget colors, spacing and interaction states."));
}
