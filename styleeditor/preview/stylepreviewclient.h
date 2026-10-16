#pragma once

#include <QJsonObject>
#include <QLocalServer>
#include <QObject>
#include <QPointer>
#include <QProcess>
#include <QSize>
#include <QTimer>

class StylePreviewChannel;

class StylePreviewClient : public QObject {
    Q_OBJECT

public:
    explicit StylePreviewClient(QObject* parent = nullptr);
    ~StylePreviewClient() override;

    void request(const QJsonObject& snapshot);
    void confirmWindow(quintptr hostWindowId, const QSize& viewportSize);
    void setViewportSize(const QSize& size);
    void restart();
    void stop();

signals:
    void starting();
    void windowReady(quintptr windowId);
    void windowClosing();
    void updated(const QJsonObject& message);
    void failed(const QString& error);

private:
    void start();
    void acceptConnection();
    void receive(const QJsonObject& message);
    void reportFailure(const QString& error);

    QLocalServer m_server;
    QProcess m_process;
    QTimer m_startupTimer;
    QPointer<StylePreviewChannel> m_channel;
    QJsonObject m_snapshot;
    QString m_token;
    QSize m_viewportSize;
    quint64 m_revision = 0;
    bool m_attached = false;
    bool m_authenticated = false;
    bool m_stopping = false;
    bool m_failed = false;
};
