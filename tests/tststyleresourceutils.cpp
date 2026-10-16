#include "../styleeditor/utils/styleresourceutils.h"
#include "../styleeditor/utils/stylecodegenerator.h"
#include "../styleeditor/utils/styledocumentcodec.h"
#include "../styleeditor/document/styledocumentsession.h"
#include "animatedwidgets/styleeditor/utils/stylecodeexpressions.h"

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QTemporaryDir>
#include <QtTest>

class StyleResourceUtilsTest : public QObject {
    Q_OBJECT

private slots:
    void relativeReferencesAndCode();
    void qrcAliasesAndPreviewResolution();
    void malformedAndMissingSources();
    void documentSettingsRoundTrip();
    void qrcHistoryAndSelection();
    void multipleQrcPreviewResolution();
    void independentQrcDirectories();
    void legacyQrcSettings();

private:
    static bool writeFile(const QString& path, const QByteArray& contents);
};

bool StyleResourceUtilsTest::writeFile(const QString& path, const QByteArray& contents) {
    QFile file(path);
    return file.open(QIODevice::WriteOnly) && file.write(contents) == contents.size();
}

void StyleResourceUtilsTest::relativeReferencesAndCode() {
    QTemporaryDir project;
    QVERIFY(project.isValid());
    QVERIFY(QDir(project.path()).mkpath(QStringLiteral("resources/nested")));
    const QString image = project.filePath(QStringLiteral("resources/nested/open.svg"));
    QVERIFY(writeFile(image, "<svg xmlns=\"http://www.w3.org/2000/svg\"/>"));

    const StyleResourceSettings settings{project.path(), {}, {}};
    const QString reference = QStringLiteral("resources/nested/open.svg");
    QCOMPARE(StyleResourceUtils::resolve(settings, {}, reference), image);
    QCOMPARE(StyleResourceUtils::relativePath(project.path(), image), reference);
    QCOMPARE(StyleResourceUtils::relativePath(project.path(), reference), reference);
    QVERIFY(StyleResourceUtils::relativePath(project.path(), QStringLiteral("../outside.svg")).isEmpty());
    QVERIFY(StyleResourceUtils::relativePath({}, image).isEmpty());
    QVERIFY(StyleResourceUtils::resolve(settings, {}, {}).isEmpty());

    animatedwidgets::styleeditor::StyleProperty icon;
    icon.name = QStringLiteral("icon");
    icon.type = animatedwidgets::styleeditor::StylePropertyType::Icon;
    icon.defaultValue = QString();
    icon.value = image;
    icon.apply = [](const QVariant&) {};
    icon.codeExpression = [](const QVariant& value) { return animatedwidgets::styleeditor::StyleCodeExpressions::icon(value.toString()); };
    StyleEditorItem item;
    item.typeName = QStringLiteral("ExampleButton");
    item.properties.append(icon);
    const QString code = StyleCodeGenerator::generate(item, project.path());
    QVERIFY(code.contains(QStringLiteral("QIcon(QStringLiteral(\"resources/nested/open.svg\"))")));
    QVERIFY(!code.contains(project.path()));
    QVERIFY(!StyleCodeGenerator::generate(item).contains(image));
}

