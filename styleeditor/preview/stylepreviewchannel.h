#pragma once

#include <QByteArray>
#include <QJsonObject>
#include <QObject>

class QLocalSocket;

class StylePreviewChannel : public QObject {
    Q_OBJECT

public:
    explicit StylePreviewChannel(QLocalSocket* socket, QObject* parent = nullptr);

    QLocalSocket* socket() const;
    bool send(const QJsonObject& message);

signals:
    void messageReceived(const QJsonObject& message);
    void failed(const QString& error);

private:
    void readMessages();

    QLocalSocket* m_socket;
    QByteArray m_buffer;
};
