#pragma once

#include <QObject>
#include <QList>
#include <QPointer>

class QStackedWidget;

class StylePluginRegistry;
class WorkspacePage;

class StyleWorkspaceRouter : public QObject {
    Q_OBJECT

public:
    enum class Section {
        StyleEditor,
        ComponentLibrary,
        ThemeManager,
        ExportProject,
        DesignGuidelines,
        HelpDocumentation
    };
    Q_ENUM(Section)

    explicit StyleWorkspaceRouter(QStackedWidget* container, StylePluginRegistry* registry, QObject* parent = nullptr);

    void navigate(Section section);
    WorkspacePage* currentPage() const;
    QList<WorkspacePage*> pages() const;

signals:
    void pageCreated(WorkspacePage* page);
    void sectionChanged(const QString& title, const QString& description);

private:
    StylePluginRegistry* m_registry = nullptr;
    QPointer<QStackedWidget> m_container;
    int m_contextId = 0;
};