void StyleResourceUtilsTest::qrcAliasesAndPreviewResolution() {
    QTemporaryDir project;
    QVERIFY(project.isValid());
    QVERIFY(writeFile(project.filePath(QStringLiteral("open.svg")), "<svg/>"));
    QVERIFY(writeFile(project.filePath(QStringLiteral("close.png")), "image"));
    const QByteArray qrc = "<RCC><qresource prefix=\"/actions\"><file alias=\"file/open.svg\">open.svg</file>"
        "<file>close.png</file><file>missing.svg</file></qresource>"
        "<qresource prefix=\"/\"><file alias=\"root.svg\">open.svg</file></qresource></RCC>";
    QVERIFY(writeFile(project.filePath(QStringLiteral("icons.qrc")), qrc));

    const StyleResourceSettings settings{project.path(), QStringLiteral("icons.qrc")};
    QString error;
    const auto entries = StyleResourceUtils::scan(settings, error);
    QVERIFY2(error.isEmpty(), qPrintable(error));
    QCOMPARE(entries.size(), 3);
    QCOMPARE(entries.at(0).reference, QStringLiteral(":/actions/close.png"));
    QCOMPARE(entries.at(1).reference, QStringLiteral(":/actions/file/open.svg"));
    QCOMPARE(entries.at(2).reference, QStringLiteral(":/root.svg"));
    QCOMPARE(entries.at(0).qrcPrefix, QStringLiteral("/actions"));
    QCOMPARE(entries.at(1).qrcPrefix, QStringLiteral("/actions"));
    QCOMPARE(entries.at(2).qrcPrefix, QStringLiteral("/"));
    QCOMPARE(StyleResourceUtils::resolve(settings, entries, entries.at(1).reference), project.filePath(QStringLiteral("open.svg")));
    QCOMPARE(StyleResourceUtils::relativePath(project.path(), entries.at(1).reference), entries.at(1).reference);
    QCOMPARE(StyleResourceUtils::resolve(settings, entries, QStringLiteral(":/builtin.svg")), QStringLiteral(":/builtin.svg"));
}

void StyleResourceUtilsTest::malformedAndMissingSources() {
    QTemporaryDir project;
    QVERIFY(project.isValid());
    QString error;
    QVERIFY(StyleResourceUtils::scan({}, error).isEmpty());
    QVERIFY(!error.isEmpty());
    QVERIFY(StyleResourceUtils::scan({project.path(), QStringLiteral("missing.qrc")}, error).isEmpty());
    QVERIFY(!error.isEmpty());
    QVERIFY(StyleResourceUtils::scan({project.path(), QStringLiteral("../outside")}, error).isEmpty());
    QVERIFY(!error.isEmpty());
    QVERIFY(StyleResourceUtils::scan({project.path(), QStringLiteral(".")}, error).isEmpty());
    QVERIFY(!error.isEmpty());
    QVERIFY(writeFile(project.filePath(QStringLiteral("broken.qrc")), "<RCC><qresource>"));
    QVERIFY(StyleResourceUtils::scan({project.path(), QStringLiteral("broken.qrc")}, error).isEmpty());
    QVERIFY(error.startsWith(QStringLiteral("Invalid QRC:")));
}

void StyleResourceUtilsTest::documentSettingsRoundTrip() {
    QTemporaryDir project;
    QVERIFY(project.isValid());
    QVERIFY(QDir(project.path()).mkpath(QStringLiteral("styles")));
    StyleDocumentSession session;
    StyleDocument content = session.document();
    content.resources = {project.path(), QStringLiteral("icons.qrc"), {QStringLiteral("icons.qrc"), QStringLiteral("extra.qrc")}};
    session.updateContent(content);
    QVERIFY(session.document().modified);
    QString error;
    const QString path = project.filePath(QStringLiteral("styles/buttons.json"));
    QVERIFY2(session.save(path, error), qPrintable(error));
    QFile saved(path);
    QVERIFY(saved.open(QIODevice::ReadOnly));
    const QByteArray json = saved.readAll();
    QVERIFY(!json.contains(project.path().toUtf8()));
    const QString storedProject = QJsonDocument::fromJson(json).object().value(QStringLiteral("resources")).toObject()
        .value(QStringLiteral("projectDirectory")).toString();
    QCOMPARE(QDir::cleanPath(storedProject), QStringLiteral(".."));
    saved.close();

    StyleDocumentSession reopened;
    QVERIFY2(reopened.open(path, error), qPrintable(error));
    QCOMPARE(QDir::cleanPath(reopened.document().resources.projectDirectory), QDir::cleanPath(project.path()));
    QCOMPARE(reopened.document().resources.source, QStringLiteral("icons.qrc"));
    QCOMPARE(reopened.document().resources.qrcFiles.size(), 2);
    QCOMPARE(reopened.document().resources.qrcFiles.at(0), QStringLiteral("icons.qrc"));
    QCOMPARE(reopened.document().resources.qrcFiles.at(1), QStringLiteral("extra.qrc"));
    reopened.updateContent(reopened.document());
    QVERIFY(!reopened.document().modified);
    QVERIFY2(reopened.save(project.filePath(QStringLiteral("buttons.json")), error), qPrintable(error));
    StyleDocument loaded;
    QVERIFY2(StyleDocumentCodec::readFile(project.filePath(QStringLiteral("buttons.json")), loaded, error), qPrintable(error));
    QCOMPARE(QDir::cleanPath(loaded.resources.projectDirectory), QDir::cleanPath(project.path()));
    QCOMPARE(loaded.resources.qrcFiles.size(), 2);
    StyleDocument updated = reopened.document();
    updated.resources.qrcFiles.append(QStringLiteral("third.qrc"));
    reopened.updateContent(updated);
    QVERIFY(reopened.document().modified);
    reopened.newDocument();
    QVERIFY(reopened.document().resources.projectDirectory.isEmpty());
    QVERIFY(reopened.document().resources.qrcFiles.isEmpty());
}

