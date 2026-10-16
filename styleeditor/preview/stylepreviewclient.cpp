#include "stylepreviewclient.h"

#include "stylepreviewchannel.h"

#include <QCoreApplication>
#include <QLocalSocket>
#include <QUuid>

StylePreviewClient::StylePreviewClient(QObject* parent)
    : QObject(parent)
{
    m_server.setSocketOptions(QLocalServer::UserAccessOption);
    m_startupTimer.setSingleShot(true);
    m_startupTimer.setInterval(15000);
    connect(&m_server, &QLocalServer::newConnection, this, &StylePreviewClient::acceptConnection);
    connect(&m_startupTimer, &QTimer::timeout, this, [this] {
        reportFailure(QStringLiteral("The preview process did not become ready. You can reload the preview."));
    });
    connect(&m_process, &QProcess::errorOccurred, this, [this](QProcess::ProcessError error) {
        if (error == QProcess::FailedToStart)
            reportFailure(QStringLiteral("Could not start the preview process: %1").arg(m_process.errorString()));
    });
    connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [this](int, QProcess::ExitStatus) {
        reportFailure(QStringLiteral("The preview process stopped. Your document is preserved; reload to continue."));
    });
    connect(&m_process, &QProcess::readyReadStandardOutput, this, [this] { m_process.readAllStandardOutput(); });
    connect(&m_process, &QProcess::readyReadStandardError, this, [this] { m_process.readAllStandardError(); });
}

StylePreviewClient::~StylePreviewClient() {
    stop();
}

void StylePreviewClient::request(const QJsonObject& snapshot) {
    m_snapshot = snapshot;
    m_snapshot.insert(QStringLiteral("type"), QStringLiteral("preview"));
    m_snapshot.insert(QStringLiteral("revision"), QString::number(++m_revision));
    if (m_attached && m_channel)
        m_channel->send(m_snapshot);
    else if (!m_failed && m_process.state() == QProcess::NotRunning && !m_snapshot.value(QStringLiteral("item")).toObject().isEmpty())
        start();
}

void StylePreviewClient::start() {
    m_failed = false;
    m_token = QUuid::createUuid().toString(QUuid::WithoutBraces);
    const QString serverName = QStringLiteral("animatedwidgets-preview-") + QUuid::createUuid().toString(QUuid::WithoutBraces);
    if (!m_server.listen(serverName)) {
        reportFailure(QStringLiteral("Could not create the local preview connection: %1").arg(m_server.errorString()));
        return;
    }

    emit starting();
    m_process.setProgram(QCoreApplication::applicationFilePath());
    m_process.setArguments({QStringLiteral("--style-preview"), serverName, m_token});
    m_startupTimer.start();
    m_process.start();
}

void StylePreviewClient::acceptConnection() {
    while (m_server.hasPendingConnections()) {
        QLocalSocket* socket = m_server.nextPendingConnection();
        if (m_channel) {
            socket->abort();
            socket->deleteLater();
            continue;
        }
        m_channel = new StylePreviewChannel(socket, this);
        connect(m_channel, &StylePreviewChannel::messageReceived, this, &StylePreviewClient::receive);
        connect(m_channel, &StylePreviewChannel::failed, this, &StylePreviewClient::reportFailure);
        connect(socket, &QLocalSocket::disconnected, this, [this] {
            reportFailure(QStringLiteral("The preview connection was lost. Reload to reconnect."));
        });
    }
}

void StylePreviewClient::receive(const QJsonObject& message) {
    if (m_stopping || m_failed)
        return;
    const QString type = message.value(QStringLiteral("type")).toString();
    if (!m_authenticated) {
        if (type != QStringLiteral("hello") || message.value(QStringLiteral("token")).toString() != m_token) {
            reportFailure(QStringLiteral("The preview process handshake failed."));
            return;
        }
        bool valid = false;
        const qulonglong id = message.value(QStringLiteral("windowId")).toString().toULongLong(&valid);
        if (!valid || id == 0 || static_cast<qulonglong>(static_cast<quintptr>(id)) != id) {
            reportFailure(QStringLiteral("The preview process returned an invalid native window."));
            return;
        }
        m_authenticated = true;
        m_startupTimer.stop();
        emit windowReady(static_cast<quintptr>(id));
        return;
    }
    if (message.value(QStringLiteral("revision")) != m_snapshot.value(QStringLiteral("revision")))
        return;
    if (type == QStringLiteral("error"))
        emit failed(message.value(QStringLiteral("message")).toString());
    else if (type == QStringLiteral("updated"))
        emit updated(message);
}

void StylePreviewClient::confirmWindow(quintptr hostWindowId, const QSize& viewportSize) {
    if (!m_authenticated || !m_channel || hostWindowId == 0 || viewportSize.isEmpty())
        return;
    m_attached = true;
    m_viewportSize = viewportSize;
    QJsonObject attached;
    attached.insert(QStringLiteral("type"), QStringLiteral("attached"));
    attached.insert(QStringLiteral("revision"), m_snapshot.value(QStringLiteral("revision")));
    attached.insert(QStringLiteral("hostWindowId"), QString::number(static_cast<qulonglong>(hostWindowId)));
    attached.insert(QStringLiteral("width"), viewportSize.width());
    attached.insert(QStringLiteral("height"), viewportSize.height());
    if (m_channel->send(attached) && !m_snapshot.isEmpty())
        m_channel->send(m_snapshot);
}

void StylePreviewClient::setViewportSize(const QSize& size) {
    if (size.isEmpty() || size == m_viewportSize)
        return;
    m_viewportSize = size;
    if (!m_attached || !m_channel || m_stopping || m_failed)
        return;

    QJsonObject message;
    message.insert(QStringLiteral("type"), QStringLiteral("viewport"));
    message.insert(QStringLiteral("width"), size.width());
    message.insert(QStringLiteral("height"), size.height());
    m_channel->send(message);
}

void StylePreviewClient::reportFailure(const QString& error) {
    if (m_stopping || m_failed)
        return;
    m_failed = true;
    m_startupTimer.stop();
    m_attached = false;
    emit windowClosing();
    emit failed(error);
}

void StylePreviewClient::restart() {
    stop();
    start();
}

void StylePreviewClient::stop() {
    m_stopping = true;
    m_startupTimer.stop();
    emit windowClosing();
    if (m_channel) {
        QJsonObject shutdown;
        shutdown.insert(QStringLiteral("type"), QStringLiteral("shutdown"));
        m_channel->send(shutdown);
        m_channel->socket()->flush();
    }
    if (m_process.state() != QProcess::NotRunning && !m_process.waitForFinished(1000)) {
        m_process.terminate();
        if (!m_process.waitForFinished(500)) {
            m_process.kill();
            m_process.waitForFinished(1000);
        }
    }
    delete m_channel.data();
    m_channel = nullptr;
    m_server.close();
    m_authenticated = false;
    m_attached = false;
    m_stopping = false;
}
