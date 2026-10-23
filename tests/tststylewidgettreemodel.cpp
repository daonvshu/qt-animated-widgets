#include "../styleeditor/panels/stylewidgettreemodel.h"
#include "../styleeditor/utils/stylewidgettypeutils.h"
#include "animatedwidgets/styleeditor/api/stylewidgettypebuilder.h"

#include <QtTest>

#include <algorithm>
#include <limits>

using animatedwidgets::styleeditor::StyleEditorWidgetType;

class StyleWidgetTreeModelTest : public QObject {
    Q_OBJECT

private slots:
    void categoriesAndDocumentIndices();
    void searchByCategoryStyleAndType();
    void unavailableTypesAndStableIds();
    void floatingPointOrderAndDocumentIndices();
    void sharedCategoriesAndDeterministicTies();
    void nonFiniteOrdersUseDefaults();
    void builderForwardsOrderMetadata();

private:
    static StyleEditorWidgetType widgetType(const QString& id, const QString& name, const QString& category,
                                           double categoryOrder = 1000.0, double order = 1000.0);
    static StyleDocumentItem documentItem(const QString& id, const QString& name, const QString& typeId);
};

StyleEditorWidgetType StyleWidgetTreeModelTest::widgetType(const QString& id, const QString& name, const QString& category,
                                                          double categoryOrder, double order) {
    StyleEditorWidgetType type;
    type.id = id;
    type.name = name;
    type.category = category;
    type.categoryOrder = categoryOrder;
    type.order = order;
    return type;
}

StyleDocumentItem StyleWidgetTreeModelTest::documentItem(const QString& id, const QString& name, const QString& typeId) {
    StyleDocumentItem item;
    item.id = id;
    item.displayName = name;
    item.widgetTypeId = typeId;
    return item;
}

void StyleWidgetTreeModelTest::categoriesAndDocumentIndices() {
    const QList<StyleEditorWidgetType> types{
        widgetType(QStringLiteral("filled"), QStringLiteral("Filled button"), QStringLiteral("Buttons")),
        widgetType(QStringLiteral("outline"), QStringLiteral("Outline button"), QStringLiteral("Buttons")),
        widgetType(QStringLiteral("line"), QStringLiteral("Line edit"), QStringLiteral("Inputs")),
        widgetType(QStringLiteral("custom"), QStringLiteral("Custom widget"), QStringLiteral("  "))};
    const QList<StyleDocumentItem> items{
        documentItem(QStringLiteral("input"), QStringLiteral("Search"), QStringLiteral("line")),
        documentItem(QStringLiteral("button"), QStringLiteral("Primary"), QStringLiteral("filled"))};
    StyleWidgetTreeModel model;
    model.rebuild(types, items, QString());

    QCOMPARE(model.rowCount(), 3);
    QCOMPARE(model.index(0, 0).data().toString(), QStringLiteral("Buttons"));
    QCOMPARE(model.index(1, 0).data().toString(), QStringLiteral("Inputs"));
    QCOMPARE(model.index(2, 0).data().toString(), QStringLiteral("Other widgets"));
    QCOMPARE(model.rowCount(model.index(2, 0)), 0);
    QVERIFY(model.index(2, 0).data(StyleWidgetTreeModel::ActionEnabledRole).toBool());
    QVERIFY(!(model.flags(model.index(0, 0)) & Qt::ItemIsSelectable));

    const QModelIndex button = model.indexForItem(QStringLiteral("button"));
    QCOMPARE(button.parent(), model.index(0, 0));
    QCOMPARE(button.data(StyleWidgetTreeModel::ItemIndexRole).toInt(), 1);
    QCOMPARE(button.data(StyleWidgetTreeModel::DetailRole).toString(), QStringLiteral("Filled button"));
    QVERIFY(model.flags(button) & Qt::ItemIsSelectable);
    QCOMPARE(model.indexForItem(QStringLiteral("input")).data(StyleWidgetTreeModel::ItemIndexRole).toInt(), 0);
    QVERIFY(!model.indexForItem(QString()).isValid());
    QVERIFY(!model.indexForItem(QStringLiteral("missing")).isValid());
}

