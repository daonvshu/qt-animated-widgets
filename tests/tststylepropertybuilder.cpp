#include "stylepropertytestobject.h"

#include "animatedwidgets/styleeditor/api/stylepropertybuilder.h"
#include "animatedwidgets/styleeditor/api/stylewidgettypebuilder.h"
#include "animatedwidgets/styleeditor/utils/stylepropertystateutils.h"
#include "../styleeditor/utils/stylecodegenerator.h"
#include "../styleeditor/utils/styledocumentcodec.h"
#include "../plugins/styleeditor/utils/styleobjectproperties.h"

#include <QtTest>

#include <algorithm>

using animatedwidgets::styleeditor::StyleProperty;
using animatedwidgets::styleeditor::StylePropertyBuilder;
using animatedwidgets::styleeditor::StylePropertyGroup;
using animatedwidgets::styleeditor::StylePropertyOptions;
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
    void sizeAndShortcutBindingsRoundTrip();
    void iconHintPersistenceAndSparseExport();
    void invalidDefinitionsAreRejected();
    void callbacksSurviveTargetDeletion();
    void customBindingsAndRegistration();
    void styleMembersPreserveOtherFields();
    void customStatementsAndSparseExport();
    void propertyEnableConditions();
    void invalidEnableConditions();
    void enableConditionsSurviveDocumentRestore();
    void structuredGroupsAndBindings();
    void structuredExplicitEnumOptions();
    void structuredGroupsAreAtomic_data();
    void structuredGroupsAreAtomic();
};

