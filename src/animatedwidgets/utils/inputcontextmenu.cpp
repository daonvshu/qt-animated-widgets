#include "animatedwidgets/utils/inputcontextmenu.h"

#include "animatedwidgets/popups/popupmenu.h"
#include "animatedwidgets/popups/popupmenuitemdelegate.h"
#include "animatedwidgets/popups/popupmenuliststyle.h"
#include "animatedwidgets/popups/popupmenustyle.h"

#include <QAbstractItemView>
#include <QAction>
#include <QCoreApplication>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QMenu>
#include <QPlainTextEdit>
#include <QSpinBox>
#include <QStandardItemModel>
#include <QTextCursor>
#include <QTextDocument>

namespace animatedwidgets {

InputContextMenu::InputContextMenu(QWidget* editor, QAbstractSpinBox* spinBox)
    : QObject(editor)
    , m_editor(editor)
    , m_spinBox(spinBox)
{
    if (auto* plainTextEdit = qobject_cast<QPlainTextEdit*>(editor))
        connectMenuRequest(editor, plainTextEdit->viewport());
    else
        connectMenuRequest(editor, editor);

    if (spinBox)
        connectMenuRequest(spinBox, spinBox);
}

void InputContextMenu::install(QWidget* editor, QAbstractSpinBox* spinBox) {
    if (!editor || editor->findChild<InputContextMenu*>(QString(), Qt::FindDirectChildrenOnly))
        return;

    new InputContextMenu(editor, spinBox);
}

void InputContextMenu::connectMenuRequest(QWidget* widget, QWidget* coordinateWidget) {
    widget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(widget, &QWidget::customContextMenuRequested, this, [this, coordinateWidget](const QPoint& pos) {
        showMenu(coordinateWidget->mapToGlobal(pos));
    });
}

void InputContextMenu::showMenu(const QPoint& globalPos) {
    const auto restoreSelection = captureSelection(m_editor);
    QMenu* standardMenu = nullptr;
    if (auto* lineEdit = qobject_cast<QLineEdit*>(m_editor))
        standardMenu = lineEdit->createStandardContextMenu();
    else if (auto* plainTextEdit = qobject_cast<QPlainTextEdit*>(m_editor))
        standardMenu = plainTextEdit->createStandardContextMenu(plainTextEdit->viewport()->mapFromGlobal(globalPos));
    if (!standardMenu)
        return;

    appendStepActions(standardMenu);
    auto* popup = new PopupMenu(m_editor);
    popup->setWindowFlag(Qt::WindowDoesNotAcceptFocus);
    popup->setAttribute(Qt::WA_ShowWithoutActivating);
    popup->setFocusProxy(nullptr);
    popup->setFocusPolicy(Qt::NoFocus);
    popup->view()->setFocusPolicy(Qt::NoFocus);
    popup->setFont(m_editor->font());
    standardMenu->setParent(popup, standardMenu->windowFlags());
    auto* model = new QStandardItemModel(popup);
    QList<QPointer<QAction>> actions;
    for (auto* action : standardMenu->actions()) {
        if (!action->isVisible())
            continue;

        auto* item = new QStandardItem;
        item->setFlags(action->isEnabled() && !action->isSeparator()
                           ? Qt::ItemIsEnabled | Qt::ItemIsSelectable : Qt::NoItemFlags);
        if (action->isSeparator()) {
            item->setData(true, PopupMenuItemDelegate::SeparatorRole);
        } else {
            const QString text = action->text();
            item->setText(displayText(text.section(QLatin1Char('\t'), 0, 0)));
            item->setIcon(action->icon());
            const QString shortcut = text.contains(QLatin1Char('\t'))
                ? text.section(QLatin1Char('\t'), 1) : action->shortcut().toString(QKeySequence::NativeText);
            item->setData(shortcut, PopupMenuItemDelegate::ShortcutRole);
            if (action->isCheckable())
                item->setData(action->isChecked() ? Qt::Checked : Qt::Unchecked, Qt::CheckStateRole);
        }
        model->appendRow(item);
        actions.append(action);
    }
    popup->setModel(model);

    connect(popup, &PopupMenu::itemTriggered, popup, [actions, restoreSelection](const QModelIndex& index) {
        const QPointer<QAction> action = actions.value(index.row());
        if (!action || !action->isEnabled())
            return;

        restoreSelection();
        if (action && action->isEnabled())
            action->trigger();
    });

    const QMargins listMargins = popup->listStyle()->margins();
    const QMargins bodyMargins = popup->popupStyle()->margins();
    const int width = popup->view()->sizeHintForColumn(0) + listMargins.left()
        + listMargins.right() + bodyMargins.left() + bodyMargins.right();
    popup->setAdaptiveHeight(qMax(180, width));
    popup->showAt(globalPos);
}

void InputContextMenu::appendStepActions(QMenu* menu) const {
    if (!m_spinBox || m_spinBox->isReadOnly())
        return;

    bool canStepUp = false;
    bool canStepDown = false;
    if (auto* spinBox = qobject_cast<QSpinBox*>(m_spinBox.data())) {
        canStepUp = spinBox->wrapping() || spinBox->value() < spinBox->maximum();
        canStepDown = spinBox->wrapping() || spinBox->value() > spinBox->minimum();
    } else if (auto* spinBox = qobject_cast<QDoubleSpinBox*>(m_spinBox.data())) {
        canStepUp = spinBox->wrapping() || spinBox->value() < spinBox->maximum();
        canStepDown = spinBox->wrapping() || spinBox->value() > spinBox->minimum();
    }

    menu->addSeparator();
    auto* stepUp = menu->addAction(QCoreApplication::translate("QAbstractSpinBox", "Step &up"));
    auto* stepDown = menu->addAction(QCoreApplication::translate("QAbstractSpinBox", "Step &down"));
    stepUp->setEnabled(canStepUp);
    stepDown->setEnabled(canStepDown);
    connect(stepUp, &QAction::triggered, m_spinBox.data(), &QAbstractSpinBox::stepUp);
    connect(stepDown, &QAction::triggered, m_spinBox.data(), &QAbstractSpinBox::stepDown);
}

std::function<void()> InputContextMenu::captureSelection(QWidget* editor) {
    if (auto* input = qobject_cast<QLineEdit*>(editor)) {
        const QPointer<QLineEdit> guard(input);
        const QString text = input->text();
        const int cursor = input->cursorPosition();
        const int start = input->selectionStart();
        const int end = start + static_cast<int>(input->selectedText().size());
        const int anchor = start < 0 ? cursor : cursor == start ? end : start;
        return [guard, text, anchor, cursor] {
            if (guard && guard->text() == text)
                guard->setSelection(anchor, cursor - anchor);
        };
    }
    if (auto* input = qobject_cast<QPlainTextEdit*>(editor)) {
        const QPointer<QPlainTextEdit> guard(input);
        const QTextCursor cursor = input->textCursor();
        const int revision = input->document()->revision();
        return [guard, cursor, revision] {
            if (guard && cursor.document() == guard->document() && guard->document()->revision() == revision)
                guard->setTextCursor(cursor);
        };
    }
    return [] {};
}

QString InputContextMenu::displayText(const QString& text) {
    QString result;
    for (int i = 0; i < text.size(); ++i) {
        if (text.at(i) != QLatin1Char('&')) {
            result.append(text.at(i));
        } else if (i + 1 < text.size() && text.at(i + 1) == QLatin1Char('&')) {
            result.append(QLatin1Char('&'));
            ++i;
        }
    }
    return result;
}

} // namespace animatedwidgets
