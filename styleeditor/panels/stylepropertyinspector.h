#pragma once

#include "animatedwidgets/styleeditor/api/styleproperty.h"
#include "../document/styleresourceentry.h"
#include "../document/styleresourcesettings.h"

#include <QHash>
#include <QObject>
#include <QPointer>
#include <QSet>

class ColorPickerDialog;
class StyleIconResourcePopup;
class StylePopupDialog;
class StylePropertyRow;
class QTreeWidget;
class QTreeWidgetItem;
class QWidget;

class StylePropertyInspector : public QObject {
    Q_OBJECT

public:
    explicit StylePropertyInspector(QTreeWidget* tree, QObject* parent = nullptr);

    void setProperties(const QList<animatedwidgets::styleeditor::StyleProperty>& properties);
    void clear();
    void setResourceSettings(const StyleResourceSettings& settings);

signals:
    void valueEdited(const QString& name, const QVariant& value);
    void dialogRequested(StylePopupDialog* dialog);
    void resourceSettingsEdited(const StyleResourceSettings& settings);

private:
    QWidget* createRow(const animatedwidgets::styleeditor::StyleProperty& property);
    void changeValue(const QString& name, const QVariant& value);
    void refreshEnabledStates();
    void showColorPopup(StylePropertyRow* row, const QColor& color, quint64 generation);
    void showIconPopup(StylePropertyRow* row, const QString& reference, quint64 generation);

    QTreeWidget* m_tree;
    QList<animatedwidgets::styleeditor::StyleProperty> m_properties;
    QHash<QString, QTreeWidgetItem*> m_propertyItems;

    QPointer<ColorPickerDialog> m_colorPopup;
    QPointer<StyleIconResourcePopup> m_iconPopup;
    StyleResourceSettings m_resources;
    QList<StyleResourceEntry> m_resourceEntries;
    QSet<QString> m_collapsedGroups;
    quint64 m_generation = 0;
};
