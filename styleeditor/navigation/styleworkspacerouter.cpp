#include "styleworkspacerouter.h"

#include "../pages/componentlibrarypage.h"
#include "../pages/designguidelinespage.h"
#include "../pages/exportprojectpage.h"
#include "../pages/helpdocumentationpage.h"
#include "../pages/styleeditorpage.h"
#include "../pages/thememanagerpage.h"
#include "../plugins/stylepluginregistry.h"

#include <qrouter.h>
#include <QStackedWidget>

StyleWorkspaceRouter::StyleWorkspaceRouter(QStackedWidget* container, StylePluginRegistry* registry, QObject* parent)
    : QObject(parent)
    , m_registry(registry)
    , m_container(container)
{
    qRegisterMetaType<StyleEditorPage*>();
    qRegisterMetaType<ComponentLibraryPage*>();
    qRegisterMetaType<ThemeManagerPage*>();
    qRegisterMetaType<ExportProjectPage*>();
    qRegisterMetaType<DesignGuidelinesPage*>();
    qRegisterMetaType<HelpDocumentationPage*>();

    QRouter::install(container, m_contextId);
}

void StyleWorkspaceRouter::navigate(Section section) {
    QByteArray pageClassName;
    switch (section) {
        case Section::StyleEditor:
            pageClassName = StyleEditorPage::staticMetaObject.className();
            break;
        case Section::ComponentLibrary:
            pageClassName = ComponentLibraryPage::staticMetaObject.className();
            break;
        case Section::ThemeManager:
            pageClassName = ThemeManagerPage::staticMetaObject.className();
            break;
        case Section::ExportProject:
            pageClassName = ExportProjectPage::staticMetaObject.className();
            break;
        case Section::DesignGuidelines:
            pageClassName = DesignGuidelinesPage::staticMetaObject.className();
            break;
        case Section::HelpDocumentation:
            pageClassName = HelpDocumentationPage::staticMetaObject.className();
            break;
    }

    const QList<WorkspacePage*> existingPages = pages();
    auto& router = QRouter::of(m_contextId);
    router.pushOrMove2Top(pageClassName, QVariant::fromValue(static_cast<QObject*>(m_registry)));
    for (WorkspacePage* page : pages()) {
        if (!existingPages.contains(page))
            emit pageCreated(page);
    }

    if (section == Section::StyleEditor) {
        emit sectionChanged(QStringLiteral("Custom widget styles"), QStringLiteral("Adjust properties visually and generate style code in real time."));
    } else if (section == Section::ThemeManager) {
        emit sectionChanged(QStringLiteral("Theme manager"), QStringLiteral("Choose the appearance of the editor workspace."));
    } else if (const auto* page = qobject_cast<WorkspaceSectionPage*>(router.current())) {
        emit sectionChanged(page->title(), page->description());
    }
}

WorkspacePage* StyleWorkspaceRouter::currentPage() const {
    return m_container ? qobject_cast<WorkspacePage*>(m_container->currentWidget()) : nullptr;
}

QList<WorkspacePage*> StyleWorkspaceRouter::pages() const {
    QList<WorkspacePage*> result;
    if (!m_container)
        return result;

    for (int index = 0; index < m_container->count(); ++index) {
        if (auto* page = qobject_cast<WorkspacePage*>(m_container->widget(index)))
            result.append(page);
    }
    return result;
}
