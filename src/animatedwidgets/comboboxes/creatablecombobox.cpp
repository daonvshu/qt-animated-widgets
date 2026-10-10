#include "animatedwidgets/comboboxes/creatablecombobox.h"

#include "animatedwidgets/buttons/hoverbutton.h"
#include "animatedwidgets/comboboxes/combolistpopup.h"
#include "animatedwidgets/utils/combocreationutils.h"

#include <QAbstractItemView>
#include <QAbstractItemModel>
#include <QInputMethodEvent>
#include <QKeyEvent>

namespace animatedwidgets {

CreatableComboBox::CreatableComboBox(QWidget* parent)
    : SearchComboBox(parent)
    , m_createText(tr("Create \"%1\""))
{
    connect(this, &QComboBox::editTextChanged, this, [this] {
        selectCreationEntry(false);
        updateCreationEntry();
    });
}

CreatableComboBox::~CreatableComboBox() {
    disconnectCreationModel();
    disconnect(this, nullptr, this, nullptr);
}

bool CreatableComboBox::creationEnabled() const {
    return m_creationEnabled;
}

void CreatableComboBox::setCreationEnabled(bool enabled) {
    m_creationEnabled = enabled;
    updateCreationEntry();
}

QString CreatableComboBox::createText() const {
    return m_createText;
}

void CreatableComboBox::setCreateText(const QString& text) {
    m_createText = text;
    updateCreationEntry();
}

Qt::CaseSensitivity CreatableComboBox::creationCaseSensitivity() const {
    return m_creationCaseSensitivity;
}

void CreatableComboBox::setCreationCaseSensitivity(Qt::CaseSensitivity sensitivity) {
    m_creationCaseSensitivity = sensitivity;
    updateCreationEntry();
}

int CreatableComboBox::minimumCreateLength() const {
    return m_minimumCreateLength;
}

void CreatableComboBox::setMinimumCreateLength(int length) {
    m_minimumCreateLength = qMax(1, length);
    updateCreationEntry();
}

QString CreatableComboBox::creationCandidate() const {
    if (!m_creationEnabled || !isEnabled() || !isEditable() || !lineEdit()
        || lineEdit()->isReadOnly() || !lineEdit()->hasAcceptableInput() || m_preediting)
        return QString();

    return ComboCreationUtils::candidate(model(), rootModelIndex(), modelColumn(), lineEdit()->text(),
                                         m_creationCaseSensitivity, m_minimumCreateLength);
}

HoverButton* CreatableComboBox::createButton() const {
    return m_createButton;
}

void CreatableComboBox::requestCreation() {
    const QString candidate = creationCandidate();
    if (candidate.isEmpty())
        return;

    QPointer<CreatableComboBox> guard(this);
    hidePopup();
    if (guard)
        emit createRequested(candidate);
}

ComboListPopup* CreatableComboBox::createPopup() {
    auto* popup = new ComboListPopup(this);
    auto* button = new HoverButton(popup);
    button->setFocusPolicy(Qt::NoFocus);
    button->setCheckable(true);
    button->setMinimumHeight(34);
    button->setContentNormal(QColor(QStringLiteral("#1778ff")));
    button->setBackgroundHover(QColor(QStringLiteral("#f2f3f5")));
    button->setBackgroundChecked(QColor(QStringLiteral("#e3f3ff")));

    popup->setBottomWidget(button);
    m_createButton = button;
    connect(button, &QPushButton::clicked, this, &CreatableComboBox::requestCreation);
    return popup;
}

bool CreatableComboBox::canShowPopup() const {
    return SearchComboBox::canShowPopup() || m_creationEnabled;
}

void CreatableComboBox::configurePopup(ComboListPopup* popup) {
    SearchComboBox::configurePopup(popup);
    disconnectCreationModel();

    auto* source = model();
    m_modelConnections.append(connect(source, &QAbstractItemModel::rowsInserted, this, &CreatableComboBox::updateCreationEntry));
    m_modelConnections.append(connect(source, &QAbstractItemModel::rowsRemoved, this, &CreatableComboBox::updateCreationEntry));
    m_modelConnections.append(connect(source, &QAbstractItemModel::dataChanged, this, &CreatableComboBox::updateCreationEntry));
    m_modelConnections.append(connect(source, &QAbstractItemModel::modelReset, this, &CreatableComboBox::updateCreationEntry));

    updateCreationEntry();
}

void CreatableComboBox::disconnectCreationModel() {
    for (const auto& connection : m_modelConnections)
        disconnect(connection);
    m_modelConnections.clear();
}

void CreatableComboBox::popupClosed() {
    disconnectCreationModel();
    m_createButton = nullptr;
    m_creationSelected = false;
    SearchComboBox::popupClosed();
}

void CreatableComboBox::updateCreationEntry() {
    if (!m_createButton)
        return;

    const QString candidate = creationCandidate();
    const bool available = !candidate.isEmpty();
    m_createButton->setVisible(available);
    if (!available)
        selectCreationEntry(false);
    else
        m_createButton->setText(m_createText.contains(QStringLiteral("%1")) ? m_createText.arg(candidate) : m_createText);

    if (auto* popup = currentPopup())
        popup->setAdaptiveHeight(popupWidth() > 0 ? popupWidth() : width(), maxVisibleItems());
}

void CreatableComboBox::selectCreationEntry(bool selected) {
    m_creationSelected = selected;
    if (m_createButton)
        m_createButton->setChecked(selected);
}

bool CreatableComboBox::navigateCreationEntry(int direction) {
    auto* popup = currentPopup();
    if (!popup || creationCandidate().isEmpty())
        return false;

    auto* view = popup->view();
    if (m_creationSelected) {
        selectCreationEntry(false);
        popup->moveSelection(direction);
        if (!view->currentIndex().isValid())
            selectCreationEntry(true);
        return true;
    }

    const QModelIndex current = view->currentIndex();
    const int rows = view->model()->rowCount(view->rootIndex());
    int row = current.isValid() ? current.row() + direction : direction > 0 ? 0 : rows - 1;
    for (; row >= 0 && row < rows; row += direction) {
        const QModelIndex index = view->model()->index(row, modelColumn(), view->rootIndex());
        if (index.flags().testFlag(Qt::ItemIsEnabled) && index.flags().testFlag(Qt::ItemIsSelectable))
            return false;
    }

    view->setCurrentIndex(QModelIndex());
    selectCreationEntry(true);
    return true;
}

bool CreatableComboBox::eventFilter(QObject* watched, QEvent* event) {
    const bool editorEvent = watched == lineEdit() || watched == this;
    if (watched == lineEdit() && event->type() == QEvent::InputMethod) {
        m_preediting = !static_cast<QInputMethodEvent*>(event)->preeditString().isEmpty();
        updateCreationEntry();
    }

    if (editorEvent && event->type() == QEvent::KeyPress) {
        const int key = static_cast<QKeyEvent*>(event)->key();
        if ((key == Qt::Key_Return || key == Qt::Key_Enter) && m_preediting)
            return false;
        if (key == Qt::Key_Return || key == Qt::Key_Enter) {
            auto* popup = currentPopup();
            if (!creationCandidate().isEmpty() && (m_creationSelected || !popup || !popup->view()->currentIndex().isValid())) {
                requestCreation();
                return true;
            }
        }
        if ((key == Qt::Key_Up || key == Qt::Key_Down) && navigateCreationEntry(key == Qt::Key_Down ? 1 : -1))
            return true;
    }

    return SearchComboBox::eventFilter(watched, event);
}

bool CreatableComboBox::event(QEvent* event) {
    const bool handled = SearchComboBox::event(event);
    if (event->type() == QEvent::EnabledChange)
        updateCreationEntry();
    return handled;
}

} // namespace animatedwidgets
