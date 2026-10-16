#include "../styleeditor/utils/stylethemeutils.h"

#include <QDir>
#include <QFile>
#include <QRegularExpression>
#include <QtTest>

#include <algorithm>

class StyleThemeUtilsTest : public QObject {
    Q_OBJECT

private slots:
    void bundledThemesLoad();
    void themeFilesMatch();
    void themesContainOnlyColorsAndIcons();
    void scrollBarStylesAreScopedAndComplete();
    void unknownThemesAreRejected();
};

void StyleThemeUtilsTest::bundledThemesLoad() {
    QString error;
    const QString light = StyleThemeUtils::loadStyleSheet(QStringLiteral("light"), error);
    QVERIFY2(error.isEmpty(), qPrintable(error));
    QVERIFY(!light.isEmpty());
    const QString dark = StyleThemeUtils::loadStyleSheet(QStringLiteral("dark"), error);
    QVERIFY2(error.isEmpty(), qPrintable(error));
    QVERIFY(!dark.isEmpty());
    QVERIFY(light != dark);
    QVERIFY(light.contains(QStringLiteral("qproperty-surface:")));
    QVERIFY(dark.contains(QStringLiteral("qproperty-surface:")));

    const int common = light.indexOf(QStringLiteral("width: 8px;"));
    const int colors = light.indexOf(QStringLiteral("qproperty-backgroundNormal:"));
    QVERIFY(common >= 0);
    QVERIFY(colors > common);
}

void StyleThemeUtilsTest::themeFilesMatch() {
    const QStringList light = QDir(QStringLiteral(":/styleeditor/styles/light")).entryList({QStringLiteral("*.css")}, QDir::Files, QDir::Name);
    const QStringList dark = QDir(QStringLiteral(":/styleeditor/styles/dark")).entryList({QStringLiteral("*.css")}, QDir::Files, QDir::Name);
    QVERIFY(!light.isEmpty());
    QCOMPARE(light.size(), dark.size());
    QVERIFY(std::equal(light.cbegin(), light.cend(), dark.cbegin()));
}

void StyleThemeUtilsTest::themesContainOnlyColorsAndIcons() {
    const QRegularExpression declarations(QStringLiteral("^\\s*([\\w-]+)\\s*:\\s*([^;{}]+);"), QRegularExpression::MultilineOption);
    const QRegularExpression colors(QStringLiteral("^(#[0-9a-fA-F]{3,8}|white|black|transparent|rgba?\\([^)]*\\))$"));
    for (const QString& theme : {QStringLiteral("light"), QStringLiteral("dark")}) {
        const QDir directory(QStringLiteral(":/styleeditor/styles/") + theme);
        for (const QString& path : directory.entryList({QStringLiteral("*.css")}, QDir::Files)) {
            QFile file(directory.filePath(path));
            QVERIFY(file.open(QIODevice::ReadOnly));
            const QString styleSheet = QString::fromUtf8(file.readAll());
            auto matches = declarations.globalMatch(styleSheet);
            while (matches.hasNext()) {
                const auto declaration = matches.next();
                const QString name = declaration.captured(1);
                const QString value = declaration.captured(2).trimmed();
                if (name == QStringLiteral("image")) {
                    const QString prefix = QStringLiteral("url(:/styleeditor/styles/%1/").arg(theme);
                    QVERIFY2(value.startsWith(prefix) && value.endsWith(QStringLiteral(".svg)")), qPrintable(value));
                    QVERIFY2(QFile::exists(value.mid(4, value.size() - 5)), qPrintable(value));
                } else {
                    QVERIFY2(colors.match(value).hasMatch(), qPrintable(path + QLatin1Char(':') + name));
                }
            }
        }
    }
}

void StyleThemeUtilsTest::scrollBarStylesAreScopedAndComplete() {
    for (const QString& theme : {QStringLiteral("light"), QStringLiteral("dark")}) {
        QString error;
        const QString styleSheet = StyleThemeUtils::loadScrollBarStyleSheet(theme, error);
        QVERIFY2(error.isEmpty(), qPrintable(error));
        QVERIFY(styleSheet.contains(QStringLiteral("QScrollBar:vertical")));
        QVERIFY(styleSheet.contains(QStringLiteral("QScrollBar:horizontal")));
        QVERIFY(styleSheet.contains(QStringLiteral("::handle:vertical")));
        QVERIFY(styleSheet.contains(QStringLiteral("::handle:horizontal")));
    }
}

void StyleThemeUtilsTest::unknownThemesAreRejected() {
    for (const QString& theme : {QString(), QStringLiteral("../light"), QStringLiteral("LIGHT"), QStringLiteral("custom")}) {
        QString error;
        QVERIFY(StyleThemeUtils::loadStyleSheet(theme, error).isEmpty());
        QVERIFY(!error.isEmpty());
        QVERIFY(StyleThemeUtils::loadScrollBarStyleSheet(theme, error).isEmpty());
        QVERIFY(!error.isEmpty());
    }
}

QTEST_GUILESS_MAIN(StyleThemeUtilsTest)

#include "tststylethemeutils.moc"
