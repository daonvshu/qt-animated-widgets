#include "comboboxplugin.h"

#include "../designerwidgets.h"

#include <QPainter>

ComboBoxPlugin::ComboBoxPlugin(int kind, QObject* parent)
    : QObject(parent)
    , m_kind(kind)
{
}

bool ComboBoxPlugin::isContainer() const {
    return false;
}

bool ComboBoxPlugin::isInitialized() const {
    return m_initialized;
}

QIcon ComboBoxPlugin::icon() const {
    QPixmap pixmap(48, 32);
    pixmap.setDevicePixelRatio(2);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(QColor(QStringLiteral("#7f8b99")), 1.2));
    painter.setBrush(Qt::white);
    painter.drawRoundedRect(QRectF(1, 2, 22, 12), 2.5, 2.5);
    painter.setPen(QPen(QColor(QStringLiteral("#3478f6")), 1.5));
    painter.drawLine(QPointF(5, 8), QPointF(13, 8));
    painter.drawLine(QPointF(16, 6), QPointF(19, 9));
    painter.drawLine(QPointF(19, 9), QPointF(22, 6));
    if (m_kind == 1)
        painter.drawLine(QPointF(2, 14), QPointF(22, 14));
    if (m_kind == 2) {
        painter.drawEllipse(QPointF(8, 8), 2, 2);
        painter.drawLine(QPointF(9.5, 9.5), QPointF(12, 12));
    }
    return QIcon(pixmap);
}

QString ComboBoxPlugin::domXml() const {
    return QStringLiteral("<widget class=\"%1\" name=\"%2\"/>")
            .arg(name(), m_kind == 0 ? QStringLiteral("comboBox")
                : m_kind == 1 ? QStringLiteral("compositeComboBox") : QStringLiteral("searchComboBox"));
}

QString ComboBoxPlugin::group() const {
    return QStringLiteral("animated-widgets");
}

QString ComboBoxPlugin::includeFile() const {
    if (m_kind == 2)
        return QStringLiteral("animatedwidgets/comboboxes/searchcombobox.h");
    return m_kind == 0 ? QStringLiteral("animatedwidgets/comboboxes/combobox.h")
                       : QStringLiteral("animatedwidgets/comboboxes/compositecombobox.h");
}

QString ComboBoxPlugin::name() const {
    if (m_kind == 2)
        return QStringLiteral("SearchComboBox");
    return m_kind == 0 ? QStringLiteral("ComboBox") : QStringLiteral("CompositeComboBox");
}

QString ComboBoxPlugin::toolTip() const {
    return QString();
}

QString ComboBoxPlugin::whatsThis() const {
    return QString();
}

QWidget* ComboBoxPlugin::createWidget(QWidget* parent) {
    if (m_kind == 0)
        return new ComboBox(parent);
    if (m_kind == 2)
        return new SearchComboBox(parent);
    return new CompositeComboBox(parent);
}

void ComboBoxPlugin::initialize(QDesignerFormEditorInterface*) {
    m_initialized = true;
}