void StyleWidgetTreeModelTest::searchByCategoryStyleAndType() {
    const QList<StyleEditorWidgetType> types{
        widgetType(QStringLiteral("filled"), QStringLiteral("Filled button"), QStringLiteral("Buttons")),
        widgetType(QStringLiteral("outline"), QStringLiteral("Outline button"), QStringLiteral("Buttons")),
        widgetType(QStringLiteral("line"), QStringLiteral("Line edit"), QStringLiteral("Inputs"))};
    const QList<StyleDocumentItem> items{
        documentItem(QStringLiteral("first"), QStringLiteral("Primary"), QStringLiteral("filled")),
        documentItem(QStringLiteral("second"), QStringLiteral("Secondary"), QStringLiteral("outline"))};
    StyleWidgetTreeModel model;
    model.rebuild(types, items, QStringLiteral("  pRiMaRy  "));
    QCOMPARE(model.rowCount(), 1);
    QCOMPARE(model.rowCount(model.index(0, 0)), 1);
    QVERIFY(model.indexForItem(QStringLiteral("first")).isValid());
    QVERIFY(!model.indexForItem(QStringLiteral("second")).isValid());

    model.rebuild(types, items, QStringLiteral("outline"));
    QCOMPARE(model.indexForItem(QStringLiteral("second")).data(StyleWidgetTreeModel::ItemIndexRole).toInt(), 1);
    model.rebuild(types, items, QStringLiteral("BUTTONS"));
    QCOMPARE(model.rowCount(model.index(0, 0)), 2);
    model.rebuild(types, items, QStringLiteral("Inputs"));
    QCOMPARE(model.rowCount(), 1);
    QCOMPARE(model.rowCount(model.index(0, 0)), 0);
    model.rebuild(types, items, QStringLiteral("no results"));
    QCOMPARE(model.rowCount(), 0);
}

void StyleWidgetTreeModelTest::unavailableTypesAndStableIds() {
    const QList<StyleEditorWidgetType> types{
        widgetType(QStringLiteral("filled"), QStringLiteral("Filled button"), QStringLiteral("Buttons")),
        widgetType(QStringLiteral("line"), QStringLiteral("Line edit"), QStringLiteral("Inputs"))};
    QList<StyleDocumentItem> items{
        documentItem(QStringLiteral("stable"), QStringLiteral("Primary"), QStringLiteral("filled")),
        documentItem(QStringLiteral("external"), QStringLiteral("External"), QStringLiteral("missing-plugin"))};
    StyleWidgetTreeModel model;
    model.rebuild(types, items, QString());
    const QPersistentModelIndex oldIndex(model.indexForItem(QStringLiteral("stable")));
    const QModelIndex unavailable = model.indexForItem(QStringLiteral("external"));
    QCOMPARE(unavailable.parent().data().toString(), QStringLiteral("Unavailable widgets"));
    QVERIFY(!unavailable.parent().data(StyleWidgetTreeModel::ActionEnabledRole).toBool());
    QVERIFY(unavailable.data(StyleWidgetTreeModel::ActionEnabledRole).toBool());

    items[0].displayName = QStringLiteral("Renamed input");
    items[0].widgetTypeId = QStringLiteral("line");
    model.rebuild(types, items, QString());
    QVERIFY(!oldIndex.isValid());
    const QModelIndex renamed = model.indexForItem(QStringLiteral("stable"));
    QCOMPARE(renamed.parent().data().toString(), QStringLiteral("Inputs"));
    QCOMPARE(renamed.data().toString(), QStringLiteral("Renamed input"));
    QCOMPARE(renamed.data(StyleWidgetTreeModel::TypeIdRole).toString(), QStringLiteral("line"));
    QCOMPARE(renamed.data(StyleWidgetTreeModel::ItemIndexRole).toInt(), 0);

    model.rebuild({}, items, QString());
    QCOMPARE(model.rowCount(), 1);
    QVERIFY(!model.indexForItem(QStringLiteral("stable")).data(StyleWidgetTreeModel::ActionEnabledRole).toBool());
    QCOMPARE(items.at(1).widgetTypeId, QStringLiteral("missing-plugin"));
}

