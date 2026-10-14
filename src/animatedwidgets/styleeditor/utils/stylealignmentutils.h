#pragma once

#include "animatedwidgets/styleeditor/api/styleenumoption.h"

#include <QList>
#include <Qt>

namespace animatedwidgets::styleeditor {

class StyleAlignmentUtils {
public:
    static QString horizontal(const QString& value) {
        return value == QStringLiteral("AlignCenter") ? QStringLiteral("AlignHCenter") : value.section(QLatin1Char('|'), 0, 0);
    }

    static QString vertical(const QString& value) {
        return value == QStringLiteral("AlignCenter") ? QStringLiteral("AlignVCenter") : value.section(QLatin1Char('|'), 1, 1);
    }

    static QString combine(const QString& horizontal, const QString& vertical) {
        if (horizontal == QStringLiteral("AlignHCenter") && vertical == QStringLiteral("AlignVCenter"))
            return QStringLiteral("AlignCenter");
        return horizontal + QLatin1Char('|') + vertical;
    }

    static QList<StyleEnumOption> options() {
        return {
            {QStringLiteral("AlignLeft|AlignTop"), Qt::AlignLeft | Qt::AlignTop, QStringLiteral("Qt::AlignLeft | Qt::AlignTop")},
            {QStringLiteral("AlignHCenter|AlignTop"), Qt::AlignHCenter | Qt::AlignTop, QStringLiteral("Qt::AlignHCenter | Qt::AlignTop")},
            {QStringLiteral("AlignRight|AlignTop"), Qt::AlignRight | Qt::AlignTop, QStringLiteral("Qt::AlignRight | Qt::AlignTop")},
            {QStringLiteral("AlignLeft|AlignVCenter"), Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("Qt::AlignLeft | Qt::AlignVCenter")},
            {QStringLiteral("AlignCenter"), Qt::AlignCenter, QStringLiteral("Qt::AlignCenter")},
            {QStringLiteral("AlignRight|AlignVCenter"), Qt::AlignRight | Qt::AlignVCenter, QStringLiteral("Qt::AlignRight | Qt::AlignVCenter")},
            {QStringLiteral("AlignLeft|AlignBottom"), Qt::AlignLeft | Qt::AlignBottom, QStringLiteral("Qt::AlignLeft | Qt::AlignBottom")},
            {QStringLiteral("AlignHCenter|AlignBottom"), Qt::AlignHCenter | Qt::AlignBottom, QStringLiteral("Qt::AlignHCenter | Qt::AlignBottom")},
            {QStringLiteral("AlignRight|AlignBottom"), Qt::AlignRight | Qt::AlignBottom, QStringLiteral("Qt::AlignRight | Qt::AlignBottom")}
        };
    }
};

} // namespace animatedwidgets::styleeditor
