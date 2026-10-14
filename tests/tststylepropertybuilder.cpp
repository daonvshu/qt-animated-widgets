#include "stylepropertytestobject.h"

#include "animatedwidgets/styleeditor/api/stylepropertybuilder.h"
#include "animatedwidgets/styleeditor/api/stylewidgettypebuilder.h"
#include "animatedwidgets/styleeditor/utils/stylepropertystateutils.h"
#include "../styleeditor/utils/stylecodegenerator.h"
#include "../styleeditor/utils/styledocumentcodec.h"

#include <QtTest>

using animatedwidgets::styleeditor::StyleProperty;
using animatedwidgets::styleeditor::StylePropertyBuilder;
using animatedwidgets::styleeditor::StylePropertyType;
using animatedwidgets::styleeditor::StylePropertyStateUtils;
using animatedwidgets::styleeditor::StyleWidgetTypeBuilder;
using animatedwidgets::styleeditor::StyleWidgetTypeOptions;

class StylePropertyBuilderTest : public QObject {
    Q_OBJECT

private slots:
    void scalarBindingsAndDefaults();
    void enumAndFlagBindings();
    void splitAlignmentBindingAndPersistence();
    void sparseExportAfterDocumentRestore();
    void textAndIconExpressionsEscapeResources();
    void invalidDefinitionsAreRejected();
    void callbacksSurviveTargetDeletion();
    void customBindingsAndRegistration();
    void propertyEnableConditions();
    void invalidEnableConditions();
    void enableConditionsSurviveDocumentRestore();
};

void StylePropertyBuilderTest::propertyEnableConditions() {
    StylePropertyTestObject defaults;
    StylePropertyTestObject object;
    StylePropertyBuilder builder(&object, &defaults);
    QVERIFY(builder.add(QStringLiteral("backgroundHover"), QStringLiteral("Hover")));
    QVERIFY(builder.add(QStringLiteral("animateContentColor"), QStringLiteral("Animate")));
    QVERIFY(builder.enumeration(QStringLiteral("placement"), QStringLiteral("Placement"), QStringLiteral("StylePropertyTestObject::Placement")));
    QVERIFY(builder.enabledWhen(QStringLiteral("backgroundHover"), QStringLiteral("animateContentColor")));
    QVERIFY(builder.enabledWhen(QStringLiteral("backgroundHover"), QStringLiteral("placement"), QStringLiteral("AfterText")));
    QVERIFY(builder.errors().isEmpty());

    QList<StyleProperty> properties = builder.properties();
    QVERIFY(StylePropertyStateUtils::isEnabled(properties.at(1), properties));
    QVERIFY(!StylePropertyStateUtils::isEnabled(properties.first(), properties));
    properties[1].value = true;
    QVERIFY(!StylePropertyStateUtils::isEnabled(properties.first(), properties));
    properties[2].value = QStringLiteral("AfterText");
    QVERIFY(StylePropertyStateUtils::isEnabled(properties.first(), properties));
    properties[1].value = false;
    QVERIFY(!StylePropertyStateUtils::isEnabled(properties.first(), properties));
    properties[1].value = true;
    QVERIFY(StylePropertyStateUtils::isEnabled(properties.first(), properties));

    properties.first().enabledWhen[0].value = false;
    QVERIFY(!StylePropertyStateUtils::isEnabled(properties.first(), properties));
    properties[1].value = false;
    QVERIFY(StylePropertyStateUtils::isEnabled(properties.first(), properties));

    properties.removeLast();
    QVERIFY(!StylePropertyStateUtils::isEnabled(properties.first(), properties));
    properties.first().enabledWhen = {{QStringLiteral("animateContentColor"), QVariant()}};
    QVERIFY(!StylePropertyStateUtils::isEnabled(properties.first(), properties));
}

void StylePropertyBuilderTest::invalidEnableConditions() {
    StylePropertyTestObject defaults;
    StylePropertyTestObject object;
    StylePropertyBuilder builder(&object, &defaults);
    QVERIFY(builder.add(QStringLiteral("backgroundHover"), QStringLiteral("Hover")));
    QVERIFY(builder.add(QStringLiteral("animateContentColor"), QStringLiteral("Animate")));
    QVERIFY(!builder.enabledWhen(QStringLiteral("missing"), QStringLiteral("animateContentColor")));
    QVERIFY(!builder.enabledWhen(QStringLiteral("backgroundHover"), QStringLiteral("missing")));
    QVERIFY(!builder.enabledWhen(QStringLiteral("animateContentColor"), QStringLiteral("animateContentColor")));
    QVERIFY(!builder.enabledWhen(QStringLiteral("backgroundHover"), QStringLiteral("animateContentColor"), QVariant()));
    QVERIFY(builder.enabledWhen(QStringLiteral("backgroundHover"), QStringLiteral("animateContentColor")));
    QVERIFY(!builder.enabledWhen(QStringLiteral("backgroundHover"), QStringLiteral("animateContentColor"), false));
    QCOMPARE(builder.errors().size(), 5);
    QCOMPARE(builder.properties().first().enabledWhen.size(), 1);
}

