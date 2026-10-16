#include "../styleeditor/document/styledocumentsession.h"
#include "../styleeditor/utils/styledocumentcodec.h"
#include "../styleeditor/utils/stylecodegenerator.h"
#include "../styleeditor/utils/stylepreviewutils.h"

#include <QTemporaryDir>
#include <QJsonArray>
#include <QtTest>

class StyleDocumentSessionTest : public QObject {
    Q_OBJECT

private slots:
    void temporaryEditsAndMetadata();
    void saveOpenAndUnchangedCollection();
    void failedFileOperationsPreserveDocument();
    void newDocumentClearsContent();
    void migrateCompatibleProperties();
    void unsetAndTransparentColorsRoundTrip();
    void sparseCodeExportAndReset();
    void codeExportPreservesExplicitColors();
    void previewSettingsRoundTrip();
    void previewSizeFallback();
};

void StyleDocumentSessionTest::temporaryEditsAndMetadata() {
    StyleDocumentSession session;
    QSignalSpy changes(&session, &StyleDocumentSession::metadataChanged);
    session.setName(QStringLiteral("Buttons"));

    StyleDocument content = session.document();
    StyleDocumentItem item;
    item.id = QStringLiteral("primary");
    item.widgetTypeId = QStringLiteral("filledbutton");
    item.displayName = QStringLiteral("Primary");
    item.serializedProperties.insert(QStringLiteral("cornerRadius"), 8);
    content.items.append(item);
    content.name = QStringLiteral("Page must not rename the document");
    content.filePath = QStringLiteral("page.json");
    content.modified = false;
    session.updateContent(content);

    QCOMPARE(session.document().name, QStringLiteral("Buttons"));
    QVERIFY(session.document().filePath.isEmpty());
    QVERIFY(session.document().modified);
    QCOMPARE(session.document().items.size(), 1);
    QCOMPARE(changes.count(), 2);
    session.updateContent(session.document());
    QCOMPARE(changes.count(), 2);
}

void StyleDocumentSessionTest::saveOpenAndUnchangedCollection() {
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    const QString filePath = directory.filePath(QStringLiteral("styles.json"));

    StyleDocumentSession session;
    session.setName(QStringLiteral("  Buttons  "));
    StyleDocument content = session.document();
    StyleDocumentItem item;
    item.id = QStringLiteral("custom");
    item.displayName = QStringLiteral("External widget");
    item.widgetTypeId = QStringLiteral("external-widget");
    item.serializedProperties.insert(QStringLiteral("customSetting"), QStringLiteral("preserved"));
    content.items.append(item);
    session.updateContent(content);

    QString error;
    QVERIFY2(session.save(filePath, error), qPrintable(error));
    QVERIFY(!session.document().modified);
    QCOMPARE(session.document().name, QStringLiteral("Buttons"));
    QCOMPARE(session.document().filePath, filePath);

    StyleDocumentSession reopened;
    QVERIFY2(reopened.open(filePath, error), qPrintable(error));
    QVERIFY(StyleDocumentCodec::toJson(reopened.document()) == StyleDocumentCodec::toJson(session.document()));
    QSignalSpy changes(&reopened, &StyleDocumentSession::metadataChanged);
    reopened.updateContent(reopened.document());
    QVERIFY(!reopened.document().modified);
    QCOMPARE(changes.count(), 0);

    content = reopened.document();
    content.items[0].serializedProperties.insert(QStringLiteral("customSetting"), QStringLiteral("updated"));
    reopened.updateContent(content);
    QVERIFY(reopened.document().modified);
    QVERIFY2(reopened.save(filePath, error), qPrintable(error));
    QVERIFY2(session.open(filePath, error), qPrintable(error));
    QCOMPARE(session.document().items.at(0).serializedProperties.value(QStringLiteral("customSetting")).toString(), QStringLiteral("updated"));
}

