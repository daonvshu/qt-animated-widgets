#include "animatedwidgets/popups/groupedpopupmenumodel.h"
#include "animatedwidgets/popups/popupmenurowtype.h"

#include <QtTest>

using namespace animatedwidgets;
using SelectionIndexes = QList<QPair<int, int>>;

class GroupedPopupMenuModelTest : public QObject {
    Q_OBJECT

private slots:
    void mixedSelectionAndSignals();
    void logicalIndexesAndUntitledGroups();
    void updatesAndReset();
    void groupRoleData();
    void nestedGroupChildren();
};

void GroupedPopupMenuModelTest::mixedSelectionAndSignals() {
    GroupedPopupMenuModel model;
    PopupMenuGroup multiple;
    multiple.title = QStringLiteral("Filters");
    multiple.multiSelection = true;
    multiple.items = {PopupMenuItem{QStringLiteral("Images")}, PopupMenuItem{QStringLiteral("Documents")}};
    PopupMenuGroup single;
    single.items = {PopupMenuItem{QStringLiteral("Newest")}, PopupMenuItem{QStringLiteral("Oldest")}};
    model.setGroups({multiple, single});
    QSignalSpy changes(&model, &GroupedPopupMenuModel::selectedIndexesChanged);
    QSignalSpy singleChanges(&model, &GroupedPopupMenuModel::currentIndexChanged);

    model.triggerItem(model.indexForItem(0, 0));
    model.triggerItem(model.indexForItem(0, 1));
    model.triggerItem(model.indexForItem(1, 0));
    model.triggerItem(model.indexForItem(1, 1));
    const SelectionIndexes expected = {{0, 0}, {0, 1}, {1, 1}};
    QCOMPARE(model.selectedIndexes(), expected);
    QCOMPARE(qvariant_cast<SelectionIndexes>(changes.last().at(0)), expected);
    QCOMPARE(singleChanges.count(), 2);
    QCOMPARE(singleChanges.last().at(0).toInt(), 1);
    QCOMPARE(singleChanges.last().at(1).toInt(), 1);
    model.triggerItem(model.indexForItem(1, 1));
    QCOMPARE(changes.count(), 4);
    QCOMPARE(model.indexForItem(1, 0).data(Qt::CheckStateRole).toInt(), int(Qt::Unchecked));

    model.triggerItem(model.indexForItem(0, 0));
    QVERIFY(!model.selectedIndexes().contains(qMakePair(0, 0)));
    QVERIFY(model.selectedIndexes().contains(qMakePair(0, 1)));
    model.setCurrentIndex(1, -1);
    QCOMPARE(model.currentIndex(1), -1);
    QCOMPARE(singleChanges.last().at(1).toInt(), -1);
    QVERIFY(model.selectedIndexes().contains(qMakePair(0, 1)));
}

void GroupedPopupMenuModelTest::logicalIndexesAndUntitledGroups() {
    GroupedPopupMenuModel model;
    PopupMenuItem separator;
    separator.separator = true;
    PopupMenuGroup empty;
    PopupMenuGroup first;
    first.items = {separator, PopupMenuItem{QStringLiteral("A")}, separator, PopupMenuItem{QStringLiteral("B")}};
    PopupMenuGroup second;
    second.title = QStringLiteral("Title");
    second.multiSelection = true;
    second.items = {separator, PopupMenuItem{QStringLiteral("C")}, separator, PopupMenuItem{QStringLiteral("D")}};
    model.setGroups({empty, first, second});
    QCOMPARE(model.rowCount(), 9);
    QCOMPARE(model.indexForItem(1, 1).row(), 3);
    QCOMPARE(model.indexForItem(2, 1).row(), 8);
    QCOMPARE(model.logicalIndex(model.index(8)), qMakePair(2, 1));
    QCOMPARE(model.logicalIndex(model.index(4)), qMakePair(-1, -1));
    QVERIFY(!model.indexForItem(0, 0).isValid());
    QVERIFY(!model.indexForItem(2, 2).isValid());
    model.triggerItem(model.index(0));
    model.triggerItem(model.index(4));
    QVERIFY(model.selectedIndexes().isEmpty());

    model.setSelectedIndexes({{1, 0}, {1, 1}, {2, 0}, {2, 1}, {2, 1}, {-1, 0}, {0, 0}, {2, 2}});
    const SelectionIndexes expected = {{1, 1}, {2, 0}, {2, 1}};
    QCOMPARE(model.selectedIndexes(), expected);
    QCOMPARE(model.indexForItem(1, 1).data(Qt::CheckStateRole).toInt(), int(Qt::Checked));
    model.setItemEnabled(2, 1, false);
    model.triggerItem(model.indexForItem(2, 1));
    QCOMPARE(model.selectedIndexes(), expected);
    model.setCurrentIndex(2, 99);
    QCOMPARE(model.selectedIndexes(), expected);
}