void StylePropertyBuilderTest::enableConditionsSurviveDocumentRestore() {
    StylePropertyTestObject defaults;
    StylePropertyTestObject object;
    StylePropertyBuilder builder(&object, &defaults);
    QVERIFY(builder.add(QStringLiteral("backgroundHover"), QStringLiteral("Hover")));
    QVERIFY(builder.add(QStringLiteral("animateContentColor"), QStringLiteral("Animate")));
    QVERIFY(builder.enabledWhen(QStringLiteral("backgroundHover"), QStringLiteral("animateContentColor")));

    StyleEditorItem item;
    item.typeName = QStringLiteral("ExampleButton");
    item.properties = builder.properties();
    QJsonObject stored;
    stored.insert(QStringLiteral("backgroundHover"), QStringLiteral("#ff123456"));
    stored.insert(QStringLiteral("animateContentColor"), true);
    StyleDocumentCodec::applySerializedProperties(item.properties, stored);
    QVERIFY(StylePropertyStateUtils::isEnabled(item.properties.first(), item.properties));
    QCOMPARE(object.backgroundHover, QColor(QStringLiteral("#123456")));

    item.properties[1].value = false;
    item.properties[1].apply(false);
    QVERIFY(!StylePropertyStateUtils::isEnabled(item.properties.first(), item.properties));
    const QJsonObject saved = StyleDocumentCodec::serializeProperties(item.properties);
    QCOMPARE(saved.value(QStringLiteral("backgroundHover")).toString(), QStringLiteral("#ff123456"));
    QVERIFY(StyleCodeGenerator::generate(item).contains(QStringLiteral("setBackgroundHover")));

    item.properties = builder.properties();
    StyleDocumentCodec::applySerializedProperties(item.properties, saved);
    QVERIFY(!StylePropertyStateUtils::isEnabled(item.properties.first(), item.properties));
    QCOMPARE(item.properties.first().value.value<QColor>(), QColor(QStringLiteral("#123456")));
    item.properties[1].value = true;
    QVERIFY(StylePropertyStateUtils::isEnabled(item.properties.first(), item.properties));
    item.properties[1].value = item.properties[1].defaultValue;
    QVERIFY(!StylePropertyStateUtils::isEnabled(item.properties.first(), item.properties));
}

void StylePropertyBuilderTest::scalarBindingsAndDefaults() {
    StylePropertyTestObject defaults;
    StylePropertyTestObject object;
    object.cornerRadius = 12;
    StylePropertyBuilder builder(&object, &defaults);
    builder.setGroup(QStringLiteral("Shape"));
    QVERIFY(builder.add(QStringLiteral("cornerRadius"), QStringLiteral("Radius")));
    QVERIFY(builder.add(QStringLiteral("borderWidth"), QStringLiteral("Border")));
    QVERIFY(builder.add(QStringLiteral("animateContentColor"), QStringLiteral("Animate")));
    QVERIFY(builder.add(QStringLiteral("backgroundHover"), QStringLiteral("Hover")));
    QVERIFY(builder.add(QStringLiteral("text"), QStringLiteral("Text")));
    QVERIFY(builder.errors().isEmpty());

    const QList<StyleProperty> properties = builder.properties();
    QCOMPARE(properties.at(0).group, QStringLiteral("Shape"));
    QCOMPARE(properties.at(0).type, StylePropertyType::Integer);
    QCOMPARE(properties.at(0).value.toInt(), 12);
    QCOMPARE(properties.at(0).defaultValue.toInt(), 6);
    properties.at(0).apply(8);
    QCOMPARE(object.cornerRadius, 8);
    QCOMPARE(defaults.cornerRadius, 6);

    QCOMPARE(properties.at(1).type, StylePropertyType::Real);
    properties.at(1).apply(1.23456789);
    QCOMPARE(object.borderWidth, 1.23456789);
    QCOMPARE(properties.at(1).codeExpression(object.borderWidth).toDouble(), object.borderWidth);
    properties.at(2).apply(true);
    QVERIFY(object.animateContentColor);
    QCOMPARE(properties.at(2).codeExpression(true), QStringLiteral("true"));
    properties.at(3).apply(QColor(Qt::blue));
    QCOMPARE(object.backgroundHover, QColor(Qt::blue));
    properties.at(4).apply(QStringLiteral("Updated"));
    QCOMPARE(object.text, QStringLiteral("Updated"));
}