void StyleDocumentSessionTest::failedFileOperationsPreserveDocument() {
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    StyleDocumentSession session;
    session.setName(QStringLiteral("Keep this document"));
    const QJsonObject before = StyleDocumentCodec::toJson(session.document());

    QString error;
    QVERIFY(!session.open(directory.filePath(QStringLiteral("missing.json")), error));
    QVERIFY(!error.isEmpty());
    QVERIFY(StyleDocumentCodec::toJson(session.document()) == before);
    QVERIFY(session.document().modified);

    error.clear();
    QVERIFY(!session.save(directory.filePath(QStringLiteral("missing/styles.json")), error));
    QVERIFY(!error.isEmpty());
    QVERIFY(StyleDocumentCodec::toJson(session.document()) == before);
    QVERIFY(session.document().filePath.isEmpty());
    QVERIFY(session.document().modified);
}

void StyleDocumentSessionTest::newDocumentClearsContent() {
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    StyleDocumentSession session;
    session.setName(QStringLiteral(" "));
    StyleDocument content = session.document();
    StyleDocumentItem item;
    item.id = QStringLiteral("primary");
    item.widgetTypeId = QStringLiteral("filledbutton");
    item.displayName = QStringLiteral("Primary");
    content.items.append(item);
    session.updateContent(content);
    QString error;
    QVERIFY2(session.save(directory.filePath(QStringLiteral("styles.json")), error), qPrintable(error));
    QCOMPARE(session.document().name, QStringLiteral("Untitled"));

    session.newDocument();
    QVERIFY(session.document().items.isEmpty());
    QVERIFY(session.document().filePath.isEmpty());
    QVERIFY(!session.document().modified);
    QCOMPARE(session.document().name, QStringLiteral("Untitled"));
}

void StyleDocumentSessionTest::migrateCompatibleProperties() {
    using animatedwidgets::styleeditor::StyleProperty;
    using animatedwidgets::styleeditor::StylePropertyType;

    int oldCalls = 0;
    StyleProperty radius;
    radius.name = QStringLiteral("cornerRadius");
    radius.type = StylePropertyType::Integer;
    radius.value = 12;
    radius.apply = [&oldCalls](const QVariant&) { ++oldCalls; };

    StyleProperty alignment;
    alignment.name = QStringLiteral("alignment");
    alignment.type = StylePropertyType::Enum;
    alignment.value = QStringLiteral("Right");

    StyleProperty shape;
    shape.name = QStringLiteral("shape");
    shape.type = StylePropertyType::Enum;
    shape.value = QStringLiteral("Circle");

    StyleProperty width;
    width.name = QStringLiteral("borderWidth");
    width.type = StylePropertyType::Text;
    width.value = QStringLiteral("incompatible");
    const QList<StyleProperty> previous{radius, alignment, shape, width};

    int newCalls = 0;
    int appliedRadius = -1;
    radius.value = 4;
    radius.defaultValue = 4;
    radius.apply = [&newCalls, &appliedRadius](const QVariant& value) {
        ++newCalls;
        appliedRadius = value.toInt();
    };
    alignment.value = QStringLiteral("Left");
    alignment.enumNames = QStringList{QStringLiteral("Left"), QStringLiteral("Right")};
    shape.value = QStringLiteral("Rectangle");
    shape.enumNames = QStringList{QStringLiteral("Rectangle")};
    width.type = StylePropertyType::Integer;
    width.value = 1;

    StyleProperty spacing;
    spacing.name = QStringLiteral("spacing");
    spacing.type = StylePropertyType::Integer;
    spacing.value = 8;
    QList<StyleProperty> properties{radius, alignment, shape, width, spacing};
    StyleDocumentCodec::applyCompatibleProperties(properties, previous);

    QCOMPARE(properties.at(0).value.toInt(), 12);
    QCOMPARE(properties.at(0).defaultValue.toInt(), 4);
    QCOMPARE(properties.at(1).value.toString(), QStringLiteral("Right"));
    QCOMPARE(properties.at(2).value.toString(), QStringLiteral("Rectangle"));
    QCOMPARE(properties.at(3).value.toInt(), 1);
    QCOMPARE(properties.at(4).value.toInt(), 8);
    QCOMPARE(appliedRadius, 12);
    QCOMPARE(newCalls, 1);
    QCOMPARE(oldCalls, 0);
    QCOMPARE(previous.at(0).value.toInt(), 12);
}