void GroupedPopupMenuModelTest::updatesAndReset() {
    GroupedPopupMenuModel model;
    PopupMenuGroup group;
    group.title = QStringLiteral("Group");
    group.items = {PopupMenuItem{QStringLiteral("A")}};
    model.setGroups({group});
    const QModelIndex item = model.indexForItem(0, 0);
    model.setCurrentIndex(0, 0);
    QVERIFY(model.setData(item, QStringLiteral("Updated"), Qt::DisplayRole));
    QCOMPARE(model.groups().first().items.first().text, QStringLiteral("Updated"));
    QVERIFY(model.setData(item, 42, Qt::UserRole + 100));
    QCOMPARE(item.data(Qt::UserRole + 100).toInt(), 42);
    QVERIFY(!model.setData(item, static_cast<int>(PopupMenuRowType::GroupTitle), PopupMenuItem::TypeRole));
    QVERIFY(!model.setData(model.index(0), Qt::Checked, Qt::CheckStateRole));
    QVERIFY(!model.setData(item, Qt::PartiallyChecked, Qt::CheckStateRole));
    GroupedPopupMenuModel other;
    other.setGroups({group});
    model.triggerItem(other.indexForItem(0, 0));
    model.triggerItem(QModelIndex());
    QCOMPARE(model.currentIndex(0), 0);

    QSignalSpy changes(&model, &GroupedPopupMenuModel::selectedIndexesChanged);
    QSignalSpy singleChanges(&model, &GroupedPopupMenuModel::currentIndexChanged);
    model.setGroups({});
    QVERIFY(model.selectedIndexes().isEmpty());
    QCOMPARE(model.rowCount(), 0);
    QCOMPARE(changes.count(), 1);
    QCOMPARE(singleChanges.last().at(0).toInt(), 0);
    QCOMPARE(singleChanges.last().at(1).toInt(), -1);
}

