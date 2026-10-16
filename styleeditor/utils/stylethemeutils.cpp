#include "stylethemeutils.h"

#include <QDir>
#include <QFile>

QString StyleThemeUtils::loadStyleSheet(const QString& theme, QString& error) {
    error.clear();
    if (theme != QStringLiteral("light") && theme != QStringLiteral("dark")) {
        error = QStringLiteral("Unknown editor theme: %1").arg(theme);
        return {};
    }

    QString styleSheet;
    for (const QString& directory : {QStringLiteral("comm"), theme}) {
        const QDir source(QStringLiteral(":/styleeditor/styles/") + directory);
        const QStringList files = source.entryList({QStringLiteral("*.css")}, QDir::Files, QDir::Name);
        if (files.isEmpty()) {
            error = QStringLiteral("No stylesheets found for %1.").arg(directory);
            return {};
        }
        for (const QString& file : files) {
            if (!appendFile(source.filePath(file), styleSheet, error))
                return {};
        }
    }
    return styleSheet;
}

QString StyleThemeUtils::loadScrollBarStyleSheet(const QString& theme, QString& error) {
    error.clear();
    if (theme != QStringLiteral("light") && theme != QStringLiteral("dark")) {
        error = QStringLiteral("Unknown editor theme: %1").arg(theme);
        return {};
    }

    QString styleSheet;
    for (const QString& directory : {QStringLiteral("comm"), theme}) {
        if (!appendFile(QStringLiteral(":/styleeditor/styles/%1/scrollbar.css").arg(directory), styleSheet, error))
            return {};
    }
    return styleSheet;
}

bool StyleThemeUtils::appendFile(const QString& path, QString& styleSheet, QString& error) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        error = QStringLiteral("Cannot read stylesheet %1: %2").arg(path, file.errorString());
        return false;
    }

    const QByteArray content = file.readAll();
    if (file.error() != QFile::NoError || content.trimmed().isEmpty()) {
        error = QStringLiteral("Cannot load stylesheet %1.").arg(path);
        return false;
    }
    styleSheet += QString::fromUtf8(content);
    styleSheet += QLatin1Char('\n');
    return true;
}