void StylePropertyBuilderTest::structuredGroupsAndBindings() {
    StylePropertyTestObject defaults;
    StylePropertyTestObject object;
    object.cornerRadius = 12;
    StylePropertyBuilder builder(&object, &defaults);
    builder.setGroup(QStringLiteral("Existing"));
    QVERIFY(builder.add(QStringLiteral("text"), QStringLiteral("Text")));

    StylePropertyOptions hover;
    hover.name = QStringLiteral("backgroundHover");
    hover.label = QStringLiteral("Hover");
    hover.description = QStringLiteral("Hover color.");
    hover.enabledWhen = {{QStringLiteral("animateContentColor"), true}, {QStringLiteral("placement"), QStringLiteral("AfterText")}};

    StylePropertyOptions icon;
    icon.name = QStringLiteral("icon");
    icon.label = QStringLiteral("Icon");
    icon.binding = StylePropertyOptions::Binding::Icon;
    icon.iconResource = QStringLiteral(":/icons/current.svg");
    icon.defaultIconResource = QStringLiteral(":/icons/default.svg");

    StylePropertyOptions size;
    size.name = QStringLiteral("iconSize");
    size.label = QStringLiteral("Icon size");

    StylePropertyOptions shortcut;
    shortcut.name = QStringLiteral("shortcut");
    shortcut.label = QStringLiteral("Shortcut");

    StylePropertyOptions radius;
    radius.name = QStringLiteral("cornerRadius");
    radius.label = QStringLiteral("Radius");

    StylePropertyOptions placement;
    placement.name = QStringLiteral("placement");
    placement.label = QStringLiteral("Placement");
    placement.binding = StylePropertyOptions::Binding::Enumeration;
    placement.enumScope = QStringLiteral("StylePropertyTestObject::Placement");

    StylePropertyOptions alignment;
    alignment.name = QStringLiteral("contentAlignment");
    alignment.label = QStringLiteral("Alignment");
    alignment.binding = StylePropertyOptions::Binding::Alignment;

    StylePropertyOptions animate;
    animate.name = QStringLiteral("animateContentColor");
    animate.label = QStringLiteral("Animate");

    StylePropertyGroup content;
    content.name = QStringLiteral("Content");
    content.properties = {hover, icon, size, shortcut};

    StylePropertyGroup shape;
    shape.name = QStringLiteral("Shape");
    shape.properties = {radius, placement, alignment};

    StylePropertyGroup animation;
    animation.properties = {animate};
    QVERIFY(builder.addGroups({content, shape, animation}));
    QVERIFY(builder.errors().isEmpty());

    StylePropertyBuilder reference(&object, &defaults);
    reference.setGroup(QStringLiteral("Existing"));
    QVERIFY(reference.add(QStringLiteral("text"), QStringLiteral("Text")));
    reference.setGroup(QStringLiteral("Content"));
    QVERIFY(reference.add(hover.name, hover.label, hover.description));
    QVERIFY(reference.icon(icon.name, icon.label, icon.iconResource, icon.defaultIconResource));
    QVERIFY(reference.add(size.name, size.label));
    QVERIFY(reference.add(shortcut.name, shortcut.label));
    reference.setGroup(QStringLiteral("Shape"));
    QVERIFY(reference.add(radius.name, radius.label));
    QVERIFY(reference.enumeration(placement.name, placement.label, placement.enumScope));
    QVERIFY(reference.alignment(alignment.name, alignment.label));
    reference.setGroup(QStringLiteral("Existing"));
    QVERIFY(reference.add(animate.name, animate.label));
    QVERIFY(reference.enabledWhen(hover.name, animate.name));
    QVERIFY(reference.enabledWhen(hover.name, placement.name, QStringLiteral("AfterText")));

    QList<StyleProperty> properties = builder.properties();
    const QList<StyleProperty> expected = reference.properties();
    QCOMPARE(properties.size(), expected.size());
    for (int index = 0; index < properties.size(); ++index) {
        const StyleProperty& actual = properties.at(index);
        const StyleProperty& original = expected.at(index);
        QCOMPARE(actual.name, original.name);
        QCOMPARE(actual.group, original.group);
        QCOMPARE(actual.label, original.label);
        QCOMPARE(actual.description, original.description);
        QCOMPARE(actual.type, original.type);
        QCOMPARE(actual.value, original.value);
        QCOMPARE(actual.defaultValue, original.defaultValue);
        QCOMPARE(actual.enumNames.size(), original.enumNames.size());
        QVERIFY(std::equal(actual.enumNames.cbegin(), actual.enumNames.cend(), original.enumNames.cbegin()));
        QCOMPARE(actual.enumExpressions.size(), original.enumExpressions.size());
        QVERIFY(std::equal(actual.enumExpressions.cbegin(), actual.enumExpressions.cend(), original.enumExpressions.cbegin()));
        QCOMPARE(actual.codeExpression(actual.value), original.codeExpression(original.value));
        QCOMPARE(actual.enabledWhen.size(), original.enabledWhen.size());
        for (int condition = 0; condition < actual.enabledWhen.size(); ++condition) {
            QCOMPARE(actual.enabledWhen.at(condition).propertyName, original.enabledWhen.at(condition).propertyName);
            QCOMPARE(actual.enabledWhen.at(condition).value, original.enabledWhen.at(condition).value);
        }
    }

    QVERIFY(!StylePropertyStateUtils::isEnabled(properties.at(1), properties));
    properties[8].value = true;
    properties[6].value = QStringLiteral("AfterText");
    QVERIFY(StylePropertyStateUtils::isEnabled(properties.at(1), properties));
    properties.at(5).apply(17);
    QCOMPARE(object.cornerRadius, 17);
    properties.at(6).apply(QStringLiteral("AfterText"));
    QCOMPARE(object.placement, StylePropertyTestObject::Placement::AfterText);
    properties.at(7).apply(QStringLiteral("AlignLeft|AlignBottom"));
    QCOMPARE(object.contentAlignment, Qt::Alignment(Qt::AlignLeft | Qt::AlignBottom));
    QCOMPARE(defaults.cornerRadius, 6);

    QVERIFY(builder.add(QStringLiteral("borderWidth"), QStringLiteral("Width")));
    QCOMPARE(builder.properties().last().group, QStringLiteral("Existing"));
}

void StylePropertyBuilderTest::structuredExplicitEnumOptions() {
    StylePropertyTestObject defaults;
    StylePropertyTestObject object;
    StylePropertyBuilder builder(&object, &defaults);

    StylePropertyOptions alignment;
    alignment.name = QStringLiteral("contentAlignment");
    alignment.label = QStringLiteral("Alignment");
    alignment.binding = StylePropertyOptions::Binding::Enumeration;
    alignment.enumOptions = {
        {QStringLiteral("Center"), Qt::AlignCenter, QStringLiteral("Qt::AlignCenter")},
        {QStringLiteral("Left"), Qt::AlignLeft, QStringLiteral("Qt::AlignLeft")}
    };

    StylePropertyGroup group;
    group.name = QStringLiteral("Layout");
    group.properties = {alignment};
    QVERIFY(builder.addGroups({group}));
    QVERIFY(builder.errors().isEmpty());
    const StyleProperty property = builder.properties().first();
    QCOMPARE(property.type, StylePropertyType::Enum);
    QCOMPARE(property.defaultValue.toString(), QStringLiteral("Center"));
    QCOMPARE(property.codeExpression(QStringLiteral("Left")), QStringLiteral("Qt::AlignLeft"));
    property.apply(QStringLiteral("Left"));
    QCOMPARE(object.contentAlignment, Qt::Alignment(Qt::AlignLeft));
}

