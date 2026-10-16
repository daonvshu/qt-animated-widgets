#pragma once

#include "../document/styleresourceentry.h"
#include "../document/styleresourcesettings.h"

#include <QList>

class StyleResourceUtils {
public:
    static QList<StyleResourceEntry> scan(const StyleResourceSettings& settings, QString& error);
    static QList<StyleResourceEntry> scanAll(const StyleResourceSettings& settings, QString& error);
    static QStringList qrcSources(const StyleResourceSettings& settings);
    static bool selectQrc(StyleResourceSettings& settings, const QString& path, QString& error);
    static QString relativePath(const QString& projectDirectory, const QString& filePath);
    static QString resolve(const StyleResourceSettings& settings, const QList<StyleResourceEntry>& entries, const QString& reference);

private:
    static bool isImage(const QString& path);
};
