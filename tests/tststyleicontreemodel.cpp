#include "../styleeditor/panels/styleicontreemodel.h"

#include <QtTest>

class StyleIconTreeModelTest : public QObject {
    Q_OBJECT

private slots:
    void qrcCategoriesAndAliases();
    void directoryPathsAndFiltering();
    void rebuildAndDuplicateReferences();
};

void StyleIconTreeModelTest::qrcCategoriesAndAliases() {
    const QList<StyleResourceEntry> entries{
        {QStringLiteral(":/actions/open.svg"), QStringLiteral("open.svg"), QStringLiteral("/actions")},
        {QStringLiteral(":/actions/files/save.svg"), QStringLiteral("save.svg"), QStringLiteral("/actions")},
        {QStringLiteral(":/actions/files/close.svg"), QStringLiteral("close.svg"), QStringLiteral("/actions/files")},
        {QStringLiteral(":/root.svg"), QStringLiteral("root.svg"), QStringLiteral("/")}};
    StyleIconTreeModel model;
    model.rebuild(entries, QString());

    QCOMPARE(model.iconCount(), 4);
    QCOMPARE(model.rowCount(), 3);
    const QModelIndex actions = model.index(0, 0);
    QCOMPARE(actions.data().toString(), QStringLiteral("/actions"));
    QVERIFY(!(model.flags(actions) & Qt::ItemIsSelectable));
    const QModelIndex save = model.indexForReference(QStringLiteral(":/actions/files/save.svg"));
    QCOMPARE(save.data().toString(), QStringLiteral("save.svg"));
    QCOMPARE(save.data(StyleIconTreeModel::FilePathRole).toString(), QStringLiteral("save.svg"));
    QCOMPARE(save.parent().data().toString(), QStringLiteral("files"));
    QCOMPARE(save.parent().parent(), actions);
    QVERIFY(model.flags(save) & Qt::ItemIsSelectable);

    const QModelIndex close = model.indexForReference(QStringLiteral(":/actions/files/close.svg"));
    QCOMPARE(close.parent().data().toString(), QStringLiteral("/actions/files"));
    QVERIFY(!close.parent().parent().isValid());
    QCOMPARE(model.indexForReference(QStringLiteral(":/root.svg")).parent().data().toString(), QStringLiteral("/"));
}

void StyleIconTreeModelTest::directoryPathsAndFiltering() {
    const QList<StyleResourceEntry> entries{
        {QStringLiteral("resources/actions/open.svg"), QStringLiteral("open.svg"), {}},
        {QStringLiteral("resources/actions/save.svg"), QStringLiteral("save.svg"), {}},
        {QStringLiteral("resources/status/ready.svg"), QStringLiteral("ready.svg"), {}}};
    StyleIconTreeModel model;
    model.rebuild(entries, QStringLiteral("  OpEn  "));

    QCOMPARE(model.iconCount(), 1);
    const QModelIndex open = model.indexForReference(QStringLiteral("resources/actions/open.svg"));
    QVERIFY(open.isValid());
    QCOMPARE(open.parent().data().toString(), QStringLiteral("actions"));
    QCOMPARE(open.parent().parent().data().toString(), QStringLiteral("resources"));
    QVERIFY(!model.indexForReference(QStringLiteral("resources/status/ready.svg")).isValid());

    model.rebuild(entries, QStringLiteral("actions"));
    QCOMPARE(model.iconCount(), 2);
    QCOMPARE(model.rowCount(model.index(0, 0)), 1);
    model.rebuild(entries, QStringLiteral("missing"));
    QCOMPARE(model.iconCount(), 0);
    QCOMPARE(model.rowCount(), 0);
}

void StyleIconTreeModelTest::rebuildAndDuplicateReferences() {
    const StyleResourceEntry entry{QStringLiteral("open.svg"), QStringLiteral("open.svg"), {}};
    StyleIconTreeModel model;
    model.rebuild({entry, entry}, QString());
    QCOMPARE(model.iconCount(), 1);
    QCOMPARE(model.rowCount(), 1);
    QPersistentModelIndex old = model.indexForReference(entry.reference);

    model.rebuild({}, QString());
    QVERIFY(!old.isValid());
    QVERIFY(!model.indexForReference(entry.reference).isValid());
    QCOMPARE(model.iconCount(), 0);
}

QTEST_APPLESS_MAIN(StyleIconTreeModelTest)

#include "tststyleicontreemodel.moc"