void StylePropertyBuilderTest::structuredGroupsAreAtomic_data() {
    QTest::addColumn<QString>("failure");
    QTest::newRow("duplicate-name") << QStringLiteral("duplicate");
    QTest::newRow("missing-controller") << QStringLiteral("missing");
    QTest::newRow("self-dependency") << QStringLiteral("self");
    QTest::newRow("duplicate-controller") << QStringLiteral("condition");
    QTest::newRow("incomplete-enum") << QStringLiteral("enum");
    QTest::newRow("wrong-binding") << QStringLiteral("icon");
}

void StylePropertyBuilderTest::structuredGroupsAreAtomic() {
    QFETCH(QString, failure);
    StylePropertyTestObject defaults;
    StylePropertyTestObject object;
    StylePropertyBuilder builder(&object, &defaults);
    builder.setGroup(QStringLiteral("Existing"));
    QVERIFY(builder.add(QStringLiteral("text"), QStringLiteral("Text")));

    StylePropertyOptions radius;
    radius.name = QStringLiteral("cornerRadius");
    radius.label = QStringLiteral("Radius");

    StylePropertyOptions hover;
    hover.name = QStringLiteral("backgroundHover");
    hover.label = QStringLiteral("Hover");

    if (failure == QStringLiteral("duplicate"))
        hover.name = QStringLiteral("text");
    else if (failure == QStringLiteral("missing"))
        hover.enabledWhen = {{QStringLiteral("missing"), true}};
    else if (failure == QStringLiteral("self"))
        hover.enabledWhen = {{hover.name, true}};
    else if (failure == QStringLiteral("condition"))
        hover.enabledWhen = {{radius.name, 6}, {radius.name, 6}};
    else if (failure == QStringLiteral("enum"))
        hover.binding = StylePropertyOptions::Binding::Enumeration;
    else if (failure == QStringLiteral("icon"))
        hover.binding = StylePropertyOptions::Binding::Icon;

    StylePropertyGroup shape;
    shape.name = QStringLiteral("Shape");
    shape.properties = {radius};

    StylePropertyGroup content;
    content.name = QStringLiteral("Content");
    content.properties = {hover};
    QVERIFY(!builder.addGroups({shape, content}));
    QVERIFY(!builder.errors().isEmpty());
    QCOMPARE(builder.properties().size(), 1);
    QCOMPARE(builder.properties().first().name, QStringLiteral("text"));
    QCOMPARE(object.cornerRadius, defaults.cornerRadius);
    QVERIFY(!object.backgroundHover.isValid());

    QVERIFY(builder.add(QStringLiteral("borderWidth"), QStringLiteral("Width")));
    QCOMPARE(builder.properties().last().group, QStringLiteral("Existing"));
}

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
    const auto& realExpression = properties.at(1).codeExpression;
    QCOMPARE(realExpression(object.borderWidth), QStringLiteral("1.23"));
    QCOMPARE(realExpression(3.0799999999999996), QStringLiteral("3.08"));
    QCOMPARE(realExpression(3.10), QStringLiteral("3.1"));
    QCOMPARE(realExpression(100.0), QStringLiteral("100"));
    QCOMPARE(realExpression(-0.004), QStringLiteral("0"));
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
    QCOMPARE(properties.at(1).type, StylePropertyType::Icon);
    const QString text = QStringLiteral("A\"B\\C\nD\rE\tF");
    QCOMPARE(properties.at(0).codeExpression(text), QStringLiteral("QStringLiteral(\"A\\\"B\\\\C\\nD\\rE\\tF\")"));
    QCOMPARE(properties.at(1).codeExpression(QStringLiteral(":/icons/a\"b.svg")), QStringLiteral("QIcon(QStringLiteral(\":/icons/a\\\"b.svg\"))"));
    QCOMPARE(properties.at(1).codeExpression(QString()), QStringLiteral("QIcon()"));
}

