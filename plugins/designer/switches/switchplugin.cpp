#include "switchplugin.h"

#include "../designerwidgets.h"

#include <QPainter>

SwitchPlugin::SwitchPlugin(QObject* parent)
    : QObject(parent)
{
}
bool SwitchPlugin::isContainer() const {
    return false;
}
bool SwitchPlugin::isInitialized() const {
    return m_initialized;
}
QIcon SwitchPlugin::icon() const {
    QPixmap pixmap(48, 32);
    pixmap.setDevicePixelRatio(2);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(QStringLiteral("#3478f6")));
    painter.drawRoundedRect(QRectF(1, 2, 22, 12), 6, 6);
    painter.setBrush(Qt::white);
    painter.drawEllipse(QRectF(12, 3, 10, 10));
    painter.end();
    return QIcon(pixmap);
}
QString SwitchPlugin::domXml() const {
    return QStringLiteral("<widget class=\"aw::Switch\" name=\"switchWidget\">"
                          "<property name=\"text\"><string>Switch</string></property>"
                          "</widget>");
}
QString SwitchPlugin::group() const {
    return QStringLiteral("animated-widgets");
}
QString SwitchPlugin::includeFile() const {
    return QStringLiteral("animatedwidgets/switches/switch.h");
}
QString SwitchPlugin::name() const {
    return QStringLiteral("aw::Switch");
}
QString SwitchPlugin::toolTip() const {
    return QString();
}
QString SwitchPlugin::whatsThis() const {
    return QString();
}
QWidget* SwitchPlugin::createWidget(QWidget* parent) {
    return new aw::Switch(parent);
}
void SwitchPlugin::initialize(QDesignerFormEditorInterface*) {
    m_initialized = true;
}