void StyleDocumentSessionTest::unsetAndTransparentColorsRoundTrip() {
    using animatedwidgets::styleeditor::StyleProperty;
    using animatedwidgets::styleeditor::StylePropertyType;

    StyleProperty automatic;
    automatic.name = QStringLiteral("backgroundHover");
    automatic.type = StylePropertyType::Color;
    automatic.value = QColor();

    StyleProperty transparent = automatic;
    transparent.name = QStringLiteral("backgroundNormal");
    transparent.value = QColor(24, 48, 96, 0);
    const QJsonObject stored = StyleDocumentCodec::serializeProperties({automatic, transparent});
    QVERIFY(stored.value(automatic.name).isNull());
    QCOMPARE(stored.value(transparent.name).toString(), QStringLiteral("#00183060"));

    int calls = 0;
    QColor applied(Qt::white);
    automatic.value = QColor(Qt::blue);
    automatic.apply = [&calls, &applied](const QVariant& value) {
        ++calls;
        applied = value.value<QColor>();
    };
    transparent.value = QColor(Qt::white);
    QList<StyleProperty> properties{automatic, transparent};
    StyleDocumentCodec::applySerializedProperties(properties, stored);
    QVERIFY(!properties.at(0).value.value<QColor>().isValid());
    QVERIFY(!applied.isValid());
    QCOMPARE(calls, 1);
    QCOMPARE(properties.at(1).value.value<QColor>(), QColor(24, 48, 96, 0));

    properties[0].value = QColor(Qt::blue);
    StyleDocumentCodec::applySerializedProperties(properties, {});
    QCOMPARE(properties.at(0).value.value<QColor>(), QColor(Qt::blue));
}

void StyleDocumentSessionTest::sparseCodeExportAndReset() {
    using animatedwidgets::styleeditor::StyleProperty;
    using animatedwidgets::styleeditor::StylePropertyType;

    StyleProperty radius;
    radius.name = QStringLiteral("cornerRadius");
    radius.type = StylePropertyType::Integer;
    radius.value = 4;
    radius.defaultValue = 4;
    radius.apply = [](const QVariant&) {};
    radius.codeExpression = [](const QVariant& value) { return QString::number(value.toInt()); };

    StyleProperty duration = radius;
    duration.name = QStringLiteral("animationDuration");
    duration.value = 300;
    duration.defaultValue = 300;

    StyleEditorItem item;
    item.displayName = QStringLiteral("Primary");
    item.typeName = QStringLiteral("animatedwidgets::FilledButton");
    item.properties = {radius, duration};
    const QString defaultCode = QStringLiteral("// Primary\nauto* widget = new animatedwidgets::FilledButton(parent);\n");
    QCOMPARE(StyleCodeGenerator::generate(item), defaultCode);

    item.properties[0].value = 12;
    QCOMPARE(StyleCodeGenerator::generate(item), defaultCode + QStringLiteral("widget->setCornerRadius(12);\n"));
    item.properties[0].value = item.properties.at(0).defaultValue;
    QCOMPARE(StyleCodeGenerator::generate(item), defaultCode);

    item.properties[0].defaultValue = QVariant();
    QCOMPARE(StyleCodeGenerator::generate(item), defaultCode + QStringLiteral("widget->setCornerRadius(4);\n"));
}

void StyleDocumentSessionTest::codeExportPreservesExplicitColors() {
    using animatedwidgets::styleeditor::StyleProperty;
    using animatedwidgets::styleeditor::StylePropertyType;

    StyleProperty background;
    background.name = QStringLiteral("backgroundHover");
    background.type = StylePropertyType::Color;
    background.value = QColor();
    background.defaultValue = QColor();
    background.apply = [](const QVariant&) {};
    background.codeExpression = [](const QVariant& value) {
        const QColor color = value.value<QColor>();
        return color.isValid() ? QStringLiteral("QColor(\"%1\")").arg(color.name(QColor::HexArgb)) : QStringLiteral("QColor()");
    };

    StyleEditorItem item;
    item.properties = {background};
    QVERIFY(!StyleCodeGenerator::generate(item).contains(QStringLiteral("setBackgroundHover")));

    QJsonObject stored;
    stored.insert(background.name, QStringLiteral("#00000000"));
    StyleDocumentCodec::applySerializedProperties(item.properties, stored);
    QVERIFY(StyleCodeGenerator::generate(item).contains(QStringLiteral("widget->setBackgroundHover(QColor(\"#00000000\"));")));

    item.properties[0].defaultValue = QColor(Qt::blue);
    stored.insert(background.name, QJsonValue(QJsonValue::Null));
    StyleDocumentCodec::applySerializedProperties(item.properties, stored);
    QVERIFY(StyleCodeGenerator::generate(item).contains(QStringLiteral("widget->setBackgroundHover(QColor());")));
}