void StylePropertyBuilderTest::sizeAndShortcutBindingsRoundTrip() {
    StylePropertyTestObject defaults;
    auto* object = new StylePropertyTestObject;
    StylePropertyBuilder builder(object, &defaults);
    QVERIFY(builder.add(QStringLiteral("iconSize"), QStringLiteral("Icon size")));
    QVERIFY(builder.add(QStringLiteral("shortcut"), QStringLiteral("Shortcut")));
    QVERIFY(builder.errors().isEmpty());

    StyleEditorItem item;
    item.typeName = QStringLiteral("ExampleButton");
    item.properties = builder.properties();
    QCOMPARE(item.properties.at(0).type, StylePropertyType::Size);
    QCOMPARE(item.properties.at(1).type, StylePropertyType::KeySequence);
    QVERIFY(!StyleCodeGenerator::generate(item).contains(QStringLiteral("widget->set")));

    item.properties[0].value = QSize(24, 20);
    item.properties[1].value = QStringLiteral("Ctrl+Return");
    const QJsonObject stored = StyleDocumentCodec::serializeProperties(item.properties);
    item.properties = builder.properties();
    StyleDocumentCodec::applySerializedProperties(item.properties, stored);
    QCOMPARE(object->iconSize, QSize(24, 20));
    QCOMPARE(object->shortcut.toString(QKeySequence::PortableText), QStringLiteral("Ctrl+Return"));
    const QString code = StyleCodeGenerator::generate(item);
    QVERIFY(code.contains(QStringLiteral("setIconSize(QSize(24, 20))")));
    QVERIFY(code.contains(QStringLiteral("setShortcut(QKeySequence(QStringLiteral(\"Ctrl+Return\"), QKeySequence::PortableText))")));

    QJsonObject invalid = stored;
    QJsonObject size;
    size.insert(QStringLiteral("width"), -1);
    size.insert(QStringLiteral("height"), 32);
    invalid.insert(QStringLiteral("iconSize"), size);
    StyleDocumentCodec::applySerializedProperties(item.properties, invalid);
    QCOMPARE(object->iconSize, QSize(24, 20));

    for (auto& property : item.properties)
        property.apply(property.defaultValue);
    QCOMPARE(object->iconSize, QSize(16, 16));
    QVERIFY(object->shortcut.isEmpty());
    delete object;
    item.properties.at(0).apply(QSize(32, 32));
    item.properties.at(1).apply(QStringLiteral("Ctrl+A"));
}

