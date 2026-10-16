#pragma once

#include "../document/styleeditoritem.h"
#include "../document/styleresourceentry.h"
#include "../plugins/stylepluginregistry.h"

#include <QObject>
#include <QPointer>
#include <memory>

class StylePreviewCanvas;
class StylePreviewChannel;
class QWindow;

class StylePreviewWorker : public QObject {
    Q_OBJECT

public:
    StylePreviewWorker(const QString& serverName, const QString& token);
    ~StylePreviewWorker() override;

private:
    void receive(const QJsonObject& message);
    void attachWindow(const QJsonObject& message);
    void resizeCanvas(const QJsonObject& message);
    void updatePreview(const QJsonObject& message);
    bool createPreview(const QJsonObject& data);
    bool updateResources(const QJsonObject& data);
    void sendError(const QJsonObject& request, const QString& error);

    StylePluginRegistry m_registry;
    std::unique_ptr<QWindow> m_hostWindow;
    std::unique_ptr<StylePreviewCanvas> m_canvas;
    StylePreviewChannel* m_channel = nullptr;
    QPointer<QWidget> m_preview;
    StyleEditorItem m_item;
    StyleResourceSettings m_resources;
    QList<StyleResourceEntry> m_resourceEntries;
    QJsonObject m_resourceData;
};
