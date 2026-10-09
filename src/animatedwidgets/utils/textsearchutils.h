#pragma once

#include <QColor>
#include <QString>
#include <QVector>

namespace animatedwidgets {

class TextSearchUtils {
public:
    static bool matches(const QString& text, const QString& keyword);
    static QString searchAndHighlight(const QString& text, const QString& keyword,
                                      const QColor& color, bool bold, bool* matched = nullptr);

private:
    static QVector<int> matchIndexes(const QString& text, const QString& keyword);
};

} // namespace animatedwidgets