void StylePropertyBuilderTest::iconHintPersistenceAndSparseExport() {
    StylePropertyTestObject defaults;
    StylePropertyTestObject object;
    StylePropertyBuilder builder(&object, &defaults);
    QVERIFY(builder.add(QStringLiteral("iconHint"), QStringLiteral("Tint icon")));

    StyleEditorItem item;
    item.typeName = QStringLiteral("ExampleButton");
    item.properties = builder.properties();
    QVERIFY(!StyleCodeGenerator::generate(item).contains(QStringLiteral("setIconHint")));

    item.properties[0].value = true;
    const QJsonObject stored = StyleDocumentCodec::serializeProperties(item.properties);
    item.properties = builder.properties();
    StyleDocumentCodec::applySerializedProperties(item.properties, stored);
    QVERIFY(object.iconHint);
    QVERIFY(StyleCodeGenerator::generate(item).contains(QStringLiteral("setIconHint(true)")));

    item.properties[0].value = false;
    item.properties[0].apply(false);
    QVERIFY(!object.iconHint);
    QVERIFY(!StyleCodeGenerator::generate(item).contains(QStringLiteral("setIconHint")));
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

void StylePropertyBuilderTest::styleMembersPreserveOtherFields() {
    using Object = StylePropertyTestObject;
    using Appearance = Object::Appearance;
    Object defaults;
    auto* object = new Object;
    StylePropertyBuilder builder(object, &defaults);
    const auto getter = [](Object* target) { return target->appearance(); };
    const auto setter = [](Object* target, const Appearance& style) { target->setAppearance(style); };
    const QString getterCode = QStringLiteral("widget->appearance()");
    const QString setterCode = QStringLiteral("setAppearance(style)");

    StyleObjectProperties::add<Object>(builder, QStringLiteral("Appearance"), QStringLiteral("style.background"), QStringLiteral("Background"),
        &Appearance::background, getter, setter, getterCode, setterCode);
    StyleObjectProperties::add<Object>(builder, QStringLiteral("Appearance"), QStringLiteral("style.radius"), QStringLiteral("Radius"),
        &Appearance::radius, getter, setter, getterCode, setterCode);
    StyleObjectProperties::margins<Object, Appearance>(builder, QStringLiteral("Padding"), QStringLiteral("style.padding"),
        &Appearance::padding, getter, setter, getterCode, setterCode);
    StyleObjectProperties::choice<Object>(builder, QStringLiteral("Appearance"), QStringLiteral("style.checkAlignment"), QStringLiteral("Alignment"),
        &Appearance::checkAlignment, getter, setter,
        {{QStringLiteral("Left"), Qt::AlignLeft, QStringLiteral("Qt::AlignLeft")},
         {QStringLiteral("Right"), Qt::AlignRight, QStringLiteral("Qt::AlignRight")}},
        getterCode, setterCode);
    QVERIFY(builder.errors().isEmpty());
    const auto properties = builder.properties();
    QCOMPARE(properties.size(), 7);
    QCOMPARE(properties.first().defaultValue.value<QColor>(), QColor(Qt::white));
    properties.at(0).apply(QColor(Qt::blue));
    properties.at(1).apply(12);
    properties.at(2).apply(9);
    properties.at(6).apply(QStringLiteral("Right"));
    QCOMPARE(object->appearance().background, QColor(Qt::blue));
    QCOMPARE(object->appearance().radius, 12);
    QCOMPARE(object->appearance().padding, QMargins(9, 3, 4, 5));
    QCOMPARE(object->appearance().checkAlignment, Qt::Alignment(Qt::AlignRight));
    QVERIFY(properties.at(2).codeStatement(9).contains(QStringLiteral("style.padding.setLeft(9);")));
    QVERIFY(properties.at(6).codeStatement(QStringLiteral("Right")).contains(QStringLiteral("style.checkAlignment = Qt::AlignRight;")));
    QVERIFY(properties.first().codeStatement(QColor(Qt::blue)).contains(QStringLiteral("auto style = widget->appearance();")));

    delete object;
    for (const auto& property : properties)
        property.apply(property.defaultValue);
}

void StylePropertyBuilderTest::customStatementsAndSparseExport() {
    StylePropertyTestObject defaults;
    StylePropertyTestObject object;
    StylePropertyBuilder builder(&object, &defaults);
    StyleProperty statement;
    statement.name = QStringLiteral("editor.text");
    statement.type = StylePropertyType::Text;
    statement.value = QString();
    statement.defaultValue = QString();
    statement.apply = [&object](const QVariant& value) { object.text = value.toString(); };
    statement.codeStatement = [](const QVariant& value) {
        return QStringLiteral("widget->editor()->setText(%1);\n").arg(animatedwidgets::styleeditor::StyleCodeExpressions::text(value.toString()));
    };
    QVERIFY(builder.add(statement));

    StyleEditorItem item;
    item.typeName = QStringLiteral("ExampleComposite");
    item.properties = builder.properties();
    QVERIFY(!StyleCodeGenerator::generate(item).contains(QStringLiteral("setText")));
    QJsonObject values;
    values.insert(QStringLiteral("editor.text"), QStringLiteral("Preview"));
    StyleDocumentCodec::applySerializedProperties(item.properties, values);
    QCOMPARE(object.text, QStringLiteral("Preview"));
    QVERIFY(StyleCodeGenerator::generate(item).contains(QStringLiteral("widget->editor()->setText(QStringLiteral(\"Preview\"));")));
    QVERIFY(!StyleCodeGenerator::generate(item).contains(QStringLiteral("setEditor.text")));
    QCOMPARE(StyleDocumentCodec::serializeProperties(item.properties).value(QStringLiteral("editor.text")).toString(), QStringLiteral("Preview"));
}

QTEST_APPLESS_MAIN(StylePropertyBuilderTest)

#include "tststylepropertybuilder.moc"
