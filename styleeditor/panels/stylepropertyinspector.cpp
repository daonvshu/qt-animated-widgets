#include "stylepropertyinspector.h"

#include "stylepropertyrow.h"
#include "../dialogs/colorpickerdialog.h"

#include "animatedwidgets/styleeditor/utils/stylepropertystateutils.h"

#include <QHash>
#include <QScrollBar>
#include <QTreeWidget>

using animatedwidgets::styleeditor::StyleProperty;
using animatedwidgets::styleeditor::StylePropertyStateUtils;

StylePropertyInspector::StylePropertyInspector(QTreeWidget* tree, QObject* parent)
    : QObject(parent)
    , m_tree(tree)
{
    connect(tree, &QTreeWidget::itemCollapsed, this, [this](QTreeWidgetItem* item) {
        if (!item->parent())
            m_collapsedGroups.insert(item->data(0, Qt::UserRole).toString());
    });
    connect(tree, &QTreeWidget::itemExpanded, this, [this](QTreeWidgetItem* item) {
        if (!item->parent())
            m_collapsedGroups.remove(item->data(0, Qt::UserRole).toString());
    });
}

void StylePropertyInspector::setProperties(const QList<StyleProperty>& properties) {
    const int scrollPosition = m_tree->verticalScrollBar()->value();
    m_properties = properties;
    ++m_generation;
    m_propertyItems.clear();
    m_tree->clear();

    QHash<QString, QTreeWidgetItem*> groups;
    for (const StyleProperty& property : m_properties) {
        const QString groupName = property.group.trimmed().isEmpty() ? QStringLiteral("General") : property.group;
        QTreeWidgetItem* group = groups.value(groupName, nullptr);
        if (!group) {
            group = new QTreeWidgetItem(m_tree);
            group->setText(0, groupName);
            group->setData(0, Qt::UserRole, groupName);
            group->setFlags(Qt::ItemIsEnabled);
            QFont font = m_tree->font();
            font.setBold(true);
            group->setFont(0, font);
            group->setSizeHint(0, QSize(0, 32));
            groups.insert(groupName, group);
        }

        auto* item = new QTreeWidgetItem(group);
        item->setFlags(Qt::ItemIsEnabled);
        item->setToolTip(0, property.description);
        QWidget* row = createRow(property);
        item->setSizeHint(0, QSize(0, row->sizeHint().height()));
        m_propertyItems.insert(property.name, item);
        m_tree->setItemWidget(item, 0, row);
    }

    for (auto it = groups.constBegin(); it != groups.constEnd(); ++it)
        it.value()->setExpanded(!m_collapsedGroups.contains(it.key()));
    refreshEnabledStates();
    m_tree->verticalScrollBar()->setValue(scrollPosition);
}

void StylePropertyInspector::clear() {
    setProperties({});
}

QWidget* StylePropertyInspector::createRow(const StyleProperty& property) {
    auto* row = new StylePropertyRow(property, m_tree);
    const QString name = property.name;
    const quint64 generation = m_generation;

    connect(row, &StylePropertyRow::valueEdited, this, [this, name, generation](const QVariant& value) {
        if (generation == m_generation)
            changeValue(name, value);
    });
    connect(row, &StylePropertyRow::colorRequested, this, [this, row, generation](const QColor& color) {
        if (generation == m_generation)
            showColorPopup(row, color, generation);
    });
    connect(row, &StylePropertyRow::resetRequested, this, [this, name, generation, defaultValue = property.defaultValue] {
        if (generation != m_generation)
            return;
        const QPointer<StylePropertyInspector> inspector(this);
        changeValue(name, defaultValue);
        if (inspector && generation == m_generation)
            setProperties(m_properties);
    });
    return row;
}

void StylePropertyInspector::showColorPopup(StylePropertyRow* row, const QColor& color, quint64 generation) {
    if (m_colorPopup || !row->isEnabled())
        return;
    auto* popup = new ColorPickerDialog(color, m_tree->window());
    m_colorPopup = popup;
    const QPointer<StylePropertyRow> target(row);
    connect(popup, &ColorPickerDialog::colorAccepted, this, [this, target, generation](const QColor& selected) {
        if (target && target->isEnabled() && generation == m_generation)
            target->setColor(selected);
    });
    emit dialogRequested(popup);
}

void StylePropertyInspector::changeValue(const QString& name, const QVariant& value) {
    for (StyleProperty& property : m_properties) {
        if (property.name != name)
            continue;
        if (property.value == value || !StylePropertyStateUtils::isEnabled(property, m_properties))
            return;
        property.value = value;
        refreshEnabledStates();
        emit valueEdited(name, value);
        return;
    }
}

void StylePropertyInspector::refreshEnabledStates() {
    for (const StyleProperty& property : m_properties) {
        QTreeWidgetItem* item = m_propertyItems.value(property.name, nullptr);
        if (!item)
            continue;

        const bool enabled = StylePropertyStateUtils::isEnabled(property, m_properties);
        item->setFlags(enabled ? Qt::ItemIsEnabled : Qt::NoItemFlags);
        if (QWidget* row = m_tree->itemWidget(item, 0))
            row->setEnabled(enabled);
    }
}
