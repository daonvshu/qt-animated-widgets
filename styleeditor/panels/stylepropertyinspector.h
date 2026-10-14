#pragma once

#include "animatedwidgets/styleeditor/api/styleproperty.h"

#include <QHash>
#include <QObject>
#include <QPointer>
#include <QSet>

class ColorPickerDialog;
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

signals:
    void valueEdited(const QString& name, const QVariant& value);
    void dialogRequested(StylePopupDialog* dialog);

private:
    QWidget* createRow(const animatedwidgets::styleeditor::StyleProperty& property);
    void changeValue(const QString& name, const QVariant& value);
    void refreshEnabledStates();
    void showColorPopup(StylePropertyRow* row, const QColor& color, quint64 generation);

    QTreeWidget* m_tree;
    QList<animatedwidgets::styleeditor::StyleProperty> m_properties;
    QHash<QString, QTreeWidgetItem*> m_propertyItems;

    QPointer<ColorPickerDialog> m_colorPopup;
    QSet<QString> m_collapsedGroups;
    quint64 m_generation = 0;
};
