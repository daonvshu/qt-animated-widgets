#include "progressbarplugin.h"

#include "../designerwidgets.h"

#include <QPainter>

ProgressBarPlugin::ProgressBarPlugin(QObject* parent)
    : QObject(parent)
{
}
bool ProgressBarPlugin::isContainer() const {
    return false;
}
bool ProgressBarPlugin::isInitialized() const {
    return m_initialized;
}
QIcon ProgressBarPlugin::icon() const {
    QPixmap pixmap(48, 32);
    pixmap.setDevicePixelRatio(2);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(QStringLiteral("#d4d9e0")));
    painter.drawRoundedRect(QRectF(1, 5, 22, 6), 3, 3);
    painter.setBrush(QColor(QStringLiteral("#3478f6")));
    painter.drawRoundedRect(QRectF(1, 5, 14, 6), 3, 3);
    return QIcon(pixmap);
}
QString ProgressBarPlugin::domXml() const {
    return QStringLiteral("<widget class=\"aw::ProgressBar\" name=\"progressBar\">"
                          "<property name=\"value\"><number>35</number></property>"
                          "</widget>");
}
QString ProgressBarPlugin::group() const {
    return QStringLiteral("animated-widgets");
}
QString ProgressBarPlugin::includeFile() const {
    return QStringLiteral("animatedwidgets/progressbars/progressbar.h");
}
QString ProgressBarPlugin::name() const {
    return QStringLiteral("aw::ProgressBar");
}
QString ProgressBarPlugin::toolTip() const {
    return QString();
}
QString ProgressBarPlugin::whatsThis() const {
    return QString();
}
QWidget* ProgressBarPlugin::createWidget(QWidget* parent) {
    return new aw::ProgressBar(parent);
}
void ProgressBarPlugin::initialize(QDesignerFormEditorInterface*) {
    m_initialized = true;
}