void StyleResourceUtilsTest::qrcHistoryAndSelection() {
    QTemporaryDir project;
    QVERIFY(project.isValid());
    QVERIFY(writeFile(project.filePath(QStringLiteral("first.qrc")), "<RCC/>"));
    QVERIFY(writeFile(project.filePath(QStringLiteral("second.qrc")), "<RCC/>"));
    StyleResourceSettings settings;
    QString error;
    QVERIFY(StyleResourceUtils::selectQrc(settings, project.filePath(QStringLiteral("first.qrc")), error));
    QCOMPARE(QDir::cleanPath(settings.projectDirectory), QDir::cleanPath(project.path()));
    QCOMPARE(settings.source, QStringLiteral("first.qrc"));
    QCOMPARE(settings.qrcFiles.size(), 1);
    QVERIFY(StyleResourceUtils::selectQrc(settings, project.filePath(QStringLiteral("second.qrc")), error));
    QCOMPARE(settings.source, QStringLiteral("second.qrc"));
    QCOMPARE(settings.qrcFiles.size(), 2);
    QVERIFY(StyleResourceUtils::selectQrc(settings, project.filePath(QStringLiteral("first.qrc")), error));
    QCOMPARE(settings.qrcFiles.size(), 2);
    QCOMPARE(settings.qrcFiles.first(), QStringLiteral("first.qrc"));

    QVERIFY(!StyleResourceUtils::selectQrc(settings, QString(), error));
    QVERIFY(!StyleResourceUtils::selectQrc(settings, project.filePath(QStringLiteral("missing.qrc")), error));
    QVERIFY(!error.isEmpty());
    QVERIFY(writeFile(project.filePath(QStringLiteral("broken.qrc")), "<RCC><qresource>"));
    QVERIFY(!StyleResourceUtils::selectQrc(settings, project.filePath(QStringLiteral("broken.qrc")), error));
    QVERIFY(!StyleResourceUtils::selectQrc(settings, project.filePath(QStringLiteral("image.svg")), error));
    QCOMPARE(settings.source, QStringLiteral("first.qrc"));
    QCOMPARE(settings.qrcFiles.size(), 2);

    settings.qrcFiles.append(QStringLiteral("./first.qrc"));
    settings.qrcFiles.append(QStringLiteral("../outside.qrc"));
    settings.qrcFiles.append(QStringLiteral("resources"));
    settings.qrcFiles.append(QStringLiteral(":/embedded.qrc"));
    QCOMPARE(StyleResourceUtils::qrcSources(settings).size(), 3);
}

