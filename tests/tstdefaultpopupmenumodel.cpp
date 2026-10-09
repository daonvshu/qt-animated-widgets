#include "animatedwidgets/popups/defaultpopupmenumodel.h"
#include "animatedwidgets/popups/popupmenurowtype.h"

#include <QtTest>

using namespace animatedwidgets;

class DefaultPopupMenuModelTest : public QObject {
    Q_OBJECT

private slots:
    void singleSelection();
    void multipleSelection();
    void modeChangesAndReset();
    void invalidAndDisabledItems();
    void structuredItemsAndRoles();
    void logicalSelectionExcludesSeparators();
};

void DefaultPopupMenuModelTest::singleSelection() {
    DefaultPopupMenuModel model;
    model.setStrings({"Open", "Copy", "Pin"});
    QSignalSpy changes(&model, &DefaultPopupMenuModel::currentIndexChanged);
    QSignalSpy multipleChanges(&model, &DefaultPopupMenuModel::selectedIndexesChanged);

    QCOMPARE(model.currentIndex(), -1);
    QCOMPARE(model.rowCount(), 3);
    QCOMPARE(model.data(model.index(1), Qt::DisplayRole).toString(), QStringLiteral("Copy"));
    model.triggerItem(model.index(1));
    QCOMPARE(model.currentIndex(), 1);
    QCOMPARE(changes.count(), 1);
    QCOMPARE(changes.last().at(0).toInt(), 1);
    model.triggerItem(model.index(1));
    QCOMPARE(changes.count(), 1);

    model.triggerItem(model.index(2));
    QCOMPARE(model.data(model.index(1), Qt::CheckStateRole).toInt(), int(Qt::Unchecked));
    QCOMPARE(model.data(model.index(2), Qt::CheckStateRole).toInt(), int(Qt::Checked));
    QCOMPARE(changes.count(), 2);
    QCOMPARE(multipleChanges.count(), 0);

    model.setCurrentIndex(-1);
    QCOMPARE(model.currentIndex(), -1);
    QCOMPARE(changes.last().at(0).toInt(), -1);
}

void DefaultPopupMenuModelTest::multipleSelection() {
    DefaultPopupMenuModel model;
    model.setStrings({"Open", "Copy", "Pin"});
    model.setSelectionMode(DefaultPopupMenuModel::MultiSelection);
    QSignalSpy changes(&model, &DefaultPopupMenuModel::selectedIndexesChanged);
    QSignalSpy singleChanges(&model, &DefaultPopupMenuModel::currentIndexChanged);

    model.triggerItem(model.index(2));
    model.triggerItem(model.index(0));
    QCOMPARE(model.selectedIndexes(), QList<int>({0, 2}));
    QCOMPARE(qvariant_cast<QList<int>>(changes.last().at(0)), QList<int>({0, 2}));
    QCOMPARE(model.data(model.index(0), Qt::CheckStateRole).toInt(), int(Qt::Checked));
    QCOMPARE(model.data(model.index(2), Qt::CheckStateRole).toInt(), int(Qt::Checked));

    model.triggerItem(model.index(0));
    QCOMPARE(model.selectedIndexes(), QList<int>({2}));
    QCOMPARE(changes.count(), 3);
    QCOMPARE(singleChanges.count(), 0);
    model.setSelectedIndexes({2, 2, -1, 30});
    QCOMPARE(changes.count(), 3);
    model.triggerItem(model.index(2));
    QVERIFY(model.selectedIndexes().isEmpty());
    QCOMPARE(qvariant_cast<QList<int>>(changes.last().at(0)), QList<int>());
}

void DefaultPopupMenuModelTest::modeChangesAndReset() {
    DefaultPopupMenuModel model;
    model.setStrings({"Open", "Copy", "Pin"});
    model.setSelectionMode(DefaultPopupMenuModel::MultiSelection);
    model.setSelectedIndexes({2, 0});
    QSignalSpy singleChanges(&model, &DefaultPopupMenuModel::currentIndexChanged);
    QSignalSpy multipleChanges(&model, &DefaultPopupMenuModel::selectedIndexesChanged);

    model.setSelectionMode(DefaultPopupMenuModel::SingleSelection);
    QCOMPARE(model.selectedIndexes(), QList<int>({0}));
    QCOMPARE(model.data(model.index(2), Qt::CheckStateRole).toInt(), int(Qt::Unchecked));
    QCOMPARE(singleChanges.count(), 1);
    model.setSelectionMode(DefaultPopupMenuModel::MultiSelection);
    QCOMPARE(qvariant_cast<QList<int>>(multipleChanges.last().at(0)), QList<int>({0}));

    model.setStrings({"New"});
    QCOMPARE(model.rowCount(), 1);
    QVERIFY(model.selectedIndexes().isEmpty());
    QCOMPARE(qvariant_cast<QList<int>>(multipleChanges.last().at(0)), QList<int>());
    model.setCurrentIndex(0);
    model.setStrings({"New"});
    QCOMPARE(model.selectedIndexes(), QList<int>({0}));
}

