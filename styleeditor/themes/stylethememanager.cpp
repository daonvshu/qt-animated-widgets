#include "stylethememanager.h"

#include "stylethemescope.h"
#include "../panels/stylepreviewpanel.h"
#include "../panels/stylepropertyrow.h"
#include "../utils/stylethemeutils.h"

#include "animatedwidgets/comboboxes/combobox.h"
#include "animatedwidgets/popups/defaultpopupmenuliststyle.h"
#include "animatedwidgets/popups/defaultpopupmenustyle.h"
#include "animatedwidgets/popups/popupmenu.h"

#include <virtualitemviews/virtualtreeview.h>

#include <QDebug>
#include <QApplication>
#include <QEvent>
#include <QMainWindow>
#include <QSettings>
#include <QStyle>

StyleThemeManager::StyleThemeManager(QMainWindow* window, StyleThemeScope* scope)
    : QObject(window)
    , m_window(window)
    , m_scope(scope)
{
    const QSettings settings(QStringLiteral("AnimatedWidgets"), QStringLiteral("StyleEditor"));
    const QString saved = settings.value(QStringLiteral("appearance/theme"), QStringLiteral("light")).toString();
    if (!setTheme(saved))
        setTheme(QStringLiteral("light"));
    qApp->installEventFilter(this);
}

QString StyleThemeManager::theme() const {
    return m_theme;
}

bool StyleThemeManager::eventFilter(QObject* watched, QEvent* event) {
    if (event->type() == QEvent::Show) {
        if (auto* popup = qobject_cast<animatedwidgets::PopupMenu*>(watched)) {
            if (forWidget(popup) == this) {
                for (QWidget* owner = popup; owner; owner = owner->parentWidget()) {
                    if (owner->property("editorPreview").toBool())
                        return QObject::eventFilter(watched, event);
                }
                // Input controls create context menus internally, outside the dialog callbacks.
                preparePopup(popup);
            }
        }
    }
    return QObject::eventFilter(watched, event);
}

bool StyleThemeManager::setTheme(const QString& theme) {
    if (!m_window || !m_scope)
        return false;
    if (m_theme == theme)
        return true;

    QString error;
    const QString styleSheet = StyleThemeUtils::loadStyleSheet(theme, error);
    if (!error.isEmpty()) {
        qWarning().noquote() << error;
        return false;
    }
    const QString scrollBarStyleSheet = StyleThemeUtils::loadScrollBarStyleSheet(theme, error);
    if (!error.isEmpty()) {
        qWarning().noquote() << error;
        return false;
    }

    const bool updatesEnabled = m_window->updatesEnabled();
    m_window->setUpdatesEnabled(false);
    m_theme = theme;
    m_scrollBarStyleSheet = scrollBarStyleSheet;
    prepareSubtree(m_window);
    m_window->setStyleSheet(styleSheet);
    m_scope->ensurePolished();
    m_scope->style()->unpolish(m_scope);
    m_scope->style()->polish(m_scope);
    applySubtreeColors(m_window);
    m_window->setUpdatesEnabled(updatesEnabled);

    QSettings settings(QStringLiteral("AnimatedWidgets"), QStringLiteral("StyleEditor"));
    settings.setValue(QStringLiteral("appearance/theme"), theme);
    emit themeChanged(theme);
    return true;
}

StyleThemeManager* StyleThemeManager::forWidget(QWidget* widget) {
    for (QWidget* owner = widget; owner; owner = owner->parentWidget()) {
        if (auto* manager = owner->findChild<StyleThemeManager*>(QString(), Qt::FindDirectChildrenOnly))
            return manager;
    }
    return nullptr;
}

void StyleThemeManager::prepareWidget(QWidget* widget) {
    if (auto* manager = forWidget(widget)) {
        manager->prepareSubtree(widget);
        manager->applySubtreeColors(widget);
    }
}

void StyleThemeManager::preparePopup(animatedwidgets::PopupMenu* popup) {
    if (auto* manager = forWidget(popup)) {
        manager->prepareSubtree(popup);
        manager->applyPopupColors(popup);
    }
}

void StyleThemeManager::prepareSubtree(QWidget* widget) {
    if (!widget || widget->property("editorPreview").toBool())
        return;

    if (!widget->property("editorControl").toBool()) {
        widget->setProperty("editorControl", true);
        widget->style()->unpolish(widget);
        widget->style()->polish(widget);
    }
    if (auto* combo = qobject_cast<animatedwidgets::ComboBox*>(widget)) {
        if (!combo->property("themePopupConnected").toBool()) {
            combo->setProperty("themePopupConnected", true);
            connect(combo, &animatedwidgets::ComboBox::popupAboutToShow, this, &StyleThemeManager::preparePopup);
        }
    }

    const auto children = widget->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly);
    for (QWidget* child : children)
        prepareSubtree(child);
}

void StyleThemeManager::applySubtreeColors(QWidget* widget) {
    if (!widget || widget->property("editorPreview").toBool())
        return;

    applyWidgetColors(widget);
    const auto children = widget->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly);
    for (QWidget* child : children)
        applySubtreeColors(child);
}

void StyleThemeManager::applyWidgetColors(QWidget* widget) {
    if (!widget->property("editorControl").toBool())
        return;
    widget->ensurePolished();

    if (auto* tree = qobject_cast<viv::VirtualTreeView*>(widget)) {
        tree->setHoverBackgroundColor(m_scope->hover());
        tree->setSelectedBackgroundColor(tree->objectName() == QStringLiteral("iconTree") ? m_scope->iconSelection() : m_scope->selection());
        tree->setBranchIconColor(m_scope->mutedText());
    } else if (auto* panel = qobject_cast<StylePreviewPanel*>(widget)) {
        panel->setThemeBackground(m_scope->canvas());
    } else if (auto* row = qobject_cast<StylePropertyRow*>(widget)) {
        row->setAutomaticColorBackground(m_scope->automatic());
    } else if (auto* popup = qobject_cast<animatedwidgets::PopupMenu*>(widget)) {
        applyPopupColors(popup);
    }
}

void StyleThemeManager::applyPopupColors(animatedwidgets::PopupMenu* popup) {
    if (!popup || !m_scope)
        return;

    if (auto* style = dynamic_cast<animatedwidgets::DefaultPopupMenuStyle*>(popup->popupStyle())) {
        style->popupBackground = m_scope->surface();
        style->popupBorder = m_scope->border();
    }
    if (auto* style = dynamic_cast<animatedwidgets::DefaultPopupMenuListStyle*>(popup->listStyle())) {
        style->background = Qt::transparent;
        style->itemHoverBackground = m_scope->hover();
        style->itemSelectedBackground = m_scope->iconSelection();
        style->itemText = m_scope->text();
        style->itemDisabledText = m_scope->disabledText();
        style->separator = m_scope->border();
        style->groupTitleText = m_scope->mutedText();
    }
    if (popup->scrollBarStyleSheet() == m_scrollBarStyleSheet)
        popup->refreshStyle();
    else
        popup->setScrollBarStyleSheet(m_scrollBarStyleSheet);
}