void StylePropertyBuilderTest::enumAndFlagBindings() {
    StylePropertyTestObject defaults;
    StylePropertyTestObject object;
    object.placement = StylePropertyTestObject::Placement::AfterText;
    StylePropertyBuilder builder(&object, &defaults);
    QVERIFY(builder.enumeration(QStringLiteral("placement"), QStringLiteral("Placement"), QStringLiteral("StylePropertyTestObject::Placement")));
    QVERIFY(builder.enumeration(QStringLiteral("contentAlignment"), QStringLiteral("Alignment"), {
        {QStringLiteral("AlignCenter"), Qt::AlignCenter, QStringLiteral("Qt::AlignCenter")},
        {QStringLiteral("AlignLeft|AlignTop"), Qt::AlignLeft | Qt::AlignTop, QStringLiteral("Qt::AlignLeft | Qt::AlignTop")}
    }));
    const QList<StyleProperty> properties = builder.properties();
    QCOMPARE(properties.at(0).value.toString(), QStringLiteral("AfterText"));
    QCOMPARE(properties.at(0).defaultValue.toString(), QStringLiteral("BeforeText"));
    QCOMPARE(properties.at(0).codeExpression(properties.at(0).value), QStringLiteral("StylePropertyTestObject::Placement::AfterText"));
    properties.at(0).apply(QStringLiteral("BeforeText"));
    QCOMPARE(object.placement, StylePropertyTestObject::Placement::BeforeText);

    properties.at(1).apply(QStringLiteral("AlignLeft|AlignTop"));
    QCOMPARE(object.contentAlignment, Qt::Alignment(Qt::AlignLeft | Qt::AlignTop));
    QCOMPARE(properties.at(1).codeExpression(QStringLiteral("AlignLeft|AlignTop")), QStringLiteral("Qt::AlignLeft | Qt::AlignTop"));
    properties.at(1).apply(QStringLiteral("Unknown"));
    QCOMPARE(object.contentAlignment, Qt::Alignment(Qt::AlignLeft | Qt::AlignTop));
    QVERIFY(builder.errors().isEmpty());
}

void StylePropertyBuilderTest::splitAlignmentBindingAndPersistence() {
    using animatedwidgets::styleeditor::StyleAlignmentUtils;

    StylePropertyTestObject defaults;
    auto* object = new StylePropertyTestObject;
    StylePropertyBuilder builder(object, &defaults);
    QVERIFY(builder.alignment(QStringLiteral("contentAlignment"), QStringLiteral("Content alignment")));
    QVERIFY(builder.errors().isEmpty());

    StyleEditorItem item;
    item.typeName = QStringLiteral("ExampleButton");
    item.properties = builder.properties();
    QCOMPARE(item.properties.first().type, StylePropertyType::Alignment);
    QCOMPARE(item.properties.first().defaultValue.toString(), QStringLiteral("AlignCenter"));
    QVERIFY(!StyleCodeGenerator::generate(item).contains(QStringLiteral("setContentAlignment")));

    const QStringList horizontal = {QStringLiteral("AlignLeft"), QStringLiteral("AlignHCenter"), QStringLiteral("AlignRight")};
    const QStringList vertical = {QStringLiteral("AlignTop"), QStringLiteral("AlignVCenter"), QStringLiteral("AlignBottom")};
    const int horizontalFlags[] = {Qt::AlignLeft, Qt::AlignHCenter, Qt::AlignRight};
    const int verticalFlags[] = {Qt::AlignTop, Qt::AlignVCenter, Qt::AlignBottom};
    for (int x = 0; x < horizontal.size(); ++x) {
        for (int y = 0; y < vertical.size(); ++y) {
            const QString combined = StyleAlignmentUtils::combine(horizontal.at(x), vertical.at(y));
            QCOMPARE(StyleAlignmentUtils::horizontal(combined), horizontal.at(x));
            QCOMPARE(StyleAlignmentUtils::vertical(combined), vertical.at(y));
            item.properties.first().apply(combined);
            QCOMPARE(object->contentAlignment, Qt::Alignment(horizontalFlags[x] | verticalFlags[y]));
        }
    }

    QJsonObject stored;
    stored.insert(QStringLiteral("contentAlignment"), QStringLiteral("AlignLeft|AlignBottom"));
    StyleDocumentCodec::applySerializedProperties(item.properties, stored);
    QCOMPARE(object->contentAlignment, Qt::Alignment(Qt::AlignLeft | Qt::AlignBottom));
    QCOMPARE(StyleDocumentCodec::serializeProperties(item.properties).value(QStringLiteral("contentAlignment")).toString(),
             QStringLiteral("AlignLeft|AlignBottom"));
    QVERIFY(StyleCodeGenerator::generate(item).contains(QStringLiteral("setContentAlignment(Qt::AlignLeft | Qt::AlignBottom)")));

    stored.insert(QStringLiteral("contentAlignment"), QStringLiteral("Unknown"));
    StyleDocumentCodec::applySerializedProperties(item.properties, stored);
    QCOMPARE(object->contentAlignment, Qt::Alignment(Qt::AlignLeft | Qt::AlignBottom));

    QList<StyleProperty> previous = item.properties;
    previous.first().type = StylePropertyType::Enum;
    previous.first().value = QStringLiteral("AlignRight|AlignTop");
    StyleDocumentCodec::applyCompatibleProperties(item.properties, previous);
    QCOMPARE(object->contentAlignment, Qt::Alignment(Qt::AlignRight | Qt::AlignTop));

    item.properties.first().value = item.properties.first().defaultValue;
    item.properties.first().apply(item.properties.first().value);
    QCOMPARE(object->contentAlignment, Qt::Alignment(Qt::AlignCenter));
    QVERIFY(!StyleCodeGenerator::generate(item).contains(QStringLiteral("setContentAlignment")));

    delete object;
    item.properties.first().apply(QStringLiteral("AlignLeft|AlignTop"));
}