void DefaultPopupMenuModelTest::invalidAndDisabledItems() {
    DefaultPopupMenuModel model;
    model.setStrings({"Open", "Copy"});
    DefaultPopupMenuModel other;
    other.setStrings({"Other"});
    QSignalSpy changes(&model, &DefaultPopupMenuModel::currentIndexChanged);

    model.setItemEnabled(1, false);
    QVERIFY(!model.flags(model.index(1)).testFlag(Qt::ItemIsEnabled));
    model.triggerItem(model.index(1));
    model.triggerItem(other.index(0));
    model.triggerItem(QModelIndex());
    model.setCurrentIndex(99);
    QVERIFY(!model.setData(model.index(0), Qt::PartiallyChecked, Qt::CheckStateRole));
    QVERIFY(!model.setData(other.index(0), Qt::Checked, Qt::CheckStateRole));
    QCOMPARE(model.currentIndex(), -1);
    QCOMPARE(changes.count(), 0);

    model.setItemEnabled(1, true);
    model.triggerItem(model.index(1));
    QCOMPARE(model.currentIndex(), 1);
    QCOMPARE(changes.count(), 1);
}

void DefaultPopupMenuModelTest::structuredItemsAndRoles() {
    DefaultPopupMenuModel model;
    PopupMenuItem option;
    option.text = QStringLiteral("Option");
    option.shortcut = QStringLiteral("Ctrl+O");
    option.values.insert(Qt::UserRole + 100, QVariantList{42, QStringLiteral("payload")});
    PopupMenuItem separator;
    separator.separator = true;
    model.setItems({separator, option, separator});

    QCOMPARE(model.flags(model.index(0)), Qt::NoItemFlags);
    QCOMPARE(model.flags(model.index(2)), Qt::NoItemFlags);
    QVERIFY(!model.index(0).data(Qt::CheckStateRole).isValid());
    QVERIFY(model.index(2).data(PopupMenuItem::SeparatorRole).toBool());
    QCOMPARE(model.index(1).data(PopupMenuItem::ShortcutRole).toString(), option.shortcut);
    QCOMPARE(model.index(1).data(Qt::UserRole + 100).toList(), option.values.value(Qt::UserRole + 100).toList());
    model.setSelectedIndexes({1, 2});
    QCOMPARE(model.currentIndex(), -1);
    model.triggerItem(model.index(1));
    QCOMPARE(model.currentIndex(), 0);
    QVERIFY(!model.setData(model.index(0), Qt::Checked, Qt::CheckStateRole));

    QSignalSpy changes(&model, &QAbstractItemModel::dataChanged);
    QVERIFY(model.setData(model.index(1), 99, Qt::UserRole + 100));
    QCOMPARE(model.index(1).data(Qt::UserRole + 100).toInt(), 99);
    QCOMPARE(changes.count(), 1);
    QVERIFY(model.setData(model.index(1), QVariant(), Qt::UserRole + 100));
    QVERIFY(!model.index(1).data(Qt::UserRole + 100).isValid());
    QVERIFY(!model.setData(model.index(1), static_cast<int>(PopupMenuRowType::GroupTitle), PopupMenuItem::TypeRole));

    model.setStrings({"Plain"});
    QCOMPARE(model.rowCount(), 1);
    QCOMPARE(model.index(0).data(PopupMenuItem::TypeRole).toInt(), static_cast<int>(PopupMenuRowType::Option));
    QCOMPARE(model.currentIndex(), -1);
}

void DefaultPopupMenuModelTest::logicalSelectionExcludesSeparators() {
    DefaultPopupMenuModel model;
    PopupMenuItem first;
    first.text = QStringLiteral("First");
    PopupMenuItem second;
    second.text = QStringLiteral("Second");
    PopupMenuItem separator;
    separator.separator = true;
    model.setItems({separator, first, separator, separator, second, separator});
    QSignalSpy singleChanges(&model, &DefaultPopupMenuModel::currentIndexChanged);

    model.triggerItem(model.index(4));
    QCOMPARE(model.currentIndex(), 1);
    QCOMPARE(singleChanges.last().at(0).toInt(), 1);
    QCOMPARE(model.strings().size(), 2);
    QCOMPARE(model.strings().at(0), QStringLiteral("First"));
    QCOMPARE(model.strings().at(1), QStringLiteral("Second"));
    QCOMPARE(model.logicalIndex(model.index(4)), 1);
    QCOMPARE(model.logicalIndex(model.index(2)), -1);
    QCOMPARE(model.indexForItem(1).row(), 4);
    QVERIFY(!model.indexForItem(2).isValid());

    model.setCurrentIndex(0);
    QCOMPARE(model.index(1).data(Qt::CheckStateRole).toInt(), int(Qt::Checked));
    model.setItemEnabled(1, false);
    model.triggerItem(model.index(4));
    QCOMPARE(model.currentIndex(), 0);
    model.setItemEnabled(1, true);
    model.setSelectionMode(DefaultPopupMenuModel::MultiSelection);
    QSignalSpy multipleChanges(&model, &DefaultPopupMenuModel::selectedIndexesChanged);
    model.setSelectedIndexes({1, 0, 1, -1, 2});
    QCOMPARE(model.selectedIndexes(), QList<int>({0, 1}));
    QCOMPARE(qvariant_cast<QList<int>>(multipleChanges.last().at(0)), QList<int>({0, 1}));
    model.triggerItem(model.index(1));
    QCOMPARE(model.selectedIndexes(), QList<int>({1}));
}

QTEST_APPLESS_MAIN(DefaultPopupMenuModelTest)

#include "tstdefaultpopupmenumodel.moc"
