#include "variantbuttonplugin.h"

#include "animatedwidgets/buttons/variantbuttons.h"

namespace {
const char* const buttonNames[] = {
    "FilledButton", "OutlineButton", "BorderedButton", "HoverButton",
    "FilledIconButton", "OutlineIconButton", "BorderedIconButton", "HoverIconButton"
};
const char* const buttonObjectNames[] = {
    "filledButton", "outlineButton", "borderedButton", "hoverButton",
    "filledIconButton", "outlineIconButton", "borderedIconButton", "hoverIconButton"
};
}

VariantButtonPlugin::VariantButtonPlugin(int kind, QObject* parent)
    : QObject(parent), m_kind(kind)
{
}

bool VariantButtonPlugin::isContainer() const { return false; }
bool VariantButtonPlugin::isInitialized() const { return m_initialized; }
QIcon VariantButtonPlugin::icon() const { return QIcon(); }

QString VariantButtonPlugin::domXml() const
{
    const QString className = name();
    return QStringLiteral("<widget class=\"%1\" name=\"%2\">"
                          "<property name=\"text\"><string>%3</string></property>"
                          "</widget>")
        .arg(className, QString::fromLatin1(buttonObjectNames[m_kind]),
             m_kind < 4 ? QStringLiteral("Button") : QString());
}

QString VariantButtonPlugin::group() const { return QStringLiteral("animated-widgets"); }
QString VariantButtonPlugin::includeFile() const
{
    switch (m_kind) {
    case 0: return QStringLiteral("animatedwidgets/buttons/filledbutton.h");
    case 1: return QStringLiteral("animatedwidgets/buttons/outlinebutton.h");
    case 2: return QStringLiteral("animatedwidgets/buttons/borderedbutton.h");
    case 3: return QStringLiteral("animatedwidgets/buttons/hoverbutton.h");
    case 4: return QStringLiteral("animatedwidgets/buttons/fillediconbutton.h");
    case 5: return QStringLiteral("animatedwidgets/buttons/outlineiconbutton.h");
    case 6: return QStringLiteral("animatedwidgets/buttons/borderediconbutton.h");
    default: return QStringLiteral("animatedwidgets/buttons/hovericonbutton.h");
    }
}
QString VariantButtonPlugin::name() const
{
    return QStringLiteral("animatedwidgets::") + QString::fromLatin1(buttonNames[m_kind]);
}
QString VariantButtonPlugin::toolTip() const { return QString(); }
QString VariantButtonPlugin::whatsThis() const { return QString(); }

QWidget* VariantButtonPlugin::createWidget(QWidget* parent)
{
    switch (m_kind) {
    case 0: return new animatedwidgets::FilledButton(parent);
    case 1: return new animatedwidgets::OutlineButton(parent);
    case 2: return new animatedwidgets::BorderedButton(parent);
    case 3: return new animatedwidgets::HoverButton(parent);
    case 4: return new animatedwidgets::FilledIconButton(parent);
    case 5: return new animatedwidgets::OutlineIconButton(parent);
    case 6: return new animatedwidgets::BorderedIconButton(parent);
    default: return new animatedwidgets::HoverIconButton(parent);
    }
}

void VariantButtonPlugin::initialize(QDesignerFormEditorInterface*)
{
    m_initialized = true;
}