void StyleResourceUtilsTest::multipleQrcPreviewResolution() {
    QTemporaryDir project;
    QVERIFY(project.isValid());
    QVERIFY(writeFile(project.filePath(QStringLiteral("first.svg")), "<svg/>"));
    QVERIFY(writeFile(project.filePath(QStringLiteral("second.svg")), "<svg/>"));
    QVERIFY(writeFile(project.filePath(QStringLiteral("first.qrc")),
        "<RCC><qresource prefix=\"/icons\"><file alias=\"shared.svg\">first.svg</file>"
        "<file alias=\"first.svg\">first.svg</file></qresource></RCC>"));
    QVERIFY(writeFile(project.filePath(QStringLiteral("second.qrc")),
        "<RCC><qresource prefix=\"/icons\"><file alias=\"shared.svg\">second.svg</file>"
        "<file alias=\"second.svg\">second.svg</file></qresource></RCC>"));
    StyleResourceSettings settings{project.path(), {}, {}};
    QString error;
    QVERIFY(StyleResourceUtils::selectQrc(settings, project.filePath(QStringLiteral("first.qrc")), error));
    QVERIFY(StyleResourceUtils::selectQrc(settings, project.filePath(QStringLiteral("second.qrc")), error));
    settings.qrcFiles.append(QStringLiteral("missing.qrc"));
    auto entries = StyleResourceUtils::scanAll(settings, error);
    QVERIFY(error.isEmpty());
    QCOMPARE(entries.size(), 3);
    QCOMPARE(StyleResourceUtils::resolve(settings, entries, QStringLiteral(":/icons/shared.svg")), project.filePath(QStringLiteral("second.svg")));
    QCOMPARE(StyleResourceUtils::resolve(settings, entries, QStringLiteral(":/icons/first.svg")), project.filePath(QStringLiteral("first.svg")));

    QVERIFY(StyleResourceUtils::selectQrc(settings, project.filePath(QStringLiteral("first.qrc")), error));
    entries = StyleResourceUtils::scanAll(settings, error);
    QVERIFY(error.isEmpty());
    QCOMPARE(StyleResourceUtils::resolve(settings, entries, QStringLiteral(":/icons/shared.svg")), project.filePath(QStringLiteral("first.svg")));
    QCOMPARE(StyleResourceUtils::resolve(settings, entries, QStringLiteral(":/icons/second.svg")), project.filePath(QStringLiteral("second.svg")));
}

