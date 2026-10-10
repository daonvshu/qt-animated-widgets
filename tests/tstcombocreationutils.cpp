#include "animatedwidgets/utils/combocreationutils.h"

#include <QStandardItemModel>
#include <QtTest>

using namespace animatedwidgets;

class ComboCreationUtilsTest : public QObject {
    Q_OBJECT

private slots:
    void emptyModelAndWhitespace();
    void exactMatchesAndCaseSensitivity();
    void headingsAndDisabledOptions();
    void rootAndColumn();
};

void ComboCreationUtilsTest::emptyModelAndWhitespace() {
    QStandardItemModel model;
    QVERIFY(ComboCreationUtils::candidate(&model, QModelIndex(), 0, QStringLiteral(" \t\n ")).isEmpty());
    QCOMPARE(ComboCreationUtils::candidate(&model, QModelIndex(), 0, QStringLiteral("  New option  ")), QStringLiteral("New option"));
    QVERIFY(ComboCreationUtils::candidate(&model, QModelIndex(), 0, QStringLiteral("a"), Qt::CaseInsensitive, 2).isEmpty());
    QCOMPARE(ComboCreationUtils::candidate(&model, QModelIndex(), 0, QStringLiteral("ab"), Qt::CaseInsensitive, 2), QStringLiteral("ab"));
    QVERIFY(ComboCreationUtils::candidate(nullptr, QModelIndex(), 0, QStringLiteral("New")).isEmpty());
    QVERIFY(ComboCreationUtils::candidate(&model, QModelIndex(), -1, QStringLiteral("New")).isEmpty());
}

void ComboCreationUtilsTest::exactMatchesAndCaseSensitivity() {
    QStandardItemModel model;
    model.appendRow(new QStandardItem(QStringLiteral("Motion")));
    QVERIFY(ComboCreationUtils::candidate(&model, QModelIndex(), 0, QStringLiteral(" motion ")).isEmpty());
    QVERIFY(ComboCreationUtils::candidate(&model, QModelIndex(), 0, QStringLiteral("Motion"), Qt::CaseSensitive).isEmpty());
    QCOMPARE(ComboCreationUtils::candidate(&model, QModelIndex(), 0, QStringLiteral("motion"), Qt::CaseSensitive), QStringLiteral("motion"));
    QCOMPARE(ComboCreationUtils::candidate(&model, QModelIndex(), 0, QStringLiteral("Mot")), QStringLiteral("Mot"));

    QSignalSpy changes(&model, &QAbstractItemModel::dataChanged);
    ComboCreationUtils::candidate(&model, QModelIndex(), 0, QStringLiteral("New"));
    QCOMPARE(model.rowCount(), 1);
    QCOMPARE(changes.count(), 0);
}

void ComboCreationUtilsTest::headingsAndDisabledOptions() {
    QStandardItemModel model;
    auto* heading = new QStandardItem(QStringLiteral("Assets"));
    heading->setData(QStringLiteral("groupTitle"), Qt::AccessibleDescriptionRole);
    model.appendRow(heading);
    auto* separator = new QStandardItem(QStringLiteral("Divider"));
    separator->setData(QStringLiteral("separator"), Qt::AccessibleDescriptionRole);
    model.appendRow(separator);
    auto* option = new QStandardItem(QStringLiteral("Reserved"));
    option->setEnabled(false);
    model.appendRow(option);

    QCOMPARE(ComboCreationUtils::candidate(&model, QModelIndex(), 0, QStringLiteral("Assets")), QStringLiteral("Assets"));
    QCOMPARE(ComboCreationUtils::candidate(&model, QModelIndex(), 0, QStringLiteral("Divider")), QStringLiteral("Divider"));
    QVERIFY(ComboCreationUtils::candidate(&model, QModelIndex(), 0, QStringLiteral("reserved")).isEmpty());
}

void ComboCreationUtilsTest::rootAndColumn() {
    QStandardItemModel model;
    auto* root = new QStandardItem(QStringLiteral("Category"));
    root->appendRow({new QStandardItem(QStringLiteral("id")), new QStandardItem(QStringLiteral("Label"))});
    model.appendRow(root);

    QVERIFY(ComboCreationUtils::candidate(&model, root->index(), 1, QStringLiteral("Label")).isEmpty());
    QCOMPARE(ComboCreationUtils::candidate(&model, root->index(), 0, QStringLiteral("Label")), QStringLiteral("Label"));
    QVERIFY(ComboCreationUtils::candidate(&model, root->index(), 2, QStringLiteral("New")).isEmpty());

    QStandardItemModel foreign;
    foreign.appendRow(new QStandardItem(QStringLiteral("Foreign")));
    QVERIFY(ComboCreationUtils::candidate(&model, foreign.index(0, 0), 0, QStringLiteral("New")).isEmpty());
    root->removeRow(0);
    QCOMPARE(ComboCreationUtils::candidate(&model, root->index(), 1, QStringLiteral("Label")), QStringLiteral("Label"));
}

QTEST_APPLESS_MAIN(ComboCreationUtilsTest)

#include "tstcombocreationutils.moc"
