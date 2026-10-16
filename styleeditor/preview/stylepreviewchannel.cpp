#include "stylepreviewchannel.h"

#include "stylepreviewprotocol.h"

#include <QLocalSocket>
#include <QPointer>
#include <QTimer>

StylePreviewChannel::StylePreviewChannel(QLocalSocket* socket, QObject* parent)
    : QObject(parent)
    , m_socket(socket)
{
    socket->setParent(this);
    socket->setReadBufferSize(StylePreviewProtocol::MaximumFrameSize + 4);
    connect(socket, &QLocalSocket::readyRead, this, &StylePreviewChannel::readMessages);
    QTimer::singleShot(0, this, &StylePreviewChannel::readMessages);
}

QLocalSocket* StylePreviewChannel::socket() const {
    return m_socket;
}

bool StylePreviewChannel::send(const QJsonObject& message) {
    const QByteArray frame = StylePreviewProtocol::encode(message);
    if (frame.isEmpty() || m_socket->state() != QLocalSocket::ConnectedState || m_socket->write(frame) != frame.size()) {
        emit failed(QStringLiteral("Could not send the preview message."));
        return false;
    }
    return true;
}

void StylePreviewChannel::readMessages() {
    m_buffer.append(m_socket->readAll());
    QList<QJsonObject> messages;
    QString error;
    if (!StylePreviewProtocol::takeMessages(m_buffer, messages, error)) {
        m_buffer.clear();
        emit failed(error);
        return;
    }
    const QPointer<StylePreviewChannel> guard(this);
    for (const QJsonObject& message : messages) {
        emit messageReceived(message);
        if (!guard)
            return;
    }
}
