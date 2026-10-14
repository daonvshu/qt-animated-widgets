#pragma once

#include <QColor>
#include <QString>

#include <limits>

namespace animatedwidgets::styleeditor {

class StyleCodeExpressions {
public:
    static QString text(const QString& value) {
        QString escaped = value;
        escaped.replace(QLatin1Char('\\'), QStringLiteral("\\\\"));
        escaped.replace(QLatin1Char('"'), QStringLiteral("\\\""));
        escaped.replace(QLatin1Char('\n'), QStringLiteral("\\n"));
        escaped.replace(QLatin1Char('\r'), QStringLiteral("\\r"));
        escaped.replace(QLatin1Char('\t'), QStringLiteral("\\t"));
        escaped.replace(QChar(0), QStringLiteral("\\000"));
        return QStringLiteral("QStringLiteral(\"%1\")").arg(escaped);
    }

    static QString color(const QColor& value) {
        return value.isValid() ? QStringLiteral("QColor(\"%1\")").arg(value.name(QColor::HexArgb)) : QStringLiteral("QColor()");
    }

    static QString real(double value) {
        return QString::number(value, 'g', std::numeric_limits<double>::max_digits10);
    }

    static QString icon(const QString& resource) {
        return resource.isEmpty() ? QStringLiteral("QIcon()") : QStringLiteral("QIcon(%1)").arg(text(resource));
    }
};

} // namespace animatedwidgets::styleeditor
