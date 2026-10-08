#include "animatedwidgets/comboboxes/combopopupmodel.h"
#include "animatedwidgets/popups/popupmenuitemdelegate.h"

#include <QStandardItemModel>
#include <QtTest>

using namespace animatedwidgets;

class ComboPopupModelTest : public QObject {
    Q_OBJECT

private slots:
    void selectionDoesNotChangeSource();
    void rootColumnAndPersistentSelection();
    void separatorsAndSourceReplacement();
};

void ComboPopupModelTest::selectionDoesNotChangeSource() {
    QStandardItemModel source;
    auto* first = new QStandardItem(QStringLiteral("First"));
    first->setCheckable(true);
    first->setCheckState(Qt::Unchecked);
    first->setData(42, PopupMenuItemDelegate::SeparatorRole);
    first->setData(QStringLiteral("Business data"), PopupMenuItemDelegate::ShortcutRole);
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

QTEST_APPLESS_MAIN(ComboPopupModelTest)

#include "tstcombopopupmodel.moc"
