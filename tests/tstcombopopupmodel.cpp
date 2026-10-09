#include "animatedwidgets/comboboxes/combopopupmodel.h"
#include "animatedwidgets/popups/popupmenuitemdelegate.h"
#include "animatedwidgets/popups/popupmenufiltermodel.h"

#include <QStandardItemModel>
#include <QtTest>

using namespace animatedwidgets;

class ComboPopupModelTest : public QObject {
    Q_OBJECT

private slots:
    void selectionDoesNotChangeSource();
    void rootColumnAndPersistentSelection();
    void separatorsAndSourceReplacement();
    void groupTitles();
    void filteredGroupsAndSelectionMapping();
};

void ComboPopupModelTest::selectionDoesNotChangeSource() {
    QStandardItemModel source;
    auto* first = new QStandardItem(QStringLiteral("First"));
    first->setCheckable(true);
    first->setCheckState(Qt::Unchecked);
    first->setData(42, PopupMenuItemDelegate::SeparatorRole);
    first->setData(QStringLiteral("Business data"), PopupMenuItemDelegate::ShortcutRole);
    first->setData(PopupMenuItem::GroupTitle, PopupMenuItemDelegate::TypeRole);
    first->setData(QStringLiteral("Business payload"), PopupMenuItem::HighlightRole);
    source.appendRow(first);
    source.appendRow(new QStandardItem(QStringLiteral("Second")));
    ComboPopupModel model;
    model.setSourceModel(&source);
    QSignalSpy changes(&model, &QAbstractItemModel::dataChanged);
    QSignalSpy sourceChanges(&source, &QAbstractItemModel::dataChanged);

    model.setCurrentIndex(source.index(0, 0));
    QCOMPARE(model.data(model.index(0, 0), Qt::CheckStateRole).toInt(), int(Qt::Checked));
    QCOMPARE(first->checkState(), Qt::Unchecked);
    QVERIFY(!model.flags(model.index(0, 0)).testFlag(Qt::ItemIsUserCheckable));
    QVERIFY(!model.data(model.index(0, 0), PopupMenuItemDelegate::SeparatorRole).toBool());
    QVERIFY(!model.data(model.index(0, 0), PopupMenuItemDelegate::ShortcutRole).isValid());
    QVERIFY(!model.data(model.index(0, 0), PopupMenuItem::HighlightRole).isValid());
    QVERIFY(!model.setData(model.index(0, 0), QStringLiteral("html"), PopupMenuItem::HighlightRole));
    QCOMPARE(model.data(model.index(0, 0), PopupMenuItemDelegate::TypeRole).toInt(), int(PopupMenuItem::Option));
    QVERIFY(!model.setData(model.index(0, 0), PopupMenuItem::Separator, PopupMenuItemDelegate::TypeRole));
    QVERIFY(!model.setData(model.index(0, 0), Qt::Checked, Qt::CheckStateRole));
    QCOMPARE(first->data(PopupMenuItemDelegate::SeparatorRole).toInt(), 42);
    QCOMPARE(sourceChanges.count(), 0);

    model.setCurrentIndex(source.index(1, 0));
    QCOMPARE(model.data(model.index(0, 0), Qt::CheckStateRole).toInt(), int(Qt::Unchecked));
    QCOMPARE(model.data(model.index(1, 0), Qt::CheckStateRole).toInt(), int(Qt::Checked));
    QCOMPARE(changes.count(), 3);
    model.setCurrentIndex(source.index(1, 0));
    QCOMPARE(changes.count(), 3);
    model.setCurrentIndex(QModelIndex());
    QCOMPARE(model.data(model.index(1, 0), Qt::CheckStateRole).toInt(), int(Qt::Unchecked));
}

