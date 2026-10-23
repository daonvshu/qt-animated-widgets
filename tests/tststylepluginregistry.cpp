#include "animatedwidgets/styleeditor/api/styleeditorplugin.h"
#include "../styleeditor/plugins/stylepluginregistry.h"

#include <QDir>
#include <QLibrary>
#include <QPluginLoader>
#include <QSet>
#include <QtTest>

class StylePluginRegistryTest : public QObject {
    Q_OBJECT

private slots:
    void independentPluginCatalogs();
    void displayOrder();
};

void StylePluginRegistryTest::independentPluginCatalogs() {
    const QMap<QString, int> expected = {{QStringLiteral("animatedwidgets.buttons"), 8},    {QStringLiteral("animatedwidgets.selections"), 2},
                                         {QStringLiteral("animatedwidgets.switches"), 1},   {QStringLiteral("animatedwidgets.progressbars"), 1},
                                         {QStringLiteral("animatedwidgets.sliders"), 1},    {QStringLiteral("animatedwidgets.inputs"), 10},
                                         {QStringLiteral("animatedwidgets.comboboxes"), 7}, {QStringLiteral("animatedwidgets.tags"), 1},
                                         {QStringLiteral("animatedwidgets.calendars"), 9},  {QStringLiteral("animatedwidgets.popups"), 4}};
    QSet<QString> loaded;
    QSet<QString> typeIds;
    const QDir directory(QString::fromUtf8(STYLE_PLUGIN_DIRECTORY));
    for (const QString& file : directory.entryList(QDir::Files)) {
        if (!QLibrary::isLibrary(file))
            continue;
        QPluginLoader loader(directory.absoluteFilePath(file));
        if (loader.metaData().value(QStringLiteral("IID")).toString() != QString::fromLatin1(ANIMATEDWIDGETS_STYLE_EDITOR_PLUGIN_IID))
            continue;
        auto* plugin = qobject_cast<animatedwidgets::styleeditor::StyleEditorPlugin*>(loader.instance());
        QVERIFY2(plugin, qPrintable(loader.errorString()));
        QVERIFY(expected.contains(plugin->pluginId()));
        QVERIFY(!loaded.contains(plugin->pluginId()));
        loaded.insert(plugin->pluginId());
        const auto types = plugin->widgetTypes();
        QCOMPARE(int(types.size()), expected.value(plugin->pluginId()));
        for (const auto& type : types) {
            QVERIFY(!typeIds.contains(type.id));
            QVERIFY(!type.id.isEmpty());
            QVERIFY(!type.name.isEmpty());
            QVERIFY(!type.category.isEmpty());
            QVERIFY(type.typeName.startsWith(QStringLiteral("animatedwidgets::")));
            QVERIFY(type.suggestedSize.isValid());
            QVERIFY(type.create);
            QVERIFY(type.properties);
            typeIds.insert(type.id);
        }
    }
    QCOMPARE(int(loaded.size()), int(expected.size()));
    QCOMPARE(int(typeIds.size()), 44);
}

void StylePluginRegistryTest::displayOrder() {
    StylePluginRegistry registry;
    registry.load(QString::fromUtf8(STYLE_PLUGIN_DIRECTORY));
    const auto& types = registry.widgetTypes();
    QCOMPARE(int(types.size()), 44);
    const QStringList expected = {QStringLiteral("Buttons"), QStringLiteral("Selections"), QStringLiteral("Switches"),
                                  QStringLiteral("Inputs"), QStringLiteral("Combo boxes"), QStringLiteral("Sliders"),
                                  QStringLiteral("Progress bars"), QStringLiteral("Tags"), QStringLiteral("Calendars"), QStringLiteral("Popups")};
    int categoryIndex = -1;
    QString category;
    double order = 0.0;
    for (const auto& type : types) {
        if (type.category != category) {
            ++categoryIndex;
            QVERIFY(categoryIndex < expected.size());
            QCOMPARE(type.category, expected.at(categoryIndex));
            QCOMPARE(type.categoryOrder, (categoryIndex + 1) * 100.0);
            category = type.category;
            order = 0.0;
        }
        QVERIFY(type.order > order);
        order = type.order;
        QCOMPARE(registry.findType(type.id), &type);
    }
    QCOMPARE(categoryIndex + 1, int(expected.size()));
}

QTEST_GUILESS_MAIN(StylePluginRegistryTest)

#include "tststylepluginregistry.moc"
