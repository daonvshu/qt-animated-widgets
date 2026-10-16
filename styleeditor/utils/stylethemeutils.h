#pragma once

#include <QString>

class StyleThemeUtils {
public:
    static QString loadStyleSheet(const QString& theme, QString& error);
    static QString loadScrollBarStyleSheet(const QString& theme, QString& error);

private:
    static bool appendFile(const QString& path, QString& styleSheet, QString& error);
};
