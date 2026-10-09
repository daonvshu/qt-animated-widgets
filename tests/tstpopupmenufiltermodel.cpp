#include "animatedwidgets/popups/defaultpopupmenumodel.h"
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
};

void PopupMenuFilterModelTest::groupsAndSourceMapping() {
    DefaultPopupMenuModel source;
    PopupMenuItem heading;
    heading.text = QStringLiteral("First");
    heading.type = PopupMenuItem::GroupTitle;
    PopupMenuItem apple;
    apple.text = QStringLiteral("Apple");
    apple.values.insert(Qt::UserRole + 100, 123);
    PopupMenuItem separator;
    separator.type = PopupMenuItem::Separator;
    PopupMenuItem second = heading;
    second.text = QStringLiteral("Second");
    PopupMenuItem pear;
    pear.text = QStringLiteral("Pear");
    source.setItems({heading, apple, separator, second, pear});

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
    QCOMPARE(source.currentIndex(), 1);
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
    source.setItems({});
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
    DefaultPopupMenuModel source;
    PopupMenuItem title;
    title.text = QStringLiteral("Assets");
    title.type = PopupMenuItem::GroupTitle;
    PopupMenuItem item;
    item.text = QStringLiteral("Harbor");
    item.values.insert(Qt::UserRole + 100, 42);
    source.setItems({title, item});
    source.setCurrentIndex(1);

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

QTEST_APPLESS_MAIN(PopupMenuFilterModelTest)

#include "tstpopupmenufiltermodel.moc"
