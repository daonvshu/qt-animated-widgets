#include "inputplugin.h"

#include "animatedwidgets/inputs/compositedoublespinbox.h"
#include "animatedwidgets/inputs/compositelineedit.h"
#include "animatedwidgets/inputs/compositeplaintextedit.h"
#include "animatedwidgets/inputs/compositespinbox.h"
#include "animatedwidgets/inputs/doublespinbox.h"
#include "animatedwidgets/inputs/iconlineedit.h"
#include "animatedwidgets/inputs/passwordlineedit.h"
#include "animatedwidgets/inputs/plaintextedit.h"
#include "animatedwidgets/inputs/spinbox.h"

#include <QPainter>
#include <QStringList>

InputPlugin::InputPlugin(int kind, QObject* parent)
    : QObject(parent)
    , m_kind(kind)
{
}
bool InputPlugin::isContainer() const {
    return false;
}
bool InputPlugin::isInitialized() const {
    return m_initialized;
}
QIcon InputPlugin::icon() const {
    QPixmap pixmap(48, 32);
    pixmap.setDevicePixelRatio(2);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(QColor(QStringLiteral("#7f8b99")), 1.2));
    painter.setBrush(Qt::white);
    painter.drawRoundedRect(QRectF(1, 2, 22, 12), 2.5, 2.5);
    painter.setPen(QPen(QColor(QStringLiteral("#3478f6")), 1.5));
    if (m_kind >= 6) {
        painter.drawLine(QPointF(5, 8), QPointF(13, 8));
        painter.drawLine(QPointF(18, 6), QPointF(20, 4));
        painter.drawLine(QPointF(18, 10), QPointF(20, 12));
        if (m_kind == 7 || m_kind == 9)
            painter.drawPoint(QPointF(15, 10));
        if (m_kind >= 8)
            painter.drawLine(QPointF(2, 14), QPointF(22, 14));
    } else if (m_kind == 4 || m_kind == 5) {
        for (int y : {5, 8, 11})
            painter.drawLine(QPointF(5, y), QPointF(y == 11 ? 13 : 18, y));
        if (m_kind == 5)
            painter.drawLine(QPointF(14, 13), QPointF(19, 13));
    } else if (m_kind == 2) {
        for (int x : {7, 12, 17})
            painter.drawPoint(QPointF(x, 8));
    } else if (m_kind == 3) {
        painter.drawLine(QPointF(5, 8), QPointF(18, 8));
        painter.drawLine(QPointF(9, 4), QPointF(9, 12));
    } else {
        painter.drawLine(QPointF(m_kind == 1 ? 10 : 5, 8), QPointF(18, 8));
        if (m_kind == 1)
            painter.drawEllipse(QRectF(4, 5, 3, 3));
    }
    return QIcon(pixmap);
}
QString InputPlugin::domXml() const {
    const QString className = name();
    const QStringList objectNames = {QStringLiteral("lineEdit"), QStringLiteral("iconLineEdit"), QStringLiteral("passwordLineEdit"),
                                     QStringLiteral("compositeLineEdit"), QStringLiteral("plainTextEdit"), QStringLiteral("compositePlainTextEdit"),
                                     QStringLiteral("spinBox"), QStringLiteral("doubleSpinBox"), QStringLiteral("compositeSpinBox"),
                                     QStringLiteral("compositeDoubleSpinBox")};
    const QString objectName = objectNames.at(m_kind);
    if (m_kind >= 6)
        return QStringLiteral("<widget class=\"%1\" name=\"%2\"/>").arg(className, objectName);
    const QString placeholder = m_kind == 2 ? QStringLiteral("Password") : QStringLiteral("Text");
    return QStringLiteral("<widget class=\"%1\" name=\"%2\">"
                          "<property name=\"placeholderText\"><string>%3</string></property>"
                          "</widget>")
            .arg(className, objectName, placeholder);
}
QString InputPlugin::group() const {
    return QStringLiteral("animated-widgets");
}
QString InputPlugin::includeFile() const {
    if (m_kind == 0)
        return QStringLiteral("animatedwidgets/inputs/lineedit.h");
    if (m_kind == 1)
        return QStringLiteral("animatedwidgets/inputs/iconlineedit.h");
    if (m_kind == 2)
        return QStringLiteral("animatedwidgets/inputs/passwordlineedit.h");
    if (m_kind == 4)
        return QStringLiteral("animatedwidgets/inputs/plaintextedit.h");
    if (m_kind == 5)
        return QStringLiteral("animatedwidgets/inputs/compositeplaintextedit.h");
    if (m_kind == 6)
        return QStringLiteral("animatedwidgets/inputs/spinbox.h");
    if (m_kind == 7)
        return QStringLiteral("animatedwidgets/inputs/doublespinbox.h");
    if (m_kind == 8)
        return QStringLiteral("animatedwidgets/inputs/compositespinbox.h");
    if (m_kind == 9)
        return QStringLiteral("animatedwidgets/inputs/compositedoublespinbox.h");
    return QStringLiteral("animatedwidgets/inputs/compositelineedit.h");
}
QString InputPlugin::name() const {
    if (m_kind == 0)
        return QStringLiteral("LineEdit");
    if (m_kind == 1)
        return QStringLiteral("IconLineEdit");
    if (m_kind == 2)
        return QStringLiteral("PasswordLineEdit");
    if (m_kind == 4)
        return QStringLiteral("PlainTextEdit");
    if (m_kind == 5)
        return QStringLiteral("CompositePlainTextEdit");
    if (m_kind == 6)
        return QStringLiteral("SpinBox");
    if (m_kind == 7)
        return QStringLiteral("DoubleSpinBox");
    if (m_kind == 8)
        return QStringLiteral("CompositeSpinBox");
    if (m_kind == 9)
        return QStringLiteral("CompositeDoubleSpinBox");
    return QStringLiteral("CompositeLineEdit");
}
QString InputPlugin::toolTip() const {
    return QString();
}
QString InputPlugin::whatsThis() const {
    return QString();
}
QWidget* InputPlugin::createWidget(QWidget* parent) {
    if (m_kind == 0)
        return new LineEdit(parent);
    if (m_kind == 1) {
        auto* input = new IconLineEdit(parent);
        input->setLeadingIcon(QIcon(QStringLiteral(":/animatedwidgets/inputs/search.svg")));
        return input;
    }
    if (m_kind == 2)
        return new PasswordLineEdit(parent);
    if (m_kind == 4)
        return new PlainTextEdit(parent);
    if (m_kind == 5)
        return new CompositePlainTextEdit(parent);
    if (m_kind == 6)
        return new SpinBox(parent);
    if (m_kind == 7)
        return new DoubleSpinBox(parent);
    if (m_kind == 8)
        return new CompositeSpinBox(parent);
    if (m_kind == 9)
        return new CompositeDoubleSpinBox(parent);
    return new CompositeLineEdit(parent);
}
void InputPlugin::initialize(QDesignerFormEditorInterface*) {
    m_initialized = true;
}
