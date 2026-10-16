#include "../styleeditor/preview/stylepreviewchannel.h"
#include "../styleeditor/preview/stylepreviewprotocol.h"
#include "../styleeditor/document/styleeditorsession.h"
#include "../styleeditor/plugins/stylepluginregistry.h"

#include <QColor>
#include <QJsonDocument>
#include <QLocalServer>
#include <QLocalSocket>
#include <QUuid>
#include <QtEndian>
#include <QtTest>

using animatedwidgets::styleeditor::StyleProperty;
using animatedwidgets::styleeditor::StylePropertyType;

class StylePreviewProtocolTest : public QObject {
    Q_OBJECT

private slots:
    void fragmentedAndCombinedFrames();
    void malformedFramesAreRejected();
    void propertyMetadataRoundTrip();
    void invalidMetadataPreservesOutput();
    void localSocketRoundTrip();
    void sessionPreservesDocumentWhilePreviewIsPending();

private:
    static QByteArray rawFrame(const QByteArray& payload);
};

QByteArray StylePreviewProtocolTest::rawFrame(const QByteArray& payload) {
    QByteArray frame(4, '\0');
    qToBigEndian<quint32>(static_cast<quint32>(payload.size()), reinterpret_cast<uchar*>(frame.data()));
    return frame + payload;
}

void StylePreviewProtocolTest::fragmentedAndCombinedFrames() {
    const QJsonObject first{{QStringLiteral("type"), QStringLiteral("preview")}, {QStringLiteral("revision"), QStringLiteral("1")}};
    const QJsonObject second{{QStringLiteral("type"), QStringLiteral("updated")}, {QStringLiteral("revision"), QStringLiteral("2")}};
    const QByteArray encoded = StylePreviewProtocol::encode(first);
    QByteArray buffer;
    QList<QJsonObject> messages;
    QString error;
    for (int index = 0; index < encoded.size() - 1; ++index) {
        buffer.append(encoded.at(index));
        QVERIFY(StylePreviewProtocol::takeMessages(buffer, messages, error));
        QVERIFY(error.isEmpty());
        QVERIFY(messages.isEmpty());
    }
    buffer.append(encoded.back());
    buffer.append(StylePreviewProtocol::encode(second));
    buffer.append(encoded.left(2));
    QVERIFY(StylePreviewProtocol::takeMessages(buffer, messages, error));
    QCOMPARE(messages.size(), 2);
    QCOMPARE(messages.at(0).value(QStringLiteral("revision")).toString(), QStringLiteral("1"));
    QCOMPARE(messages.at(1).value(QStringLiteral("revision")).toString(), QStringLiteral("2"));
    QCOMPARE(buffer.size(), 2);
}

void StylePreviewProtocolTest::malformedFramesAreRejected() {
    QList<QByteArray> invalid;
    invalid.append(rawFrame("not json"));
    invalid.append(rawFrame("[]"));
    invalid.append(rawFrame("{\"version\":99}"));
    invalid.append(rawFrame("{}"));
    invalid.append(QByteArray(4, '\0'));
    QByteArray oversized(4, '\0');
    qToBigEndian<quint32>(StylePreviewProtocol::MaximumFrameSize + 1, reinterpret_cast<uchar*>(oversized.data()));
    invalid.append(oversized);
    for (QByteArray buffer : invalid) {
        QList<QJsonObject> messages;
        QString error;
        QVERIFY(!StylePreviewProtocol::takeMessages(buffer, messages, error));
        QVERIFY(!error.isEmpty());
        QVERIFY(messages.isEmpty());
    }
}