void StyleResourceUtilsTest::independentQrcDirectories() {
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    QVERIFY(QDir(directory.path()).mkpath(QStringLiteral("first")));
    QVERIFY(QDir(directory.path()).mkpath(QStringLiteral("second")));
    QVERIFY(QDir(directory.path()).mkpath(QStringLiteral("documents")));
    const QString first = directory.filePath(QStringLiteral("first/icons.qrc"));
    const QString second = directory.filePath(QStringLiteral("second/icons.qrc"));
    const QString firstImage = directory.filePath(QStringLiteral("first/open.svg"));
    const QString secondImage = directory.filePath(QStringLiteral("second/close.svg"));
    QVERIFY(writeFile(firstImage, "<svg/>"));
    QVERIFY(writeFile(secondImage, "<svg/>"));
    QVERIFY(writeFile(first, "<RCC><qresource prefix=\"/first\"><file>open.svg</file></qresource></RCC>"));
    QVERIFY(writeFile(second, "<RCC><qresource prefix=\"/second\"><file>close.svg</file></qresource></RCC>"));

    QString error;
    const auto standalone = StyleResourceUtils::scan({{}, second, {}}, error);
    QVERIFY2(error.isEmpty(), qPrintable(error));
    QCOMPARE(standalone.size(), 1);
    QCOMPARE(standalone.first().filePath, secondImage);

    StyleResourceSettings settings;
    QVERIFY2(StyleResourceUtils::selectQrc(settings, first, error), qPrintable(error));
    QVERIFY2(StyleResourceUtils::selectQrc(settings, second, error), qPrintable(error));
    QCOMPARE(settings.source, QStringLiteral("../second/icons.qrc"));
    QCOMPARE(settings.qrcFiles.size(), 2);
    QCOMPARE(settings.qrcFiles.at(1), QStringLiteral("icons.qrc"));
    QVERIFY2(StyleResourceUtils::selectQrc(settings, second, error), qPrintable(error));
    QCOMPARE(settings.qrcFiles.size(), 2);
    const auto entries = StyleResourceUtils::scanAll(settings, error);
    QVERIFY2(error.isEmpty(), qPrintable(error));
    QCOMPARE(entries.size(), 2);
    QCOMPARE(StyleResourceUtils::resolve(settings, entries, QStringLiteral(":/first/open.svg")), firstImage);
    QCOMPARE(StyleResourceUtils::resolve(settings, entries, QStringLiteral(":/second/close.svg")), secondImage);

    StyleDocument document;
    document.resources = settings;
    const QString path = directory.filePath(QStringLiteral("documents/styles.json"));
    QVERIFY2(StyleDocumentCodec::writeFile(path, document, error), qPrintable(error));
    StyleDocument loaded;
    QVERIFY2(StyleDocumentCodec::readFile(path, loaded, error), qPrintable(error));
    auto restored = StyleResourceUtils::scanAll(loaded.resources, error);
    QVERIFY2(error.isEmpty(), qPrintable(error));
    QCOMPARE(restored.size(), 2);
    QCOMPARE(StyleResourceUtils::resolve(loaded.resources, restored, QStringLiteral(":/first/open.svg")), firstImage);
    QCOMPARE(StyleResourceUtils::resolve(loaded.resources, restored, QStringLiteral(":/second/close.svg")), secondImage);

    const QString moved = directory.filePath(QStringLiteral("styles.json"));
    QVERIFY2(StyleDocumentCodec::writeFile(moved, loaded, error), qPrintable(error));
    QVERIFY2(StyleDocumentCodec::readFile(moved, loaded, error), qPrintable(error));
    QVERIFY2(StyleResourceUtils::selectQrc(loaded.resources, first, error), qPrintable(error));
    QCOMPARE(loaded.resources.qrcFiles.size(), 2);
    restored = StyleResourceUtils::scanAll(loaded.resources, error);
    QVERIFY2(error.isEmpty(), qPrintable(error));
    QCOMPARE(restored.size(), 2);
    QCOMPARE(StyleResourceUtils::resolve(loaded.resources, restored, QStringLiteral(":/second/close.svg")), secondImage);
}

void StyleResourceUtilsTest::legacyQrcSettings() {
    QTemporaryDir project;
    QVERIFY(project.isValid());
    QVERIFY(writeFile(project.filePath(QStringLiteral("style.json")),
        "{\"resources\":{\"projectDirectory\":\".\",\"source\":\"first.qrc\"},\"items\":[]}"));
    StyleDocument document;
    QString error;
    QVERIFY(StyleDocumentCodec::readFile(project.filePath(QStringLiteral("style.json")), document, error));
    QCOMPARE(document.resources.qrcFiles.size(), 1);
    QCOMPARE(document.resources.qrcFiles.first(), QStringLiteral("first.qrc"));

    QVERIFY(writeFile(project.filePath(QStringLiteral("second.qrc")), "<RCC/>"));
    StyleResourceSettings settings{project.path(), QStringLiteral("first.qrc"), {}};
    QVERIFY(StyleResourceUtils::selectQrc(settings, project.filePath(QStringLiteral("second.qrc")), error));
    QCOMPARE(settings.qrcFiles.size(), 2);
    QCOMPARE(settings.qrcFiles.at(0), QStringLiteral("second.qrc"));
    QCOMPARE(settings.qrcFiles.at(1), QStringLiteral("first.qrc"));
}

QTEST_APPLESS_MAIN(StyleResourceUtilsTest)

#include "tststyleresourceutils.moc"
