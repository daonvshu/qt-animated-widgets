#include "inputplugin.h"

#include "animatedwidgets/inputs/iconlineedit.h"
#include "animatedwidgets/inputs/passwordlineedit.h"
#include "animatedwidgets/inputs/compositelineedit.h"

#include <QPainter>

InputPlugin::InputPlugin(int kind, QObject* parent) : QObject(parent), m_kind(kind) {}
bool InputPlugin::isContainer() const { return false; }
bool InputPlugin::isInitialized() const { return m_initialized; }
QIcon InputPlugin::icon() const
{
    QPixmap pixmap(48, 32);
    pixmap.setDevicePixelRatio(2);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(QColor(QStringLiteral("#7f8b99")), 1.2));
    painter.setBrush(Qt::white);
    painter.drawRoundedRect(QRectF(1, 2, 22, 12), 2.5, 2.5);
    painter.setPen(QPen(QColor(QStringLiteral("#3478f6")), 1.5));
    if (m_kind == 2) {
        for (int x : {7, 12, 17}) painter.drawPoint(QPointF(x, 8));
    } else if (m_kind == 3) {
        painter.drawLine(QPointF(5, 8), QPointF(18, 8));
        painter.drawLine(QPointF(9, 4), QPointF(9, 12));
    } else {
        painter.drawLine(QPointF(m_kind == 1 ? 10 : 5, 8), QPointF(18, 8));
        if (m_kind == 1) painter.drawEllipse(QRectF(4, 5, 3, 3));
    }
    return QIcon(pixmap);
}
QString InputPlugin::domXml() const
{
    const QString className = name();
    const QString objectName = m_kind == 0 ? QStringLiteral("lineEdit")
        : (m_kind == 1 ? QStringLiteral("iconLineEdit")
        : (m_kind == 2 ? QStringLiteral("passwordLineEdit") : QStringLiteral("compositeLineEdit")));
    const QString placeholder = m_kind == 2 ? QStringLiteral("Password") : QStringLiteral("Text");
    return QStringLiteral("<widget class=\"%1\" name=\"%2\">"
                          "<property name=\"placeholderText\"><string>%3</string></property>"
                          "</widget>").arg(className, objectName, placeholder);
}
QString InputPlugin::group() const { return QStringLiteral("animated-widgets"); }
QString InputPlugin::includeFile() const
{
    if (m_kind == 0) return QStringLiteral("animatedwidgets/inputs/lineedit.h");
    if (m_kind == 1) return QStringLiteral("animatedwidgets/inputs/iconlineedit.h");
    if (m_kind == 2) return QStringLiteral("animatedwidgets/inputs/passwordlineedit.h");
    return QStringLiteral("animatedwidgets/inputs/compositelineedit.h");
}
QString InputPlugin::name() const
{
    if (m_kind == 0) return QStringLiteral("LineEdit");
    if (m_kind == 1) return QStringLiteral("IconLineEdit");
    if (m_kind == 2) return QStringLiteral("PasswordLineEdit");
    return QStringLiteral("CompositeLineEdit");
}
QString InputPlugin::toolTip() const { return QString(); }
QString InputPlugin::whatsThis() const { return QString(); }
QWidget* InputPlugin::createWidget(QWidget* parent)
{
    if (m_kind == 0) return new LineEdit(parent);
    if (m_kind == 1) {
        auto* input = new IconLineEdit(parent);
        input->setLeadingIcon(QIcon(QStringLiteral(":/animatedwidgets/inputs/search.svg")));
        return input;
    }
    if (m_kind == 2) return new PasswordLineEdit(parent);
    return new CompositeLineEdit(parent);
}
void InputPlugin::initialize(QDesignerFormEditorInterface*) { m_initialized = true; }
