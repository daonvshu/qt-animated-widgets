#include "calendarplugin.h"

#include "../designerwidgets.h"

#include <QPainter>

CalendarPlugin::CalendarPlugin(int kind, QObject* parent)
    : QObject(parent)
    , m_kind(kind)
{
}

bool CalendarPlugin::isContainer() const {
    return false;
}

bool CalendarPlugin::isInitialized() const {
    return m_initialized;
}

QIcon CalendarPlugin::icon() const {
    QPixmap pixmap(48, 32);
    pixmap.setDevicePixelRatio(2);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(QColor(QStringLiteral("#3478f6")), 1.5));
    painter.setBrush(QColor(QStringLiteral("#f2f3f5")));
    painter.drawRoundedRect(QRectF(3, 3, 18, 11), 2, 2);
    painter.drawLine(QPointF(3, 6), QPointF(21, 6));
    painter.drawLine(QPointF(7, 1), QPointF(7, 4));
    painter.drawLine(QPointF(17, 1), QPointF(17, 4));
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(QStringLiteral("#3478f6")));
    painter.drawEllipse(QRectF(10, 8, 3, 3));
    return QIcon(pixmap);
}

QString CalendarPlugin::domXml() const {
    return QStringLiteral("<widget class=\"%1\" name=\"calendarSelector\"/>").arg(name());
}

QString CalendarPlugin::group() const {
    return QStringLiteral("animated-widgets");
}

QString CalendarPlugin::includeFile() const {
    switch (m_kind) {
    case 1: return QStringLiteral("animatedwidgets/calendars/calendaryearselector.h");
    case 2: return QStringLiteral("animatedwidgets/calendars/calendarmonthselector.h");
    case 3: return QStringLiteral("animatedwidgets/calendars/calendartimeselector.h");
    default: return QStringLiteral("animatedwidgets/calendars/calendardateselector.h");
    }
}

QString CalendarPlugin::name() const {
    switch (m_kind) {
    case 1: return QStringLiteral("CalendarYearSelector");
    case 2: return QStringLiteral("CalendarMonthSelector");
    case 3: return QStringLiteral("CalendarTimeSelector");
    default: return QStringLiteral("CalendarDateSelector");
    }
}

QString CalendarPlugin::toolTip() const {
    return name();
}

QString CalendarPlugin::whatsThis() const {
    return QString();
}

QWidget* CalendarPlugin::createWidget(QWidget* parent) {
    switch (m_kind) {
    case 1: return new CalendarYearSelector(parent);
    case 2: return new CalendarMonthSelector(parent);
    case 3: return new CalendarTimeSelector(parent);
    default: return new CalendarDateSelector(parent);
    }
}

void CalendarPlugin::initialize(QDesignerFormEditorInterface*) {
    m_initialized = true;
}
