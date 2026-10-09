#include "animatedwidgets/popups/defaultpopupmenumodel.h"
#include "animatedwidgets/popups/groupedpopupmenumodel.h"
#include "animatedwidgets/popups/popupmenufiltermodel.h"
#include "animatedwidgets/utils/textsearchutils.h"

#include <QtTest>

using namespace animatedwidgets;

class PopupMenuFilterModelTest : public QObject {
    Q_OBJECT

private slots:
    void groupsAndSourceMapping();
    void fuzzyHighlightAndEscaping();
    void highlightConfiguration();
    void untitledGroupBoundary();
    void ordinaryLogicalIndexAfterFiltering();
};

void PopupMenuFilterModelTest::groupsAndSourceMapping() {
    GroupedPopupMenuModel source;
    PopupMenuItem apple;
    apple.text = QStringLiteral("Apple");
    apple.values.insert(Qt::UserRole + 100, 123);
    PopupMenuItem separator;
    separator.separator = true;
    PopupMenuItem pear;
    pear.text = QStringLiteral("Pear");
    PopupMenuGroup first;
    first.title = QStringLiteral("First");
    first.items = {apple, separator};
    PopupMenuGroup second;
    second.title = QStringLiteral("Second");
    second.items = {pear};
    source.setGroups({first, second});

    PopupMenuFilterModel proxy;
    proxy.setFilterKeyColumn(0);
    proxy.setSourceModel(&source);
    QCOMPARE(proxy.rowCount(), 5);
    proxy.setSearchText(QStringLiteral("  APP  "));
    QCOMPARE(proxy.rowCount(), 2);
    QCOMPARE(proxy.index(0, 0).data().toString(), QStringLiteral("First"));
    QCOMPARE(proxy.mapToSource(proxy.index(1, 0)).row(), 1);
    QCOMPARE(proxy.index(1, 0).data(Qt::UserRole + 100).toInt(), 123);
    source.triggerItem(proxy.mapToSource(proxy.index(1, 0)));
    QCOMPARE(source.currentIndex(0), 0);
    QCOMPARE(proxy.index(1, 0).data(Qt::CheckStateRole).toInt(), int(Qt::Checked));

    proxy.setSearchText(QStringLiteral("pear"));
    QCOMPARE(proxy.rowCount(), 2);
    QCOMPARE(proxy.mapToSource(proxy.index(1, 0)).row(), 4);
    proxy.setSearchText(QStringLiteral("unmatched"));
    QCOMPARE(proxy.rowCount(), 0);
    source.setData(source.index(4), QStringLiteral("Unmatched project"), Qt::DisplayRole);
    QCOMPARE(proxy.rowCount(), 2);
    source.setData(source.index(4), QStringLiteral("Pear"), Qt::DisplayRole);
    QCOMPARE(proxy.rowCount(), 0);
    proxy.setSearchText(QString());
    QCOMPARE(proxy.rowCount(), 5);

    source.setData(source.index(1), QStringLiteral("Pear apple"), Qt::DisplayRole);
    proxy.setSearchText(QStringLiteral("pear"));
    QCOMPARE(proxy.rowCount(), 4);
    source.setGroups({});
    QCOMPARE(proxy.rowCount(), 0);
}

void PopupMenuFilterModelTest::fuzzyHighlightAndEscaping() {
    bool matched = false;
    const QString html = TextSearchUtils::searchAndHighlight(QStringLiteral("Harbor <&>"), QStringLiteral("HR"),
                                                           QColor(QStringLiteral("#123456")), true, &matched);
    QVERIFY(matched);
    QVERIFY(html.contains(QStringLiteral("font-weight:bold")));
    QVERIFY(html.contains(QStringLiteral("color:#123456")));
    QVERIFY(html.contains(QStringLiteral("&lt;&amp;&gt;")));
    QVERIFY(!html.contains(QStringLiteral("<Harbor")));
    QVERIFY(TextSearchUtils::matches(QStringLiteral("Harbor"), QStringLiteral("hr")));
    QVERIFY(!TextSearchUtils::matches(QStringLiteral("Harbor"), QStringLiteral("rh")));

    QCOMPARE(TextSearchUtils::searchAndHighlight(QStringLiteral("<&>"), QStringLiteral("missing"), Qt::blue, false, &matched),
             QStringLiteral("&lt;&amp;&gt;"));
    QVERIFY(!matched);
    QCOMPARE(TextSearchUtils::searchAndHighlight(QStringLiteral("<&>"), QString(), Qt::blue, false, &matched),
             QStringLiteral("&lt;&amp;&gt;"));
    QVERIFY(matched);
    QVERIFY(!TextSearchUtils::matches(QString(), QStringLiteral("x")));
}