void StylePropertyBuilderTest::sparseExportAfterDocumentRestore() {
    StylePropertyTestObject defaults;
    StylePropertyTestObject object;
    object.text = QStringLiteral("Preview");
    StylePropertyBuilder builder(&object, &defaults);
    QVERIFY(builder.add(QStringLiteral("text"), QStringLiteral("Text")));
    QVERIFY(builder.add(QStringLiteral("cornerRadius"), QStringLiteral("Radius")));
    QVERIFY(builder.add(QStringLiteral("backgroundHover"), QStringLiteral("Hover")));

    StyleEditorItem item;
    item.typeName = QStringLiteral("ExampleButton");
    item.properties = builder.properties();
    const QString initialCode = StyleCodeGenerator::generate(item);
    QVERIFY(initialCode.contains(QStringLiteral("setText(QStringLiteral(\"Preview\"))")));
    QVERIFY(!initialCode.contains(QStringLiteral("setCornerRadius")));
    QVERIFY(!initialCode.contains(QStringLiteral("setBackgroundHover")));

    QJsonObject stored = StyleDocumentCodec::serializeProperties(item.properties);
    stored.insert(QStringLiteral("cornerRadius"), 14);
    stored.insert(QStringLiteral("backgroundHover"), QStringLiteral("#00000000"));
    StyleDocumentCodec::applySerializedProperties(item.properties, stored);
    QCOMPARE(object.cornerRadius, 14);
    QCOMPARE(object.backgroundHover, QColor(0, 0, 0, 0));
    const QString changedCode = StyleCodeGenerator::generate(item);
    QVERIFY(changedCode.contains(QStringLiteral("setCornerRadius(14)")));
    QVERIFY(changedCode.contains(QStringLiteral("setBackgroundHover(QColor(\"#00000000\"))")));

    for (StyleProperty& property : item.properties) {
        property.value = property.defaultValue;
        property.apply(property.value);
    }
    QVERIFY(!StyleCodeGenerator::generate(item).contains(QStringLiteral("widget->set")));
    QCOMPARE(object.cornerRadius, defaults.cornerRadius);
    QVERIFY(!object.backgroundHover.isValid());
}

void StylePropertyBuilderTest::textAndIconExpressionsEscapeResources() {
    StylePropertyTestObject defaults;
    StylePropertyTestObject object;
    StylePropertyBuilder builder(&object, &defaults);
    QVERIFY(builder.add(QStringLiteral("text"), QStringLiteral("Text")));
    QVERIFY(builder.icon(QStringLiteral("icon"), QStringLiteral("Icon")));
    const QList<StyleProperty> properties = builder.properties();
    const QString text = QStringLiteral("A\"B\\C\nD\rE\tF");
    QCOMPARE(properties.at(0).codeExpression(text), QStringLiteral("QStringLiteral(\"A\\\"B\\\\C\\nD\\rE\\tF\")"));
    QCOMPARE(properties.at(1).codeExpression(QStringLiteral(":/icons/a\"b.svg")), QStringLiteral("QIcon(QStringLiteral(\":/icons/a\\\"b.svg\"))"));
    QCOMPARE(properties.at(1).codeExpression(QString()), QStringLiteral("QIcon()"));
}

