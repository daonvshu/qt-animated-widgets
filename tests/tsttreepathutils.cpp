#include "animatedwidgets/utils/treemodelutils.h"

#include <QPersistentModelIndex>
#include <QStandardItemModel>
#include <QtTest>

using namespace animatedwidgets;

class TreePathUtilsTest : public QObject {
    Q_OBJECT

private slots:
    void pathRelativeToRoot();
    void invalidAndForeignRoots();
    void persistentSelectionAfterChanges();
    void cascadeColumnsFollowActiveBranch();
};

void TreePathUtilsTest::pathRelativeToRoot() {
    QStandardItemModel model;
    auto* category = new QStandardItem(QStringLiteral("Creative"));
    auto* branch = new QStandardItem(QStringLiteral("Design"));
    auto* leaf = new QStandardItem(QStringLiteral("Illustration"));
    leaf->setData(42, Qt::UserRole);
    branch->appendRow(leaf);
    category->appendRow(branch);
    model.appendRow(category);

    const auto full = TreeModelUtils::path(leaf->index());
    QCOMPARE(full.size(), 3);
    QCOMPARE(full.at(0), category->index());
    QCOMPARE(full.at(1), branch->index());
    QCOMPARE(full.at(2).data(Qt::UserRole).toInt(), 42);

    const auto relative = TreeModelUtils::path(leaf->index(), category->index());
    QCOMPARE(relative.size(), 2);
    QCOMPARE(relative.first(), branch->index());
    QCOMPARE(relative.last(), leaf->index());
    QCOMPARE(TreeModelUtils::path(category->index()).size(), 1);
}

void TreePathUtilsTest::invalidAndForeignRoots() {
    QStandardItemModel model;
    auto* first = new QStandardItem(QStringLiteral("First"));
    auto* second = new QStandardItem(QStringLiteral("Second"));
    model.appendRow(first);
    model.appendRow(second);
    QStandardItemModel other;
    other.appendRow(new QStandardItem(QStringLiteral("Other")));

    QVERIFY(TreeModelUtils::path(QModelIndex()).isEmpty());
    QVERIFY(TreeModelUtils::path(first->index(), first->index()).isEmpty());
    QVERIFY(TreeModelUtils::path(first->index(), second->index()).isEmpty());
    QVERIFY(TreeModelUtils::path(first->index(), other.index(0, 0)).isEmpty());
}

void TreePathUtilsTest::persistentSelectionAfterChanges() {
    QStandardItemModel model;
    auto* category = new QStandardItem(QStringLiteral("Category"));
    category->appendRow(new QStandardItem(QStringLiteral("Selected")));
    model.appendRow(category);
    QPersistentModelIndex selected(category->child(0)->index());

    category->insertRow(0, new QStandardItem(QStringLiteral("Inserted")));
    const auto path = TreeModelUtils::path(selected);
    QCOMPARE(path.size(), 2);
    QCOMPARE(path.last().row(), 1);
    QCOMPARE(path.last().data().toString(), QStringLiteral("Selected"));

    category->removeRow(1);
    QVERIFY(TreeModelUtils::path(selected).isEmpty());
}

void TreePathUtilsTest::cascadeColumnsFollowActiveBranch() {
    QStandardItemModel model;
    auto* creative = new QStandardItem(QStringLiteral("Creative"));
    auto* design = new QStandardItem(QStringLiteral("Design"));
    auto* leaf = new QStandardItem(QStringLiteral("Illustration"));
    design->appendRow(leaf);
    creative->appendRow(design);
    model.appendRow(creative);
    auto* engineering = new QStandardItem(QStringLiteral("Engineering"));
    for (int row = 0; row < 12; ++row)
        engineering->appendRow(new QStandardItem(QString::number(row)));
    model.appendRow(engineering);

    const auto initial = TreeModelUtils::columnRoots(QModelIndex());
    QCOMPARE(initial.size(), 1);
    QVERIFY(!initial.first().isValid());

    const auto selected = TreeModelUtils::columnRoots(leaf->index());
    QCOMPARE(selected.size(), 3);
    QCOMPARE(selected.at(1), creative->index());
    QCOMPARE(selected.at(2), design->index());
    // A terminal item adds no empty preview column.
    QCOMPARE(TreeModelUtils::columnRoots(design->index()).size(), selected.size());

    const auto switched = TreeModelUtils::columnRoots(engineering->index());
    QCOMPARE(switched.size(), 2);
    QCOMPARE(switched.last(), engineering->index());
    QCOMPARE(model.rowCount(switched.last()), 12);
    QCOMPARE(TreeModelUtils::columnRoots(leaf->index()).last(), design->index());

    const auto subtree = TreeModelUtils::columnRoots(leaf->index(), creative->index());
    QCOMPARE(subtree.size(), 2);
    QCOMPARE(subtree.first(), creative->index());
    QCOMPARE(subtree.last(), design->index());
}

QTEST_APPLESS_MAIN(TreePathUtilsTest)

#include "tsttreepathutils.moc"
