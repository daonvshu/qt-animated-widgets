#include "stylepreviewworker.h"

#include "stylepreviewcanvas.h"
#include "stylepreviewchannel.h"
#include "stylepreviewprotocol.h"
#include "../utils/stylecodegenerator.h"
#include "../utils/styledocumentcodec.h"
#include "../utils/stylepreviewutils.h"
#include "../utils/styleresourceutils.h"

#include <QApplication>
#include <QLayout>
#include <QLocalSocket>
#include <QTimer>
#include <QWindow>

using animatedwidgets::styleeditor::StyleProperty;
using animatedwidgets::styleeditor::StylePropertyType;

StylePreviewWorker::StylePreviewWorker(const QString& serverName, const QString& token)
    : m_canvas(std::make_unique<StylePreviewCanvas>())
{
    m_registry.load(QApplication::applicationDirPath() + QStringLiteral("/plugins/styleeditor"));
    auto* socket = new QLocalSocket;
    m_channel = new StylePreviewChannel(socket, this);
    connect(socket, &QLocalSocket::connected, this, [this, token] {
        QJsonObject hello;
        hello.insert(QStringLiteral("type"), QStringLiteral("hello"));
        hello.insert(QStringLiteral("token"), token);
        hello.insert(QStringLiteral("windowId"), QString::number(static_cast<qulonglong>(m_canvas->winId())));
        m_channel->send(hello);
    });
    connect(socket, &QLocalSocket::disconnected, qApp, &QApplication::quit);
    connect(m_channel, &StylePreviewChannel::failed, qApp, &QApplication::quit);
    connect(m_channel, &StylePreviewChannel::messageReceived, this, &StylePreviewWorker::receive);
    QTimer::singleShot(15000, this, [socket] {
        if (socket->state() != QLocalSocket::ConnectedState)
            QApplication::quit();
    });
    socket->connectToServer(serverName);
}

StylePreviewWorker::~StylePreviewWorker() = default;

void StylePreviewWorker::receive(const QJsonObject& message) {
    const QString type = message.value(QStringLiteral("type")).toString();
    if (type == QStringLiteral("shutdown"))
        QApplication::quit();
    else if (type == QStringLiteral("attached"))
        attachWindow(message);
    else if (type == QStringLiteral("viewport"))
        resizeCanvas(message);
    else if (type == QStringLiteral("preview"))
        updatePreview(message);
}

void StylePreviewWorker::attachWindow(const QJsonObject& message) {
    if (m_hostWindow)
        return;

    bool valid = false;
    const qulonglong id = message.value(QStringLiteral("hostWindowId")).toString().toULongLong(&valid);
    if (!valid || id == 0 || static_cast<qulonglong>(static_cast<quintptr>(id)) != id
        || message.value(QStringLiteral("width")).toInt() <= 0 || message.value(QStringLiteral("height")).toInt() <= 0) {
        sendError(message, QStringLiteral("The editor returned an invalid preview host."));
        return;
    }

    m_hostWindow.reset(QWindow::fromWinId(static_cast<WId>(id)));
    if (!m_hostWindow) {
        sendError(message, QStringLiteral("This platform could not attach the preview to its host."));
        return;
    }

    // Keep the owning process aware of native parenting and size before the first QWidget show.
    m_canvas->windowHandle()->setParent(m_hostWindow.get());
    resizeCanvas(message);
}

void StylePreviewWorker::resizeCanvas(const QJsonObject& message) {
    if (!m_hostWindow)
        return;
    const QSize size(message.value(QStringLiteral("width")).toInt(), message.value(QStringLiteral("height")).toInt());
    if (!size.isEmpty())
        m_canvas->setGeometry(QRect(QPoint(), size));
}

bool StylePreviewWorker::createPreview(const QJsonObject& data) {
    const QString typeId = data.value(QStringLiteral("typeId")).toString();
    const auto* type = m_registry.findType(typeId);
    if (!type || !type->create || !type->properties)
        return false;

    delete m_preview.data();
    m_item = {};
    m_item.id = data.value(QStringLiteral("id")).toString();
    m_item.widgetTypeId = typeId;
    m_item.typeName = type->typeName;
    m_item.suggestedSize = type->suggestedSize;
    m_preview = type->create(m_canvas.get());
    if (!m_preview)
        return false;
    m_item.properties = type->properties(m_preview);
    m_canvas->addPreview(m_preview);
    return true;
}

bool StylePreviewWorker::updateResources(const QJsonObject& data) {
    if (data == m_resourceData)
        return false;
    m_resourceData = data;
    m_resources.projectDirectory = data.value(QStringLiteral("directory")).toString();
    m_resources.source = data.value(QStringLiteral("source")).toString();
    m_resources.qrcFiles.clear();
    for (const QJsonValue& file : data.value(QStringLiteral("qrcFiles")).toArray())
        m_resources.qrcFiles.append(file.toString());
    QString error;
    m_resourceEntries = StyleResourceUtils::scanAll(m_resources, error);
    return true;
}