void StylePropertyBuilderTest::invalidDefinitionsAreRejected() {
    StylePropertyTestObject defaults;
    StylePropertyTestObject object;
    StylePropertyBuilder builder(&object, &defaults);
    QVERIFY(!builder.add(QStringLiteral("missing"), QStringLiteral("Missing")));
    QVERIFY(!builder.add(QStringLiteral("readOnly"), QStringLiteral("Read only")));
    QVERIFY(!builder.add(QStringLiteral("placement"), QStringLiteral("Placement")));
    QVERIFY(!builder.icon(QStringLiteral("text"), QStringLiteral("Icon")));
    QVERIFY(builder.add(QStringLiteral("cornerRadius"), QStringLiteral("Radius")));
    QVERIFY(!builder.add(QStringLiteral("cornerRadius"), QStringLiteral("Duplicate")));
    QVERIFY(!builder.enumeration(QStringLiteral("contentAlignment"), QStringLiteral("Alignment"), {
        {QStringLiteral("Left"), Qt::AlignLeft, QStringLiteral("Qt::AlignLeft")}
    }));
    QCOMPARE(builder.properties().size(), 1);
    QCOMPARE(builder.errors().size(), 6);
    QCOMPARE(object.cornerRadius, defaults.cornerRadius);
}

void StylePropertyBuilderTest::callbacksSurviveTargetDeletion() {
    StylePropertyTestObject defaults;
    auto* object = new StylePropertyTestObject;
    StylePropertyBuilder builder(object, &defaults);
    QVERIFY(builder.add(QStringLiteral("cornerRadius"), QStringLiteral("Radius")));
    QVERIFY(builder.enumeration(QStringLiteral("placement"), QStringLiteral("Placement"), QStringLiteral("StylePropertyTestObject::Placement")));
    QVERIFY(builder.icon(QStringLiteral("icon"), QStringLiteral("Icon")));
    const QList<StyleProperty> properties = builder.properties();
    delete object;
    properties.at(0).apply(12);
    properties.at(1).apply(QStringLiteral("AfterText"));
    properties.at(2).apply(QString());
    QVERIFY(!builder.add(QStringLiteral("text"), QStringLiteral("Text")));
    QCOMPARE(defaults.cornerRadius, 6);
}

void StylePropertyBuilderTest::customBindingsAndRegistration() {
    StylePropertyTestObject defaults;
    StylePropertyTestObject object;
    StylePropertyBuilder builder(&object, &defaults);
    builder.setGroup(QStringLiteral("Custom"));
    int applied = 0;
    StyleProperty property;
    property.name = QStringLiteral("customValue");
    property.apply = [&applied](const QVariant& value) { applied = value.toInt(); };
    property.codeExpression = [](const QVariant& value) { return QString::number(value.toInt()); };
    QVERIFY(builder.add(property));
    QCOMPARE(builder.properties().first().group, QStringLiteral("Custom"));
    builder.properties().first().apply(9);
    QCOMPARE(applied, 9);

    StyleWidgetTypeOptions options;
    options.id = QStringLiteral("example");
    options.name = QStringLiteral("Example");
    options.category = QStringLiteral("Custom");
    options.suggestedSize = QSize(150, 42);
    const auto type = StyleWidgetTypeBuilder::create<QWidget>(options);
    QCOMPARE(type.id, QStringLiteral("example"));
    QCOMPARE(type.name, QStringLiteral("Example"));
    QCOMPARE(type.category, QStringLiteral("Custom"));
    QCOMPARE(type.typeName, QStringLiteral("QWidget"));
    QCOMPARE(type.suggestedSize, QSize(150, 42));
    QVERIFY(type.create);
    QVERIFY(type.properties);

    StyleWidgetTypeOptions namedOptions;
    namedOptions.id = QStringLiteral("named");
    namedOptions.name = QStringLiteral("Named");
    namedOptions.category = QStringLiteral("Custom");
    namedOptions.typeName = QStringLiteral("ExampleWidget");
    const auto named = StyleWidgetTypeBuilder::create<QWidget>(namedOptions);
    QCOMPARE(named.typeName, QStringLiteral("ExampleWidget"));
    QVERIFY(!named.suggestedSize.isValid());
}

QTEST_APPLESS_MAIN(StylePropertyBuilderTest)

#include "tststylepropertybuilder.moc"