void GroupedPopupMenuModelTest::groupRoleData() {
    constexpr int role = Qt::UserRole + 100;
    GroupedPopupMenuModel model;
    PopupMenuGroup titled;
    titled.title = QStringLiteral("Filters");
    titled.items = {PopupMenuItem{QStringLiteral("Image")}};
    titled.values.insert(role, 100);
    titled.values.insert(Qt::DisplayRole, QStringLiteral("Ignored title"));
    titled.values.insert(PopupMenuItem::TypeRole, static_cast<int>(PopupMenuRowType::Separator));
    titled.values.insert(PopupMenuItem::GroupIndexRole, 99);
    PopupMenuGroup untitled;
    untitled.items = {PopupMenuItem{QStringLiteral("Name")}};
    untitled.values.insert(role, 200);
    PopupMenuGroup empty;
    empty.values.insert(role, 300);
    model.setGroups({titled, untitled, empty});

    const QModelIndex heading = model.index(0);
    QCOMPARE(model.groupData(0, role).toInt(), 100);
    QCOMPARE(heading.data(role).toInt(), 100);
    QCOMPARE(heading.data().toString(), titled.title);
    QCOMPARE(heading.data(PopupMenuItem::TypeRole).toInt(), static_cast<int>(PopupMenuRowType::GroupTitle));
    QCOMPARE(heading.data(PopupMenuItem::GroupIndexRole).toInt(), 0);
    QVERIFY(!heading.data(Qt::CheckStateRole).isValid());
    QCOMPARE(model.groupData(1, role).toInt(), 200);
    QCOMPARE(model.groupData(2, role).toInt(), 300);
    QVERIFY(!model.indexForItem(0, 0).data(role).isValid());

    int selectedGroupId = -1;
    connect(&model, &GroupedPopupMenuModel::currentIndexChanged, &model,
            [&model, &selectedGroupId, role](int groupIndex, int) {
                selectedGroupId = model.groupData(groupIndex, role).toInt();
            });
    model.setCurrentIndex(1, 0);
    QCOMPARE(selectedGroupId, 200);
    QSignalSpy selectionChanges(&model, &GroupedPopupMenuModel::selectedIndexesChanged);
    QSignalSpy groupChanges(&model, &GroupedPopupMenuModel::groupDataChanged);
    QSignalSpy rowChanges(&model, &QAbstractItemModel::dataChanged);

    QVERIFY(model.setGroupData(0, 101, role));
    QCOMPARE(heading.data(role).toInt(), 101);
    QCOMPARE(model.groups().at(0).values.value(role).toInt(), 101);
    QCOMPARE(rowChanges.count(), 1);
    QCOMPARE(qvariant_cast<QModelIndex>(rowChanges.last().at(0)), heading);
    QCOMPARE(qvariant_cast<QVector<int>>(rowChanges.last().at(2)), QVector<int>({role}));
    QVERIFY(model.setGroupData(0, 101, role));
    QCOMPARE(groupChanges.count(), 1);
    QVERIFY(model.setData(heading, 102, role));
    QCOMPARE(model.groupData(0, role).toInt(), 102);
    QVERIFY(model.setGroupData(1, 201, role));
    QVERIFY(model.setGroupData(2, 301, role));
    QCOMPARE(rowChanges.count(), 2);
    QCOMPARE(groupChanges.last().at(0).toInt(), 2);
    QCOMPARE(groupChanges.last().at(1).toInt(), role);

    QVERIFY(model.setGroupData(0, QVariant(), role));
    QVERIFY(!heading.data(role).isValid());
    QVERIFY(!model.groups().at(0).values.contains(role));
    QVERIFY(!model.groupData(-1, role).isValid());
    QVERIFY(!model.setGroupData(3, 1, role));
    QVERIFY(!model.setGroupData(0, 1, PopupMenuItem::TypeRole));
    QVERIFY(!model.setGroupData(0, 1, PopupMenuItem::SeparatorRole));
    QVERIFY(!model.setGroupData(0, 1, PopupMenuItem::GroupIndexRole));
    QVERIFY(!model.setGroupData(0, Qt::Checked, Qt::CheckStateRole));
    QVERIFY(!model.setGroupData(0, QStringLiteral("New title"), Qt::DisplayRole));
    QCOMPARE(groupChanges.count(), 5);
    QCOMPARE(selectionChanges.count(), 0);
    QCOMPARE(model.currentIndex(1), 0);
    QCOMPARE(model.rowCount(), 3);
}

void GroupedPopupMenuModelTest::nestedGroupChildren() {
    PopupMenuItem separator;
    separator.separator = true;
    PopupMenuGroup nested;
    nested.title = QStringLiteral("More options");
    nested.children = {PopupMenuItem{QStringLiteral("Child")}};
    PopupMenuGroup root;
    root.items = {PopupMenuItem{QStringLiteral("First")}};
    root.children = {separator, nested, PopupMenuItem{QStringLiteral("Last")}};

    GroupedPopupMenuModel model;
    model.setGroups({root});
    QCOMPARE(model.rowCount(), 4);
    QCOMPARE(model.indexForItem(0, 1).data().toString(), nested.title);
    QVERIFY(!model.indexForItem(0, 1).data(Qt::CheckStateRole).isValid());
    const auto children = model.indexForItem(0, 1).data(PopupMenuItem::ChildrenRole).value<QList<PopupMenuNode>>();
    QCOMPARE(children.size(), 1);
    QCOMPARE(children.first().item()->text, QStringLiteral("Child"));
    model.setCurrentIndex(0, 0);
    model.triggerItem(model.indexForItem(0, 1));
    model.setCurrentIndex(0, 1);
    QCOMPARE(model.currentIndex(0), 0);
    model.setSelectedIndexes({{0, 0}, {0, 1}, {0, 2}});
    QCOMPARE(model.currentIndex(0), 2);

    // groups() returns canonical item entries; feeding them back must not duplicate children.
    model.setGroups(model.groups());
    QCOMPARE(model.rowCount(), 4);
    QVERIFY(!model.setGroupData(0, QVariant(), PopupMenuItem::ChildrenRole));
}

QTEST_APPLESS_MAIN(GroupedPopupMenuModelTest)

#include "tstgroupedpopupmenumodel.moc"
