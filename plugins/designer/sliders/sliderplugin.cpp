#include "sliderplugin.h"

#include "animatedwidgets/sliders/slider.h"

#include <QPainter>

SliderPlugin::SliderPlugin(QObject* parent)
    : QObject(parent)
{
}
bool SliderPlugin::isContainer() const {
    return false;
}
bool SliderPlugin::isInitialized() const {
    return m_initialized;
}
QIcon SliderPlugin::icon() const {
    QPixmap pixmap(48, 32);
    pixmap.setDevicePixelRatio(2);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(QColor(QStringLiteral("#87919e")), 2));
    painter.drawLine(QPointF(2, 8), QPointF(22, 8));
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(QStringLiteral("#3478f6")));
    painter.drawEllipse(QRectF(12, 3, 10, 10));
    return QIcon(pixmap);
}
QString SliderPlugin::domXml() const {
    return QStringLiteral("<widget class=\"Slider\" name=\"slider\">"
                          "<property name=\"orientation\"><enum>Qt::Horizontal</enum></property>"
                          "<property name=\"value\"><number>35</number></property>"
                          "</widget>");
}
QString SliderPlugin::group() const {
    return QStringLiteral("animated-widgets");
}
QString SliderPlugin::includeFile() const {
    return QStringLiteral("animatedwidgets/sliders/slider.h");
}
QString SliderPlugin::name() const {
    return QStringLiteral("Slider");
}
QString SliderPlugin::toolTip() const {
    return QString();
}
QString SliderPlugin::whatsThis() const {
    return QString();
}
QWidget* SliderPlugin::createWidget(QWidget* parent) {
    return new Slider(parent);
}
void SliderPlugin::initialize(QDesignerFormEditorInterface*) {
    m_initialized = true;
}