void ComboPopupModelTest::rootColumnAndPersistentSelection() {
    QStandardItemModel source;
    auto* root = new QStandardItem(QStringLiteral("Group"));
    source.appendRow(root);
    root->appendRow({new QStandardItem(QStringLiteral("Id")), new QStandardItem(QStringLiteral("Label"))});
    ComboPopupModel model;
    model.setSourceModel(&source);
    const QModelIndex selected = source.index(0, 1, root->index());
    model.setCurrentIndex(selected);

    const QModelIndex proxy = model.mapFromSource(selected);
    QCOMPARE(proxy.column(), 1);
    QCOMPARE(model.mapToSource(proxy), selected);
    QCOMPARE(model.mapToSource(proxy.parent()), root->index());
    QCOMPARE(model.data(proxy).toString(), QStringLiteral("Label"));
    QCOMPARE(model.data(proxy, Qt::CheckStateRole).toInt(), int(Qt::Checked));
    QVERIFY(model.setData(proxy, QStringLiteral("Updated"), Qt::EditRole));
    QCOMPARE(source.data(selected).toString(), QStringLiteral("Updated"));

    root->insertRow(0, {new QStandardItem(QStringLiteral("New id")), new QStandardItem(QStringLiteral("New label"))});
    const QModelIndex moved = model.index(1, 1, model.mapFromSource(root->index()));
    QCOMPARE(model.data(moved, Qt::CheckStateRole).toInt(), int(Qt::Checked));
    root->removeRow(1);
    QCOMPARE(model.data(model.index(0, 1, model.mapFromSource(root->index())), Qt::CheckStateRole).toInt(), int(Qt::Unchecked));
}

void ComboPopupModelTest::separatorsAndSourceReplacement() {
    QStandardItemModel source;
    auto* separator = new QStandardItem;
    separator->setData(QStringLiteral("separator"), Qt::AccessibleDescriptionRole);
    source.appendRow(separator);
    auto* disabled = new QStandardItem(QStringLiteral("Disabled"));
    disabled->setEnabled(false);
    source.appendRow(disabled);
    ComboPopupModel model;
    model.setSourceModel(&source);
    QVERIFY(model.data(model.index(0, 0), PopupMenuItemDelegate::SeparatorRole).toBool());
    QCOMPARE(model.flags(model.index(0, 0)), Qt::ItemFlags(Qt::NoItemFlags));
    QVERIFY(!model.flags(model.index(1, 0)).testFlag(Qt::ItemIsEnabled));
    model.setCurrentIndex(source.index(1, 0));

    QStandardItemModel replacement;
    replacement.appendRow(new QStandardItem(QStringLiteral("Replacement")));
    model.setSourceModel(&replacement);
    QCOMPARE(model.data(model.index(0, 0), Qt::CheckStateRole).toInt(), int(Qt::Unchecked));
    model.setCurrentIndex(source.index(1, 0));
    QCOMPARE(model.data(model.index(0, 0), Qt::CheckStateRole).toInt(), int(Qt::Unchecked));
    model.setCurrentIndex(replacement.index(0, 0));
    QCOMPARE(model.data(model.index(0, 0), Qt::CheckStateRole).toInt(), int(Qt::Checked));
    replacement.clear();
    QCOMPARE(model.rowCount(), 0);
}

