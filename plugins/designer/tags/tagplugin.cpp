#include "tagplugin.h"

#include "../designerwidgets.h"

#include <QPainter>

TagPlugin::TagPlugin(QObject* parent)
    : QObject(parent)
{
}

bool TagPlugin::isContainer() const {
    return false;
}

bool TagPlugin::isInitialized() const {
    return m_initialized;
}

QIcon TagPlugin::icon() const {
    QPixmap pixmap(48, 32);
    pixmap.setDevicePixelRatio(2);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(QStringLiteral("#e3f3ff")));
    painter.drawRoundedRect(QRectF(1, 3, 22, 10), 2, 2);
    painter.setPen(QPen(QColor(QStringLiteral("#3478f6")), 1.2));
    painter.drawLine(QPointF(4, 8), QPointF(12, 8));
    painter.drawLine(QPointF(16, 6), QPointF(20, 10));
    painter.drawLine(QPointF(20, 6), QPointF(16, 10));
    return QIcon(pixmap);
}

QString TagPlugin::domXml() const {
    return QStringLiteral("<widget class=\"TagWidget\" name=\"tagWidget\"><property name=\"text\"><string>Tag</string></property></widget>");
}

QString TagPlugin::group() const {
    return QStringLiteral("animated-widgets");
}

QString TagPlugin::includeFile() const {
    return QStringLiteral("animatedwidgets/tags/tagwidget.h");
}

QString TagPlugin::name() const {
    return QStringLiteral("TagWidget");
}

QString TagPlugin::toolTip() const {
    return QString();
}

QString TagPlugin::whatsThis() const {
    return QString();
}

QWidget* TagPlugin::createWidget(QWidget* parent) {
    return new TagWidget(parent);
}

void TagPlugin::initialize(QDesignerFormEditorInterface*) {
    m_initialized = true;
}
