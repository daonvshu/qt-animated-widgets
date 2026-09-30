#include "variantbuttonplugin.h"

#include "../designerwidgets.h"

#include <QPainter>

namespace {
const char* const buttonNames[] = {"FilledButton",      "OutlineButton",      "BorderedButton",  "HoverButton", "FilledIconButton",
                                   "OutlineIconButton", "BorderedIconButton", "HoverIconButton", "RadioButton", "CheckBox"};
const char* const buttonObjectNames[] = {"filledButton",      "outlineButton",      "borderedButton",  "hoverButton", "filledIconButton",
                                         "outlineIconButton", "borderedIconButton", "hoverIconButton", "radioButton", "checkBox"};
} // namespace

VariantButtonPlugin::VariantButtonPlugin(int kind, QObject* parent)
    : QObject(parent)
    , m_kind(kind)
{
}

bool VariantButtonPlugin::isContainer() const {
    return false;
}
bool VariantButtonPlugin::isInitialized() const {
    return m_initialized;
}
QIcon VariantButtonPlugin::icon() const {
    QPixmap pixmap(48, 32);
    pixmap.setDevicePixelRatio(2);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    const QColor accent(QStringLiteral("#3478f6"));
    const QColor border(QStringLiteral("#6b7280"));
    const QColor pale(QStringLiteral("#e8f0ff"));
    if (m_kind == 8) {
        painter.setPen(QPen(accent, 1.5));
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(QRectF(5, 1, 14, 14));
        painter.setPen(Qt::NoPen);
        painter.setBrush(accent);
        painter.drawEllipse(QRectF(10, 6, 4, 4));
    } else if (m_kind == 9) {
        painter.setPen(QPen(accent, 1.5));
        painter.setBrush(Qt::NoBrush);
        painter.drawRoundedRect(QRectF(5, 1, 14, 14), 2, 2);
        painter.drawLine(QPointF(8, 8), QPointF(11, 11));
        painter.drawLine(QPointF(11, 11), QPointF(16, 5));
    } else {
        const bool iconOnly = m_kind >= 4;
        const int style = m_kind % 4;
        const QRectF shape = iconOnly ? QRectF(5, 1, 14, 14) : QRectF(1, 2, 22, 12);
        painter.setPen(style == 0 || style == 3 ? QPen(Qt::NoPen) : QPen(style == 1 ? accent : border, 1.3));
        if (style == 0)
            painter.setBrush(accent);
        else if (style == 1)
            painter.setBrush(Qt::NoBrush);
        else if (style == 2)
            painter.setBrush(Qt::white);
        else
            painter.setBrush(pale);
        if (iconOnly)
            painter.drawEllipse(shape);
        else
            painter.drawRoundedRect(shape, 3, 3);

        const QColor glyphColor = style == 0 ? QColor(Qt::white) : (style == 3 ? accent : border);
        painter.setPen(QPen(glyphColor, 1.4));
        if (iconOnly) {
            painter.drawLine(QPointF(9, 8), QPointF(15, 8));
            painter.drawLine(QPointF(12, 5), QPointF(12, 11));
        } else {
            painter.drawLine(QPointF(6, 8), QPointF(18, 8));
        }
    }
    painter.end();
    return QIcon(pixmap);
}

QString VariantButtonPlugin::domXml() const {
    const QString className = name();
    const QString defaultText =
            m_kind < 4 ? QStringLiteral("Button") : (m_kind == 8 ? QStringLiteral("Radio button") : (m_kind == 9 ? QStringLiteral("Check box") : QString()));
    return QStringLiteral("<widget class=\"%1\" name=\"%2\">"
                          "<property name=\"text\"><string>%3</string></property>"
                          "</widget>")
            .arg(className, QString::fromLatin1(buttonObjectNames[m_kind]), defaultText);
}

QString VariantButtonPlugin::group() const {
    return QStringLiteral("animated-widgets");
}
QString VariantButtonPlugin::includeFile() const {
    switch (m_kind) {
        case 0:
            return QStringLiteral("animatedwidgets/buttons/filledbutton.h");
        case 1:
            return QStringLiteral("animatedwidgets/buttons/outlinebutton.h");
        case 2:
            return QStringLiteral("animatedwidgets/buttons/borderedbutton.h");
        case 3:
            return QStringLiteral("animatedwidgets/buttons/hoverbutton.h");
        case 4:
            return QStringLiteral("animatedwidgets/buttons/fillediconbutton.h");
        case 5:
            return QStringLiteral("animatedwidgets/buttons/outlineiconbutton.h");
        case 6:
            return QStringLiteral("animatedwidgets/buttons/borderediconbutton.h");
        case 7:
            return QStringLiteral("animatedwidgets/buttons/hovericonbutton.h");
        case 8:
            return QStringLiteral("animatedwidgets/buttons/radiobutton.h");
        default:
            return QStringLiteral("animatedwidgets/buttons/checkbox.h");
    }
}
QString VariantButtonPlugin::name() const {
    return QString::fromLatin1(buttonNames[m_kind]);
}
QString VariantButtonPlugin::toolTip() const {
    return QString();
}
QString VariantButtonPlugin::whatsThis() const {
    return QString();
}

QWidget* VariantButtonPlugin::createWidget(QWidget* parent) {
    switch (m_kind) {
        case 0:
            return new FilledButton(parent);
        case 1:
            return new OutlineButton(parent);
        case 2:
            return new BorderedButton(parent);
        case 3:
            return new HoverButton(parent);
        case 4:
            return new FilledIconButton(parent);
        case 5:
            return new OutlineIconButton(parent);
        case 6:
            return new BorderedIconButton(parent);
        case 7:
            return new HoverIconButton(parent);
        case 8:
            return new RadioButton(parent);
        default:
            return new CheckBox(parent);
    }
}

void VariantButtonPlugin::initialize(QDesignerFormEditorInterface*) {
    m_initialized = true;
}
