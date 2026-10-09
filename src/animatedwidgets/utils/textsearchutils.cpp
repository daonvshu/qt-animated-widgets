#include "animatedwidgets/utils/textsearchutils.h"

namespace animatedwidgets {

QVector<int> TextSearchUtils::matchIndexes(const QString& text, const QString& keyword) {
    QVector<int> indexes;
    int position = 0;
    for (const QChar character : keyword) {
        bool found = false;
        while (position < text.size()) {
            if (QString(text.at(position)).compare(QString(character), Qt::CaseInsensitive) == 0) {
                indexes.append(position++);
                found = true;
                break;
            }
            ++position;
        }
        if (!found)
            return {};
    }
    return indexes;
}

bool TextSearchUtils::matches(const QString& text, const QString& keyword) {
    return keyword.isEmpty() || !matchIndexes(text, keyword).isEmpty();
}

QString TextSearchUtils::searchAndHighlight(const QString& text, const QString& keyword,
                                          const QColor& color, bool bold, bool* matched) {
    const QVector<int> indexes = matchIndexes(text, keyword);
    const bool found = keyword.isEmpty() || !indexes.isEmpty();
    if (matched)
        *matched = found;
    if (!found || keyword.isEmpty())
        return text.toHtmlEscaped();

    QString result;
    int hit = 0;
    for (int position = 0; position < text.size(); ++position) {
        const QString character = QString(text.at(position)).toHtmlEscaped();
        if (hit < indexes.size() && position == indexes.at(hit)) {
            result += QStringLiteral("<span style='color:%1;font-weight:%2;'>%3</span>")
                          .arg(color.name(), bold ? QStringLiteral("bold") : QStringLiteral("normal"), character);
            ++hit;
        } else {
            result += character;
        }
    }
    return result;
}

} // namespace animatedwidgets