void StyleWidgetTreeModelTest::floatingPointOrderAndDocumentIndices() {
    QList<StyleEditorWidgetType> types{
        widgetType(QStringLiteral("input"), QStringLiteral("Input"), QStringLiteral("Inputs"), 200.0),
        widgetType(QStringLiteral("late"), QStringLiteral("A late button"), QStringLiteral("Buttons"), 100.0, 200.0),
        widgetType(QStringLiteral("custom"), QStringLiteral("Custom"), QStringLiteral("Custom widgets"), 150.5),
        widgetType(QStringLiteral("middle"), QStringLiteral("Z middle button"), QStringLiteral("Buttons"), 100.0, 100.5),
        widgetType(QStringLiteral("early"), QStringLiteral("Early button"), QStringLiteral("Buttons"), 100.0, 100.25)};
    StyleWidgetTypeUtils::sort(types);
    const QStringList expected = {QStringLiteral("early"), QStringLiteral("middle"), QStringLiteral("late"),
                                  QStringLiteral("custom"), QStringLiteral("input")};
    QCOMPARE(types.size(), expected.size());
    for (int index = 0; index < types.size(); ++index)
        QCOMPARE(types.at(index).id, expected.at(index));

    const QList<StyleDocumentItem> items{
        documentItem(QStringLiteral("late-style"), QStringLiteral("Z first created"), QStringLiteral("late")),
        documentItem(QStringLiteral("early-style"), QStringLiteral("A second created"), QStringLiteral("early")),
        documentItem(QStringLiteral("missing"), QStringLiteral("Missing"), QStringLiteral("external"))};
    StyleWidgetTreeModel model;
    model.rebuild(types, items, QString());
    QCOMPARE(model.index(0, 0).data().toString(), QStringLiteral("Buttons"));
    QCOMPARE(model.index(1, 0).data().toString(), QStringLiteral("Custom widgets"));
    QCOMPARE(model.index(2, 0).data().toString(), QStringLiteral("Inputs"));
    QCOMPARE(model.index(3, 0).data().toString(), QStringLiteral("Unavailable widgets"));
    const QModelIndex late = model.indexForItem(QStringLiteral("late-style"));
    const QModelIndex early = model.indexForItem(QStringLiteral("early-style"));
    QCOMPARE(late.row(), 0);
    QCOMPARE(early.row(), 1);
    QCOMPARE(late.data(StyleWidgetTreeModel::ItemIndexRole).toInt(), 0);
    QCOMPARE(early.data(StyleWidgetTreeModel::ItemIndexRole).toInt(), 1);
}

void StyleWidgetTreeModelTest::sharedCategoriesAndDeterministicTies() {
    QList<StyleEditorWidgetType> types{
        widgetType(QStringLiteral("z"), QStringLiteral("First name"), QStringLiteral("Buttons"), 900.0, 50.5),
        widgetType(QStringLiteral("input"), QStringLiteral("Input"), QStringLiteral("Inputs"), 150.0),
        widgetType(QStringLiteral("a"), QStringLiteral("Last name"), QStringLiteral("Buttons"), 100.0, 50.5),
        widgetType(QStringLiteral("alpha"), QStringLiteral("Alpha"), QStringLiteral("Alpha"), 100.0)};
    const QStringList expected = {QStringLiteral("alpha"), QStringLiteral("a"), QStringLiteral("z"), QStringLiteral("input")};
    for (int pass = 0; pass < 2; ++pass) {
        StyleWidgetTypeUtils::sort(types);
        for (int index = 0; index < types.size(); ++index)
            QCOMPARE(types.at(index).id, expected.at(index));
        std::reverse(types.begin(), types.end());
    }
}

void StyleWidgetTreeModelTest::nonFiniteOrdersUseDefaults() {
    const double nan = std::numeric_limits<double>::quiet_NaN();
    const double infinity = std::numeric_limits<double>::infinity();
    QList<StyleEditorWidgetType> types{
        widgetType(QStringLiteral("nan"), QStringLiteral("NaN"), QStringLiteral("Other widgets"), nan, nan),
        widgetType(QStringLiteral("negative"), QStringLiteral("Negative"), QStringLiteral("Other widgets"), 1000.0, -0.5),
        widgetType(QStringLiteral("infinity"), QStringLiteral("Infinity"), QStringLiteral("  "), infinity, -infinity),
        widgetType(QStringLiteral("last"), QStringLiteral("Last"), QStringLiteral("Other widgets"), 1000.0, 1000.5),
        widgetType(QStringLiteral("first"), QStringLiteral("First"), QStringLiteral("First"), 999.5)};
    StyleWidgetTypeUtils::sort(types);
    const QStringList expected = {QStringLiteral("first"), QStringLiteral("negative"), QStringLiteral("infinity"),
                                  QStringLiteral("nan"), QStringLiteral("last")};
    for (int index = 0; index < types.size(); ++index)
        QCOMPARE(types.at(index).id, expected.at(index));
}

void StyleWidgetTreeModelTest::builderForwardsOrderMetadata() {
    using animatedwidgets::styleeditor::StyleWidgetTypeBuilder;
    using animatedwidgets::styleeditor::StyleWidgetTypeOptions;

    StyleWidgetTypeOptions options;
    const StyleEditorWidgetType defaults = StyleWidgetTypeBuilder::create<QWidget>(options);
    QCOMPARE(defaults.categoryOrder, 1000.0);
    QCOMPARE(defaults.order, 1000.0);

    options.categoryOrder = 150.5;
    options.order = 100.25;
    const StyleEditorWidgetType custom = StyleWidgetTypeBuilder::create<QWidget>(options);
    QCOMPARE(custom.categoryOrder, options.categoryOrder);
    QCOMPARE(custom.order, options.order);
}

QTEST_APPLESS_MAIN(StyleWidgetTreeModelTest)

#include "tststylewidgettreemodel.moc"