void ComboPopupModelTest::groupTitles() {
    QStandardItemModel source;
    auto* heading = new QStandardItem(QStringLiteral("Planning"));
    heading->setData(QStringLiteral("groupTitle"), Qt::AccessibleDescriptionRole);
    source.appendRow(heading);
    source.appendRow(new QStandardItem(QStringLiteral("Design")));
    auto* separator = new QStandardItem;
    separator->setData(QStringLiteral("separator"), Qt::AccessibleDescriptionRole);
    source.appendRow(separator);

    ComboPopupModel model;
    model.setSourceModel(&source);
    const QModelIndex title = model.index(0, 0);
    const QModelIndex option = model.index(1, 0);
    QCOMPARE(title.data(PopupMenuItem::TypeRole).toInt(), int(PopupMenuItem::GroupTitle));
    QCOMPARE(model.flags(title), Qt::ItemFlags(Qt::NoItemFlags));
    QVERIFY(!title.data(Qt::CheckStateRole).isValid());
    QVERIFY(!title.data(PopupMenuItem::SeparatorRole).toBool());
    QCOMPARE(option.data(PopupMenuItem::TypeRole).toInt(), int(PopupMenuItem::Option));
    QVERIFY(model.flags(option).testFlag(Qt::ItemIsSelectable));
    model.setCurrentIndex(source.index(1, 0));
    QCOMPARE(option.data(Qt::CheckStateRole).toInt(), int(Qt::Checked));
    QCOMPARE(model.index(2, 0).data(PopupMenuItem::TypeRole).toInt(), int(PopupMenuItem::Separator));
    QVERIFY(!model.index(2, 0).data(Qt::CheckStateRole).isValid());

    heading->setData(QVariant(), Qt::AccessibleDescriptionRole);
    QCOMPARE(title.data(PopupMenuItem::TypeRole).toInt(), int(PopupMenuItem::Option));
    QVERIFY(model.flags(title).testFlag(Qt::ItemIsSelectable));
}

void ComboPopupModelTest::filteredGroupsAndSelectionMapping() {
    QStandardItemModel source;
    auto* root = new QStandardItem(QStringLiteral("Assets"));
    source.appendRow(root);
    auto* heading = new QStandardItem(QStringLiteral("Local"));
    heading->setData(QStringLiteral("groupTitle"), Qt::AccessibleDescriptionRole);
    root->appendRow({new QStandardItem(QStringLiteral("Group")), heading});
    auto* orchard = new QStandardItem(QStringLiteral("Orchard"));
    orchard->setData(123, Qt::UserRole);
    root->appendRow({new QStandardItem(QStringLiteral("First")), orchard});
    auto* harbor = new QStandardItem(QStringLiteral("Harbor"));
    harbor->setData(456, Qt::UserRole);
    root->appendRow({new QStandardItem(QStringLiteral("Second")), harbor});

    ComboPopupModel selection;
    selection.setSourceModel(&source);
    selection.setCurrentIndex(harbor->index());
    PopupMenuFilterModel filter;
    filter.setRecursiveFilteringEnabled(true);
    filter.setFilterKeyColumn(1);
    filter.setSourceModel(&selection);
    filter.setSearchText(QStringLiteral("hb"));
    const QModelIndex filteredRoot = filter.mapFromSource(selection.mapFromSource(root->index()));
    QVERIFY(filteredRoot.isValid());
    QCOMPARE(filter.rowCount(filteredRoot), 2);
    const QModelIndex title = filter.index(0, 1, filteredRoot);
    QCOMPARE(title.data(PopupMenuItem::TypeRole).toInt(), int(PopupMenuItem::GroupTitle));
    QVERIFY(!title.flags().testFlag(Qt::ItemIsSelectable));
    const QModelIndex result = filter.index(1, 1, filteredRoot);
    QCOMPARE(selection.mapToSource(filter.mapToSource(result)), harbor->index());
    QCOMPARE(result.data(Qt::UserRole).toInt(), 456);
    QCOMPARE(result.data(Qt::CheckStateRole).toInt(), int(Qt::Checked));
    QVERIFY(result.data(PopupMenuItem::HighlightRole).toString().contains(QStringLiteral("<span")));
    QCOMPARE(source.rowCount(root->index()), 3);

    filter.setSearchText(QStringLiteral("missing"));
    QCOMPARE(filter.rowCount(), 0);
    filter.setSearchText(QString());
    const QModelIndex restoredRoot = filter.mapFromSource(selection.mapFromSource(root->index()));
    QCOMPARE(filter.rowCount(restoredRoot), 3);
    QCOMPARE(filter.index(2, 1, restoredRoot).data(Qt::CheckStateRole).toInt(), int(Qt::Checked));
}

QTEST_APPLESS_MAIN(ComboPopupModelTest)

#include "tstcombopopupmodel.moc"