void StyleDocumentSessionTest::previewSettingsRoundTrip() {
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    StyleDocument document;
    StyleDocumentItem item;
    item.id = QStringLiteral("primary");
    item.widgetTypeId = QStringLiteral("filledbutton");
    item.displayName = QStringLiteral("Primary");
    document.items.append(item);
    QVERIFY(!StyleDocumentCodec::toJson(document).value(QStringLiteral("items")).toArray().first().toObject().contains(QStringLiteral("preview")));

    document.items[0].previewSettings.size = QSize(240, 56);
    document.items[0].previewSettings.background = QColor(24, 48, 96, 0);
    document.items[0].previewSettings.widthAuto = true;
    StyleDocumentSession session;
    session.updateContent(document);
    QVERIFY(session.document().modified);
    QString error;
    const QString path = directory.filePath(QStringLiteral("preview.json"));
    QVERIFY2(session.save(path, error), qPrintable(error));

    StyleDocumentSession reopened;
    QVERIFY2(reopened.open(path, error), qPrintable(error));
    QCOMPARE(reopened.document().items.first().previewSettings.size, QSize(-1, 56));
    QCOMPARE(reopened.document().items.first().previewSettings.background, QColor(24, 48, 96, 0));
    QVERIFY(reopened.document().items.first().previewSettings.widthAuto);
    QVERIFY(!reopened.document().items.first().previewSettings.heightAuto);

    StyleEditorItem editorItem;
    static_cast<StyleDocumentItem&>(editorItem) = reopened.document().items.first();
    editorItem.typeName = QStringLiteral("ExampleButton");
    QVERIFY(!StyleCodeGenerator::generate(editorItem).contains(QStringLiteral("widget->set")));

    document.items[0].previewSettings = {};
    reopened.updateContent(document);
    QVERIFY(reopened.document().modified);
    QVERIFY2(reopened.save(path, error), qPrintable(error));
    QVERIFY2(session.open(path, error), qPrintable(error));
    QVERIFY(!session.document().items.first().previewSettings.size.isValid());
    QVERIFY(!session.document().items.first().previewSettings.background.isValid());
    QVERIFY(!session.document().items.first().previewSettings.widthAuto);
    QVERIFY(!session.document().items.first().previewSettings.heightAuto);
}

void StyleDocumentSessionTest::previewSizeFallback() {
    QCOMPARE(StylePreviewUtils::resolveSize(QSize(220, 60), QSize(150, 42), QSize(80, 30)), QSize(220, 60));
    QCOMPARE(StylePreviewUtils::resolveSize({}, QSize(150, 42), QSize(80, 30)), QSize(150, 42));
    QCOMPARE(StylePreviewUtils::resolveSize({}, {}, QSize(80, 30)), QSize(80, 30));
    QCOMPARE(StylePreviewUtils::resolveSize({}, {}, {}), QSize(150, 42));
    QCOMPARE(StylePreviewUtils::resolveSize(QSize(9000, 7000), {}, {}), QSize(4096, 4096));
    QCOMPARE(StylePreviewUtils::resolveSize(QSize(220, 60), QSize(150, 42), QSize(80, 30), true, false), QSize(80, 60));
    QCOMPARE(StylePreviewUtils::resolveSize(QSize(220, 60), QSize(150, 42), QSize(80, 30), false, true), QSize(220, 30));
}

QTEST_APPLESS_MAIN(StyleDocumentSessionTest)

#include "tststyledocumentsession.moc"
