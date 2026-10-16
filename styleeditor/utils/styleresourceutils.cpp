#include "styleresourceutils.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSet>
#include <QXmlStreamReader>

#include <algorithm>

QString StyleResourceUtils::relativePath(const QString& projectDirectory, const QString& filePath) {
    if (filePath.isEmpty() || filePath.startsWith(QStringLiteral(":")))
        return filePath;
    if (projectDirectory.isEmpty() && QDir::isAbsolutePath(filePath))
        return {};
    const QString relative = QDir::cleanPath(QDir::fromNativeSeparators(QDir::isAbsolutePath(filePath)
        ? QDir(projectDirectory).relativeFilePath(filePath) : filePath));
    if (QDir::isAbsolutePath(relative) || relative == QStringLiteral("..") || relative.startsWith(QStringLiteral("../")))
        return {};
    return QDir::cleanPath(relative);
}

bool StyleResourceUtils::isImage(const QString& path) {
    const QString suffix = QFileInfo(path).suffix().toLower();
    return suffix == QStringLiteral("svg") || suffix == QStringLiteral("png") || suffix == QStringLiteral("ico")
        || suffix == QStringLiteral("jpg") || suffix == QStringLiteral("jpeg") || suffix == QStringLiteral("bmp")
        || suffix == QStringLiteral("gif") || suffix == QStringLiteral("webp");
}

QList<StyleResourceEntry> StyleResourceUtils::scan(const StyleResourceSettings& settings, QString& error) {
    error.clear();
    QList<StyleResourceEntry> entries;
    if (settings.source.isEmpty()) {
        error = QStringLiteral("Choose a QRC file.");
        return entries;
    }

    const QString source = QDir::cleanPath(QDir(settings.projectDirectory).absoluteFilePath(settings.source));
    QFile file(source);
    if (QFileInfo(source).suffix().compare(QStringLiteral("qrc"), Qt::CaseInsensitive) != 0 || !file.open(QIODevice::ReadOnly)) {
        error = QStringLiteral("The resource source is not a readable QRC file.");
        return entries;
    }

    QXmlStreamReader xml(&file);
    QSet<QString> seen;
    QString prefix;
    while (!xml.atEnd()) {
        xml.readNext();
        if (!xml.isStartElement())
            continue;
        if (xml.name() == QLatin1String("qresource")) {
            prefix = xml.attributes().value(QStringLiteral("prefix")).toString();
            if (!prefix.startsWith(QLatin1Char('/')))
                prefix.prepend(QLatin1Char('/'));
            prefix = QDir::cleanPath(prefix);
        } else if (xml.name() == QLatin1String("file")) {
            const QString alias = xml.attributes().value(QStringLiteral("alias")).toString();
            const QString path = xml.readElementText().trimmed();
            const QString filePath = QDir(QFileInfo(source).absolutePath()).absoluteFilePath(path);
            QString resourcePath = QDir::cleanPath(prefix + QLatin1Char('/') + (alias.isEmpty() ? path : alias));
            while (resourcePath.startsWith(QLatin1Char('/')))
                resourcePath.remove(0, 1);
            const QString reference = QStringLiteral(":/") + resourcePath;
            if (isImage(path) && QFileInfo(filePath).isFile() && !seen.contains(reference)) {
                entries.append({reference, filePath, prefix});
                seen.insert(reference);
            }
        }
    }
    if (xml.hasError()) {
        error = QStringLiteral("Invalid QRC: %1").arg(xml.errorString());
        return {};
    }

    std::sort(entries.begin(), entries.end(), [](const StyleResourceEntry& left, const StyleResourceEntry& right) {
        return left.reference < right.reference;
    });
    return entries;
}

QString StyleResourceUtils::resolve(const StyleResourceSettings& settings, const QList<StyleResourceEntry>& entries, const QString& reference) {
    if (reference.isEmpty())
        return {};
    for (const StyleResourceEntry& entry : entries) {
        if (entry.reference == reference)
            return entry.filePath;
    }
    if (reference.startsWith(QStringLiteral(":")) || QDir::isAbsolutePath(reference) || settings.projectDirectory.isEmpty())
        return reference;
    return QDir(settings.projectDirectory).absoluteFilePath(reference);
}

QStringList StyleResourceUtils::qrcSources(const StyleResourceSettings& settings) {
    QStringList sources = settings.qrcFiles;
    sources.prepend(settings.source);
    QStringList result;
    for (const QString& source : sources) {
        if (source.isEmpty() || source.startsWith(QLatin1Char(':')))
            continue;
        const QString relative = QDir::cleanPath(QDir::fromNativeSeparators(
            QDir::isAbsolutePath(source) && !settings.projectDirectory.isEmpty()
                ? QDir(settings.projectDirectory).relativeFilePath(source) : source));
        if (QFileInfo(relative).suffix().compare(QStringLiteral("qrc"), Qt::CaseInsensitive) == 0
            && !result.contains(relative))
            result.append(relative);
    }
    return result;
}

bool StyleResourceUtils::selectQrc(StyleResourceSettings& settings, const QString& path, QString& error) {
    error.clear();
    if (path.isEmpty())
        return false;

    StyleResourceSettings selected = settings;
    selected.qrcFiles = qrcSources(settings);
    if (selected.projectDirectory.isEmpty())
        selected.projectDirectory = QFileInfo(path).absolutePath();
    selected.source = QDir::cleanPath(QDir::fromNativeSeparators(QDir(selected.projectDirectory).relativeFilePath(path)));
    if (path.startsWith(QLatin1Char(':'))
        || QFileInfo(selected.source).suffix().compare(QStringLiteral("qrc"), Qt::CaseInsensitive) != 0) {
        error = QStringLiteral("Choose a QRC file.");
        return false;
    }
    scan(selected, error);
    if (!error.isEmpty())
        return false;

    selected.qrcFiles = qrcSources(selected);
    settings = selected;
    return true;
}

QList<StyleResourceEntry> StyleResourceUtils::scanAll(const StyleResourceSettings& settings, QString& error) {
    QList<StyleResourceEntry> entries = scan(settings, error);
    QSet<QString> seen;
    for (const StyleResourceEntry& entry : entries)
        seen.insert(entry.reference);

    const QString active = QDir::cleanPath(QDir::fromNativeSeparators(settings.source));
    for (const QString& source : qrcSources(settings)) {
        if (source == active)
            continue;
        StyleResourceSettings other = settings;
        other.source = source;
        QString sourceError;
        const auto additional = scan(other, sourceError);
        for (const StyleResourceEntry& entry : additional) {
            if (!seen.contains(entry.reference)) {
                entries.append(entry);
                seen.insert(entry.reference);
            }
        }
    }
    return entries;
}