void PopupMenuFilterModelTest::highlightConfiguration() {
    GroupedPopupMenuModel source;
    PopupMenuItem item;
    item.text = QStringLiteral("Harbor");
    item.values.insert(Qt::UserRole + 100, 42);
    PopupMenuGroup group;
    group.title = QStringLiteral("Assets");
    group.items = {item};
    source.setGroups({group});
    source.setCurrentIndex(0, 0);

    PopupMenuFilterModel proxy;
    QVERIFY(proxy.highlightEnabled());
    proxy.setHighlightEnabled(false);
    proxy.setSourceModel(&source);
    proxy.setSearchText(QStringLiteral("hr"));
    QCOMPARE(proxy.rowCount(), 2);
    QVERIFY(!proxy.index(1, 0).data(PopupMenuItem::HighlightRole).isValid());
    QSignalSpy changes(&proxy, &QAbstractItemModel::dataChanged);
    proxy.setHighlightEnabled(true);
    proxy.setHighlightColor(QColor(QStringLiteral("#123456")));
    proxy.setHighlightBold(false);
    QVERIFY(changes.count() > 0);
    const QString html = proxy.index(1, 0).data(PopupMenuItem::HighlightRole).toString();
    QVERIFY(html.contains(QStringLiteral("color:#123456")));
    QVERIFY(html.contains(QStringLiteral("font-weight:normal")));
    QVERIFY(!proxy.index(0, 0).data(PopupMenuItem::HighlightRole).isValid());
    QCOMPARE(proxy.index(1, 0).data().toString(), QStringLiteral("Harbor"));
    QCOMPARE(source.index(1).data().toString(), QStringLiteral("Harbor"));
    QCOMPARE(proxy.index(1, 0).data(Qt::UserRole + 100).toInt(), 42);
    QCOMPARE(proxy.index(1, 0).data(Qt::CheckStateRole).toInt(), int(Qt::Checked));
    proxy.setHighlightEnabled(false);
    QVERIFY(!proxy.index(1, 0).data(PopupMenuItem::HighlightRole).isValid());
    QCOMPARE(proxy.rowCount(), 2);
    proxy.setHighlightEnabled(true);
    proxy.setSearchText(QString());
    QVERIFY(!proxy.index(1, 0).data(PopupMenuItem::HighlightRole).isValid());
}

void PopupMenuFilterModelTest::untitledGroupBoundary() {
    GroupedPopupMenuModel source;
    PopupMenuItem separator;
    separator.separator = true;
    PopupMenuGroup first;
    first.title = QStringLiteral("First");
    first.items = {PopupMenuItem{QStringLiteral("Apple")}, separator, PopupMenuItem{QStringLiteral("Pear")}};
    PopupMenuGroup second;
    second.items = {PopupMenuItem{QStringLiteral("Harbor")}};
    source.setGroups({first, second});

    PopupMenuFilterModel proxy;
    proxy.setSourceModel(&source);
    proxy.setSearchText(QStringLiteral("pear"));
    QCOMPARE(proxy.rowCount(), 2);
    QCOMPARE(proxy.index(0, 0).data().toString(), first.title);
    QCOMPARE(source.logicalIndex(proxy.mapToSource(proxy.index(1, 0))), qMakePair(0, 1));

    proxy.setSearchText(QStringLiteral("harbor"));
    QCOMPARE(proxy.rowCount(), 1);
    QCOMPARE(proxy.index(0, 0).data().toString(), QStringLiteral("Harbor"));
    source.triggerItem(proxy.mapToSource(proxy.index(0, 0)));
    QCOMPARE(source.currentIndex(1), 0);
}

void PopupMenuFilterModelTest::ordinaryLogicalIndexAfterFiltering() {
    DefaultPopupMenuModel source;
    PopupMenuItem separator;
    separator.separator = true;
    source.setItems({separator, PopupMenuItem{QStringLiteral("Apple")}, separator,
                     PopupMenuItem{QStringLiteral("Pear")}});
    PopupMenuFilterModel proxy;
    proxy.setSourceModel(&source);
    proxy.setSearchText(QStringLiteral("pear"));
    QCOMPARE(proxy.rowCount(), 1);
    const QModelIndex item = proxy.mapToSource(proxy.index(0, 0));
    QCOMPARE(item.row(), 3);
    QCOMPARE(source.logicalIndex(item), 1);
    source.triggerItem(item);
    QCOMPARE(source.currentIndex(), 1);
}

QTEST_APPLESS_MAIN(PopupMenuFilterModelTest)

#include "tstpopupmenufiltermodel.moc"