void StylePreviewProtocolTest::propertyMetadataRoundTrip() {
    QList<StyleProperty> original;
    for (const auto type : {StylePropertyType::Text, StylePropertyType::Color, StylePropertyType::Integer, StylePropertyType::Real,
                           StylePropertyType::Boolean, StylePropertyType::Enum, StylePropertyType::Alignment, StylePropertyType::Icon,
                           StylePropertyType::Size, StylePropertyType::KeySequence}) {
        StyleProperty property;
        property.name = QString::number(static_cast<int>(type));
        property.group = QStringLiteral("Content");
        property.label = QStringLiteral("Example");
        property.description = QStringLiteral("Editable property");
        property.type = type;
        property.enumNames = QStringList{QStringLiteral("Left"), QStringLiteral("Right")};
        property.enumExpressions = QStringList{QStringLiteral("Qt::AlignLeft"), QStringLiteral("Qt::AlignRight")};
        switch (type) {
            case StylePropertyType::Color:
                property.value = QColor(20, 40, 60, 0);
                property.defaultValue = QColor();
                break;
            case StylePropertyType::Size:
                property.value = QSize(24, 32);
                property.defaultValue = QSize();
                break;
            case StylePropertyType::Boolean:
                property.value = true;
                property.defaultValue = false;
                break;
            case StylePropertyType::Integer:
                property.value = 300;
                property.defaultValue = 100;
                break;
            case StylePropertyType::Real:
                property.value = 1.25;
                property.defaultValue = 0.5;
                break;
            default:
                property.value = QStringLiteral("Right");
                property.defaultValue = QStringLiteral("Left");
                break;
        }
        property.enabledWhen.append({QStringLiteral("enabled"), true});
        property.apply = [](const QVariant&) {};
        property.codeExpression = [](const QVariant&) { return QStringLiteral("example"); };
        original.append(property);
    }

    const QJsonArray encoded = StylePreviewProtocol::encodeProperties(original);
    const QJsonDocument document = QJsonDocument::fromJson(QJsonDocument(encoded).toJson());
    QList<StyleProperty> decoded;
    QVERIFY(StylePreviewProtocol::decodeProperties(document.array(), decoded));
    QCOMPARE(decoded.size(), original.size());
    for (int index = 0; index < original.size(); ++index) {
        const StyleProperty& before = original.at(index);
        const StyleProperty& after = decoded.at(index);
        QCOMPARE(after.name, before.name);
        QCOMPARE(after.group, before.group);
        QCOMPARE(after.label, before.label);
        QCOMPARE(after.description, before.description);
        QCOMPARE(after.type, before.type);
        QCOMPARE(after.value, before.value);
        QCOMPARE(after.defaultValue, before.defaultValue);
        QCOMPARE(after.enumNames.size(), before.enumNames.size());
        for (int option = 0; option < before.enumNames.size(); ++option) {
            QCOMPARE(after.enumNames.at(option), before.enumNames.at(option));
            QCOMPARE(after.enumExpressions.at(option), before.enumExpressions.at(option));
        }
        QCOMPARE(after.enabledWhen.size(), 1);
        QCOMPARE(after.enabledWhen.first().propertyName, QStringLiteral("enabled"));
        QCOMPARE(after.enabledWhen.first().value.toBool(), true);
        QVERIFY(!after.apply);
        QVERIFY(!after.codeExpression);
    }
}

void StylePreviewProtocolTest::invalidMetadataPreservesOutput() {
    StyleProperty property;
    property.name = QStringLiteral("text");
    property.value = QStringLiteral("Example");
    const QJsonObject encoded = StylePreviewProtocol::encodeProperties({property}).first().toObject();
    QList<StyleProperty> output{property};
    QJsonObject invalid = encoded;
    invalid.insert(QStringLiteral("type"), 999);
    QVERIFY(!StylePreviewProtocol::decodeProperties({invalid}, output));
    QCOMPARE(output.size(), 1);
    QCOMPARE(output.first().name, property.name);
    QVERIFY(!StylePreviewProtocol::decodeProperties({encoded, encoded}, output));
    QCOMPARE(output.size(), 1);
}