void StylePreviewWorker::updatePreview(const QJsonObject& message) {
    if (!m_hostWindow) {
        sendError(message, QStringLiteral("The preview window has not been attached to its host."));
        return;
    }
    m_canvas->setBackground(QColor(message.value(QStringLiteral("background")).toString()));
    const QJsonObject data = message.value(QStringLiteral("item")).toObject();
    if (data.isEmpty()) {
        delete m_preview.data();
        m_item = {};
        return;
    }
    const bool created = !m_preview || m_item.id != data.value(QStringLiteral("id")).toString()
        || m_item.widgetTypeId != data.value(QStringLiteral("typeId")).toString();
    if (created && !createPreview(data)) {
        sendError(message, QStringLiteral("The selected plugin could not create its preview widget."));
        return;
    }
    m_item.displayName = data.value(QStringLiteral("name")).toString();
    const bool resourcesChanged = updateResources(message.value(QStringLiteral("resources")).toObject());

    QList<StyleProperty> values = m_item.properties;
    for (StyleProperty& property : values)
        property.apply = {};
    QList<StyleProperty> inherited;
    if (!StylePreviewProtocol::decodeProperties(data.value(QStringLiteral("inheritedProperties")).toArray(), inherited)) {
        sendError(message, QStringLiteral("Could not transfer compatible properties to the new widget type."));
        return;
    }
    StyleDocumentCodec::applyCompatibleProperties(values, inherited);
    if (inherited.isEmpty())
        StyleDocumentCodec::applySerializedProperties(values, data.value(QStringLiteral("properties")).toObject());
    for (int index = 0; index < values.size(); ++index) {
        StyleProperty& property = m_item.properties[index];
        const bool icon = property.type == StylePropertyType::Icon;
        if (!created && property.value == values.at(index).value && !(icon && resourcesChanged))
            continue;
        property.value = values.at(index).value;
        if (property.apply) {
            const QVariant value = icon ? QVariant(StyleResourceUtils::resolve(m_resources, m_resourceEntries, property.value.toString())) : property.value;
            property.apply(value);
        }
    }
    StylePreviewUtils::applyState(m_preview, message.value(QStringLiteral("state")).toInt(), m_item.properties);

    const QJsonObject settings = message.value(QStringLiteral("settings")).toObject();
    const bool widthAuto = settings.value(QStringLiteral("widthAuto")).toBool();
    const bool heightAuto = settings.value(QStringLiteral("heightAuto")).toBool();
    const QSize hint = m_preview->sizeHint();
    const QSize size = StylePreviewUtils::resolveSize(QSize(settings.value(QStringLiteral("width")).toInt(), settings.value(QStringLiteral("height")).toInt()),
                                                     m_item.suggestedSize, hint, widthAuto, heightAuto);
    m_preview->setMinimumSize(0, 0);
    m_preview->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    m_preview->setSizePolicy(widthAuto ? QSizePolicy::Preferred : QSizePolicy::Fixed, heightAuto ? QSizePolicy::Preferred : QSizePolicy::Fixed);
    m_preview->resize(size);
    if (!widthAuto)
        m_preview->setFixedWidth(size.width());
    if (!heightAuto)
        m_preview->setFixedHeight(size.height());
    m_preview->show();
    m_canvas->layout()->activate();
    m_canvas->show();

    QJsonObject reply;
    reply.insert(QStringLiteral("type"), QStringLiteral("updated"));
    reply.insert(QStringLiteral("revision"), message.value(QStringLiteral("revision")));
    reply.insert(QStringLiteral("itemId"), m_item.id);
    reply.insert(QStringLiteral("width"), size.width());
    reply.insert(QStringLiteral("height"), size.height());
    reply.insert(QStringLiteral("hintWidth"), hint.width());
    reply.insert(QStringLiteral("hintHeight"), hint.height());
    reply.insert(QStringLiteral("properties"), StylePreviewProtocol::encodeProperties(m_item.properties));
    reply.insert(QStringLiteral("code"), StyleCodeGenerator::generate(m_item, m_resources.projectDirectory));
    m_channel->send(reply);
}

void StylePreviewWorker::sendError(const QJsonObject& request, const QString& error) {
    QJsonObject reply;
    reply.insert(QStringLiteral("type"), QStringLiteral("error"));
    reply.insert(QStringLiteral("revision"), request.value(QStringLiteral("revision")));
    reply.insert(QStringLiteral("message"), error);
    m_channel->send(reply);
}