void StylePreviewProtocolTest::localSocketRoundTrip() {
    QLocalServer server;
    server.setSocketOptions(QLocalServer::UserAccessOption);
    const QString name = QStringLiteral("style-preview-test-") + QUuid::createUuid().toString(QUuid::WithoutBraces);
    QVERIFY(server.listen(name));
    StylePreviewChannel client(new QLocalSocket);
    QSignalSpy clientMessages(&client, &StylePreviewChannel::messageReceived);
    QSignalSpy clientErrors(&client, &StylePreviewChannel::failed);
    client.socket()->connectToServer(name);
    QTRY_VERIFY(server.hasPendingConnections());
    QTRY_COMPARE(client.socket()->state(), QLocalSocket::ConnectedState);
    StylePreviewChannel peer(server.nextPendingConnection());
    QSignalSpy peerMessages(&peer, &StylePreviewChannel::messageReceived);
    QSignalSpy peerErrors(&peer, &StylePreviewChannel::failed);

    QVERIFY(client.send({{QStringLiteral("type"), QStringLiteral("preview")}, {QStringLiteral("revision"), QStringLiteral("7")}}));
    QTRY_COMPARE(peerMessages.count(), 1);
    const QJsonObject request = qvariant_cast<QJsonObject>(peerMessages.first().first());
    QCOMPARE(request.value(QStringLiteral("revision")).toString(), QStringLiteral("7"));
    QVERIFY(peer.send({{QStringLiteral("type"), QStringLiteral("updated")}, {QStringLiteral("revision"), request.value(QStringLiteral("revision"))}}));
    QTRY_COMPARE(clientMessages.count(), 1);
    QCOMPARE(qvariant_cast<QJsonObject>(clientMessages.first().first()).value(QStringLiteral("type")).toString(), QStringLiteral("updated"));
    QCOMPARE(clientErrors.count(), 0);
    QCOMPARE(peerErrors.count(), 0);
}

void StylePreviewProtocolTest::sessionPreservesDocumentWhilePreviewIsPending() {
    StylePluginRegistry registry;
    StyleEditorSession session(registry);
    StyleDocumentItem item;
    item.id = QStringLiteral("primary");
    item.widgetTypeId = QStringLiteral("example.button");
    item.displayName = QStringLiteral("Primary");
    item.serializedProperties.insert(QStringLiteral("text"), QStringLiteral("Original"));
    item.serializedProperties.insert(QStringLiteral("extensionValue"), 42);
    session.loadItems({item});
    QCOMPARE(session.collectItems().first().serializedProperties.value(QStringLiteral("text")).toString(), QStringLiteral("Original"));

    StyleProperty text;
    text.name = QStringLiteral("text");
    text.value = QStringLiteral("Original");
    text.defaultValue = QStringLiteral("Default");
    QSignalSpy changes(&session, &StyleEditorSession::contentChanged);
    session.updatePreviewResult(0, {text}, QStringLiteral("generated"));
    QCOMPARE(changes.count(), 0);
    QCOMPARE(session.items().first().generatedCode, QStringLiteral("generated"));
    session.setPropertyValue(0, QStringLiteral("text"), QStringLiteral("Edited"));
    QVERIFY(session.items().first().generatedCode.isEmpty());
    const QJsonObject stored = session.collectItems().first().serializedProperties;
    QCOMPARE(stored.value(QStringLiteral("text")).toString(), QStringLiteral("Edited"));
    QCOMPARE(stored.value(QStringLiteral("extensionValue")).toInt(), 42);
    session.resetProperties(0);
    QCOMPARE(session.collectItems().first().serializedProperties.value(QStringLiteral("text")).toString(), QStringLiteral("Default"));

    const int before = changes.count();
    StylePreviewSettings settings;
    settings.widthAuto = true;
    session.setPreviewSettings(0, settings);
    QCOMPARE(changes.count(), before + 1);
    QVERIFY(session.collectItems().first().previewSettings.widthAuto);
    session.setPreviewSettings(0, settings);
    QCOMPARE(changes.count(), before + 1);
}

QTEST_GUILESS_MAIN(StylePreviewProtocolTest)

#include "tststylepreviewprotocol.moc"
