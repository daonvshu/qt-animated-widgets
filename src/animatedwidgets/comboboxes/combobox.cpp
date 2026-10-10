#include "animatedwidgets/comboboxes/combobox.h"
#include "animatedwidgets/comboboxes/combolistpopup.h"
#include "animatedwidgets/utils/inputcontextmenu.h"
#include "animatedwidgets/popups/popupdismissfilter.h"

#include <QApplication>
#include <QAbstractItemView>
#include <QEvent>
#include <QKeyEvent>
#include <QLineEdit>
#include <QMouseEvent>
#include <QPainter>
#include <QSignalBlocker>
#include <QStandardItemModel>
#include <QTimer>

namespace animatedwidgets {

ComboBox::ComboBox(QWidget* parent)
    : QComboBox(parent)
    , m_borderTransition(this)
    , m_backgroundTransition(this)
{
    setFrame(false);
    setFocusPolicy(Qt::ClickFocus);
    setAttribute(Qt::WA_Hover, true);
    Q_INIT_RESOURCE(comboboxes);
    m_arrowIcon = QIcon(QStringLiteral(":/animatedwidgets/comboboxes/dropdown.svg"));

    for (auto* transition : {&m_borderTransition, &m_backgroundTransition}) {
        transition->setDuration(m_animationDuration);
        transition->setEasingCurve(QEasingCurve::OutCubic);
        connect(transition, &ValueTransition::valueChanged, this, [this] { update(); });
    }
    connect(qApp, &QApplication::focusChanged, this, [this](QWidget* old, QWidget* current) {
        if (old == this || current == this || (old && isAncestorOf(old)) || (current && isAncestorOf(current)))
            refreshState();
    });
    refreshState(true);
}

bool ComboBox::addGroupTitle(const QString& text) {
    return insertGroupTitle(count(), text);
}

bool ComboBox::insertGroupTitle(int index, const QString& text) {
    auto* items = qobject_cast<QStandardItemModel*>(model());
    if (!items)
        return false;

    QStandardItem* root = rootModelIndex().isValid()
        ? items->itemFromIndex(rootModelIndex()) : items->invisibleRootItem();
    if (!root)
        return false;

    QList<QStandardItem*> row;
    for (int column = 0; column <= modelColumn(); ++column) {
        auto* item = new QStandardItem(column == modelColumn() ? text : QString());
        item->setData(QStringLiteral("groupTitle"), Qt::AccessibleDescriptionRole);
        item->setFlags(Qt::NoItemFlags);
        row.append(item);
    }

    // Do not let QComboBox select a heading when inserting into an empty model.
    const bool hadSelection = currentIndex() >= 0;
    QSignalBlocker blocker(this);
    if (hadSelection)
        blocker.unblock();
    root->insertRow(qBound(0, index, count()), row);
    if (!hadSelection)
        setCurrentIndex(-1);
    return true;
}

bool ComboBox::isGroupTitle(int index) const {
    return index >= 0 && index < count()
        && itemData(index, Qt::AccessibleDescriptionRole).toString() == QStringLiteral("groupTitle");
}

bool ComboBox::popupOnEdit() const {
    return m_popupOnEdit;
}

void ComboBox::setPopupOnEdit(bool enabled) {
    m_popupOnEdit = enabled;
    updateCompleter();
}

bool ComboBox::hasError() const {
    return m_hasError;
}

void ComboBox::setHasError(bool error) {
    if (m_hasError == error)
        return;
    m_hasError = error;
    refreshState();
    emit hasErrorChanged(error);
}

QColor ComboBox::borderNormal() const {
    return m_normalBorderColor;
}

void ComboBox::setBorderNormal(const QColor& color) {
    m_normalBorderColor = color;
    refreshState();
}

QColor ComboBox::borderHover() const {
    return m_hoverBorderColor;
}

void ComboBox::setBorderHover(const QColor& color) {
    m_hoverBorderColor = color;
    refreshState();
}

QColor ComboBox::borderFocus() const {
    return m_focusBorderColor;
}

void ComboBox::setBorderFocus(const QColor& color) {
    m_focusBorderColor = color;
    refreshState();
}

QColor ComboBox::borderError() const {
    return m_errorBorderColor;
}

void ComboBox::setBorderError(const QColor& color) {
    m_errorBorderColor = color;
    refreshState();
}

QColor ComboBox::borderDisabled() const {
    return m_disabledBorderColor;
}

void ComboBox::setBorderDisabled(const QColor& color) {
    m_disabledBorderColor = color;
    refreshState();
}

QColor ComboBox::backgroundNormal() const {
    return m_normalBackgroundColor;
}

void ComboBox::setBackgroundNormal(const QColor& color) {
    m_normalBackgroundColor = color;
    refreshState();
}

QColor ComboBox::backgroundHover() const {
    return m_hoverBackgroundColor;
}

void ComboBox::setBackgroundHover(const QColor& color) {
    m_hoverBackgroundColor = color;
    refreshState();
}

QColor ComboBox::backgroundFocus() const {
    return m_focusBackgroundColor;
}

void ComboBox::setBackgroundFocus(const QColor& color) {
    m_focusBackgroundColor = color;
    refreshState();
}

QColor ComboBox::backgroundError() const {
    return m_errorBackgroundColor;
}

void ComboBox::setBackgroundError(const QColor& color) {
    m_errorBackgroundColor = color;
    refreshState();
}

QColor ComboBox::backgroundDisabled() const {
    return m_disabledBackgroundColor;
}

void ComboBox::setBackgroundDisabled(const QColor& color) {
    m_disabledBackgroundColor = color;
    refreshState();
}

QColor ComboBox::textColor() const {
    return m_textColor;
}

void ComboBox::setTextColor(const QColor& color) {
    m_textColor = color;
    updateEditorPalette();
    update();
}

QColor ComboBox::disabledTextColor() const {
    return m_disabledTextColor;
}

void ComboBox::setDisabledTextColor(const QColor& color) {
    m_disabledTextColor = color;
    updateEditorPalette();
    update();
}

int ComboBox::borderWidth() const {
    return m_borderWidth;
}

void ComboBox::setBorderWidth(int width) {
    m_borderWidth = qMax(0, width);
    update();
}

bool ComboBox::isBorderVisible() const {
    return m_borderVisible;
}

void ComboBox::setBorderVisible(bool visible) {
    m_borderVisible = visible;
    update();
}

int ComboBox::cornerRadius() const {
    return m_cornerRadius;
}

void ComboBox::setCornerRadius(int radius) {
    m_cornerRadius = qMax(0, radius);
    update();
}

int ComboBox::contentPadding() const {
    return m_contentPadding;
}

void ComboBox::setContentPadding(int padding) {
    m_contentPadding = qMax(0, padding);
    updateEditorMargins();
    updateGeometry();
    update();
}

int ComboBox::animationDuration() const {
    return m_animationDuration;
}

void ComboBox::setAnimationDuration(int duration) {
    m_animationDuration = qMax(0, duration);
    m_borderTransition.setDuration(m_animationDuration);
    m_backgroundTransition.setDuration(m_animationDuration);
    if (m_animationDuration == 0)
        refreshState(true);
}

QIcon ComboBox::arrowIcon() const {
    return m_arrowIcon;
}

void ComboBox::setArrowIcon(const QIcon& icon) {
    m_arrowIcon = icon;
    update();
}

QSize ComboBox::arrowIconSize() const {
    return m_arrowIconSize;
}

void ComboBox::setArrowIconSize(const QSize& size) {
    m_arrowIconSize = QSize(qMax(1, size.width()), qMax(1, size.height()));
    updateEditorMargins();
    updateGeometry();
    update();
}

int ComboBox::arrowRightMargin() const {
    return m_arrowRightMargin;
}

void ComboBox::setArrowRightMargin(int margin) {
    m_arrowRightMargin = qMax(0, margin);
    updateEditorMargins();
    updateGeometry();
    update();
}

bool ComboBox::arrowVisible() const {
    return m_arrowVisible;
}

int ComboBox::popupWidth() const {
    return m_popupWidth;
}

void ComboBox::setPopupWidth(int width) {
    m_popupWidth = qMax(0, width);
}

Qt::Alignment ComboBox::popupAlign() const {
    return m_popupAlign;
}

void ComboBox::setPopupAlign(Qt::Alignment align) {
    m_popupAlign = align;
}

QString ComboBox::popupScrollBarStyleSheet() const {
    return m_popupScrollBarStyleSheet;
}

void ComboBox::setPopupScrollBarStyleSheet(const QString& styleSheet) {
    m_popupScrollBarStyleSheet = styleSheet;
    if (m_popup)
        m_popup->setScrollBarStyleSheet(styleSheet);
}

void ComboBox::setArrowVisible(bool visible) {
    m_arrowVisible = visible;
    updateEditorMargins();
    updateGeometry();
    update();
}

QSize ComboBox::sizeHint() const {
    QSize hint = QComboBox::sizeHint();
    hint.setWidth(qMax(hint.width(), 120));
    hint.setHeight(qMax(hint.height(), 34));
    return hint;
}

QColor ComboBox::targetBorderColor() const {
    if (!isEnabled())
        return m_disabledBorderColor;
    if (m_hasError)
        return m_errorBorderColor;
    QWidget* focused = QApplication::focusWidget();
    if (m_popupOpen || hasFocus() || (focused && isAncestorOf(focused)))
        return m_focusBorderColor;
    if (m_hovered)
        return m_hoverBorderColor;
    return m_normalBorderColor;
}

QColor ComboBox::targetBackgroundColor() const {
    if (!isEnabled())
        return m_disabledBackgroundColor;
    if (m_hasError)
        return m_errorBackgroundColor;
    QWidget* focused = QApplication::focusWidget();
    if (m_popupOpen || hasFocus() || (focused && isAncestorOf(focused)))
        return m_focusBackgroundColor;
    if (m_hovered && m_hoverBackgroundColor.isValid())
        return m_hoverBackgroundColor;
    return m_normalBackgroundColor;
}

void ComboBox::refreshState(bool immediate) {
    m_borderTransition.setTarget(targetBorderColor(), immediate);
    m_backgroundTransition.setTarget(targetBackgroundColor(), immediate);
}

void ComboBox::syncEditor() {
    updateCompleter();
    QLineEdit* editor = lineEdit();
    if (editor == m_editor)
        return;
    if (m_editor)
        m_editor->removeEventFilter(this);
    if (m_editor)
        disconnect(m_editor, &QLineEdit::textEdited, this, nullptr);
    m_editor = editor;
    if (!m_editor)
        return;
    InputContextMenu::install(m_editor);
    m_editor->setFrame(false);
    m_editor->setAutoFillBackground(false);
    m_editor->installEventFilter(this);
    connect(m_editor.data(), &QLineEdit::textEdited, this, [this] {
        m_popupNavigation = false;
        // Run after textEdited handlers so derived controls can update their query first.
        QTimer::singleShot(0, this, [this] {
            if (m_popupOnEdit && isEditable() && isEnabled() && lineEdit()
                && lineEdit()->hasFocus() && !lineEdit()->isReadOnly() && !currentPopup())
                showPopup();
        });
    });
    updateEditorPalette();
    updateEditorMargins();
}

void ComboBox::updateCompleter() {
    QCompleter* current = completer();
    if (m_inlineCompleter && (!m_popupOnEdit || current != m_inlineCompleter)) {
        m_inlineCompleter->setCompletionMode(m_savedCompletionMode);
        m_inlineCompleter = nullptr;
    }
    if (m_popupOnEdit && current && !m_inlineCompleter) {
        m_inlineCompleter = current;
        m_savedCompletionMode = current->completionMode();
        current->setCompletionMode(QCompleter::InlineCompletion);
    }
}

void ComboBox::updateEditorPalette() {
    if (!m_editor)
        return;
    QPalette colors = m_editor->palette();
    const QColor transparent(0, 0, 0, 0);
    for (auto group : {QPalette::Active, QPalette::Inactive, QPalette::Disabled})
        colors.setColor(group, QPalette::Base, transparent);
    colors.setColor(QPalette::Active, QPalette::Text, m_textColor);
    colors.setColor(QPalette::Inactive, QPalette::Text, m_textColor);
    colors.setColor(QPalette::Disabled, QPalette::Text, m_disabledTextColor);
    m_editor->setPalette(colors);
}

void ComboBox::updateEditorMargins() {
    if (!m_editor)
        return;

    const int inset = m_borderVisible ? m_borderWidth : 0;
    QRect editorRect = rect().adjusted(inset, inset, -inset, -inset);
    const QRect buttonRect = arrowButtonRect();
    if (!buttonRect.isEmpty()) {
        if (layoutDirection() == Qt::RightToLeft)
            editorRect.setLeft(qMax(editorRect.left(), buttonRect.right() + 1));
        else
            editorRect.setRight(qMin(editorRect.right(), buttonRect.left() - 1));
    }

    m_editor->setGeometry(editorRect);
    m_editor->setTextMargins(m_contentPadding, 0, m_contentPadding, 0);
}

QRect ComboBox::arrowButtonRect() const {
    if (!m_arrowVisible)
        return QRect();

    const int buttonWidth = qMin(width(), m_arrowIconSize.width() + m_arrowRightMargin + 8);
    if (layoutDirection() == Qt::RightToLeft)
        return QRect(0, 0, buttonWidth, height());
    return QRect(width() - buttonWidth, 0, buttonWidth, height());
}

void ComboBox::mousePressEvent(QMouseEvent* event) {
    if (isEditable() && event->button() == Qt::LeftButton && arrowButtonRect().contains(event->pos())) {
        event->accept();
        if (m_popup)
            hidePopup();
        else
            showPopup();
        return;
    }

    QComboBox::mousePressEvent(event);
}

bool ComboBox::eventFilter(QObject* watched, QEvent* event) {
    if (watched == lineEdit() && isEditable()) {
        if (event->type() == QEvent::MouseButtonRelease
            && static_cast<QMouseEvent*>(event)->button() == Qt::LeftButton) {
            // Preserve the editor's cursor and selection handling before opening the popup.
            QTimer::singleShot(0, this, [this] {
                if (m_popupOnEdit && isEditable() && isEnabled() && lineEdit()
                    && lineEdit()->hasFocus() && !lineEdit()->isReadOnly() && !currentPopup())
                    showPopup();
            });
        }
        if (event->type() == QEvent::FocusOut) {
            QTimer::singleShot(0, this, [this] {
                if (lineEdit() && !lineEdit()->hasFocus() && currentPopup())
                    hidePopup();
            });
        }
        if (event->type() == QEvent::KeyPress && m_popup) {
            const auto* key = static_cast<QKeyEvent*>(event);
            if (key->key() == Qt::Key_Escape) {
                hidePopup();
                return true;
            }
            if (key->key() == Qt::Key_Up || key->key() == Qt::Key_Down) {
                m_popupNavigation = true;
                m_popup->moveSelection(key->key() == Qt::Key_Down ? 1 : -1);
                return true;
            }
            if (key->key() == Qt::Key_Return || key->key() == Qt::Key_Enter) {
                if (m_popupNavigation) {
                    m_popup->triggerCurrentItem();
                    return true;
                }
                hidePopup();
            } else if (key->key() == Qt::Key_Tab || key->key() == Qt::Key_Backtab) {
                hidePopup();
            }
        }
    }
    return QComboBox::eventFilter(watched, event);
}

bool ComboBox::event(QEvent* event) {
    if (event->type() == QEvent::Enter)
        m_hovered = true;
    if (event->type() == QEvent::Leave)
        m_hovered = false;

    const bool handled = QComboBox::event(event);
    switch (event->type()) {
        case QEvent::Enter:
        case QEvent::Leave:
        case QEvent::FocusIn:
        case QEvent::FocusOut:
        case QEvent::EnabledChange:
            refreshState();
            break;
        case QEvent::LayoutDirectionChange:
        case QEvent::Resize:
        case QEvent::StyleChange:
        case QEvent::FontChange:
        case QEvent::ContentsRectChange:
        case QEvent::LayoutRequest:
        case QEvent::Show:
            syncEditor();
            updateEditorMargins();
            break;
        default:
            break;
    }
    return handled;
}

void ComboBox::paintEvent(QPaintEvent*) {
    syncEditor();
    updateEditorMargins();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    const qreal inset = m_borderVisible ? m_borderWidth / 2.0 : 0.0;
    const QRectF frame = QRectF(rect()).adjusted(inset, inset, -inset, -inset);
    painter.setBrush(m_backgroundTransition.current().value<QColor>());
    painter.setPen(m_borderVisible && m_borderWidth > 0 ? QPen(m_borderTransition.current().value<QColor>(), m_borderWidth) : QPen(Qt::NoPen));
    painter.drawRoundedRect(frame, m_cornerRadius, m_cornerRadius);

    const bool rightToLeft = layoutDirection() == Qt::RightToLeft;
    if (m_arrowVisible && !m_arrowIcon.isNull()) {
        const int arrowX = rightToLeft ? m_arrowRightMargin : width() - m_arrowRightMargin - m_arrowIconSize.width();
        const QRect arrowRect(arrowX, (height() - m_arrowIconSize.height()) / 2, m_arrowIconSize.width(), m_arrowIconSize.height());
        m_arrowIcon.paint(&painter, arrowRect, Qt::AlignCenter, isEnabled() ? QIcon::Normal : QIcon::Disabled);
    }

    if (isEditable())
        return;

    const int arrowSpace = m_arrowVisible ? m_arrowIconSize.width() + m_arrowRightMargin + 8 : 0;
    QRect textRect = rect().adjusted(m_contentPadding, 0, -m_contentPadding, 0);
    if (rightToLeft)
        textRect.setLeft(textRect.left() + arrowSpace);
    else
        textRect.setRight(textRect.right() - arrowSpace);

    const QIcon itemIcon = currentIndex() >= 0 ? this->itemIcon(currentIndex()) : QIcon();
    if (!itemIcon.isNull() && textRect.width() > 0) {
        const QSize itemSize = iconSize().boundedTo(textRect.size());
        const int iconX = rightToLeft ? textRect.right() - itemSize.width() + 1 : textRect.left();
        itemIcon.paint(&painter, QRect(iconX, (height() - itemSize.height()) / 2, itemSize.width(), itemSize.height()), Qt::AlignCenter,
                       isEnabled() ? QIcon::Normal : QIcon::Disabled);
        if (rightToLeft)
            textRect.setRight(iconX - 7);
        else
            textRect.setLeft(iconX + itemSize.width() + 6);
    }

    painter.setPen(isEnabled() ? m_textColor : m_disabledTextColor);
    const QString label = fontMetrics().elidedText(displayText(), Qt::ElideRight, qMax(0, textRect.width()));
    painter.drawText(textRect, Qt::AlignVCenter | (rightToLeft ? Qt::AlignRight : Qt::AlignLeft), label);
}

void ComboBox::showPopup() {
    if (m_popup || !canShowPopup() || !isEnabled())
        return;

    m_popupNavigation = false;
    auto* popup = createPopup();
    if (!popup)
        return;
    m_popup = popup;
    if (!m_popupScrollBarStyleSheet.isNull())
        popup->setScrollBarStyleSheet(m_popupScrollBarStyleSheet);

    connect(popup, &ComboListPopup::modelIndexChosen, this, [this](const QModelIndex& index) {
        QPointer<ComboBox> guard(this);
        choosePopupIndex(index);
        if (!guard)
            return;
        emit activated(currentIndex());
        if (!guard)
            return;
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
        emit textActivated(currentText());
        if (!guard)
            return;
#endif
        hidePopup();
    });
    connect(this, QOverload<int>::of(&QComboBox::currentIndexChanged), popup, [this, popup](int row) {
        popup->setCurrentModelIndex(model()->index(row, modelColumn(), rootModelIndex()));
    });
    connect(popup, &BasePopupDlg::closeRequested, this, [this, popup] {
        if (m_popup == popup) {
            m_popup = nullptr;
            m_popupOpen = false;
            popupClosed();
            refreshState();
        }
    });
    connect(popup, &QObject::destroyed, this, [this] {
        if (!m_popup) {
            m_popupOpen = false;
            refreshState();
        }
    });

    QPointer<ComboBox> guard(this);
    emit popupAboutToShow(popup);
    if (!guard || m_popup != popup)
        return;
    popup->view()->setIconSize(iconSize());
    popup->setItems(model(), popupRootModelIndex(), modelColumn(), currentIndex(), maxVisibleItems(), m_popupWidth > 0 ? m_popupWidth : width());
    configurePopup(popup);
    popup->moveToTargetBottom(this, m_popupAlign);
    m_popupOpen = true;
    refreshState();
    popup->show();
}

void ComboBox::hidePopup() {
    m_popupNavigation = false;
    if (m_popup) {
        ComboListPopup* popup = m_popup;
        m_popup = nullptr;
        popupClosed();
        popup->close();
    }
    m_popupOpen = false;
    refreshState();
}

ComboListPopup* ComboBox::currentPopup() const {
    return m_popup;
}

ComboListPopup* ComboBox::createPopup() {
    return new ComboListPopup(this);
}

bool ComboBox::canShowPopup() const {
    return model()->rowCount(popupRootModelIndex()) > 0;
}

QString ComboBox::displayText() const {
    return currentText();
}

QModelIndex ComboBox::popupRootModelIndex() const {
    return rootModelIndex();
}

void ComboBox::choosePopupIndex(const QModelIndex& index) {
    setCurrentIndex(index.row());
}

void ComboBox::configurePopup(ComboListPopup* popup) {
    if (!isEditable())
        return;

    // Editable combos keep keyboard and IME input in their editor.
    popup->setWindowFlags((popup->windowFlags() & ~Qt::WindowFlags(Qt::WindowType_Mask)) | Qt::Tool | Qt::WindowDoesNotAcceptFocus);
    popup->setAttribute(Qt::WA_ShowWithoutActivating);
    popup->setFocusProxy(nullptr);
    popup->setFocusPolicy(Qt::NoFocus);
    popup->view()->setFocusPolicy(Qt::NoFocus);

    auto* dismissFilter = new PopupDismissFilter(popup, this);
    connect(dismissFilter, &PopupDismissFilter::dismissRequested, popup, &QWidget::close);
    connect(popup, &BasePopupDlg::closeRequested, dismissFilter, &PopupDismissFilter::stop);
}

void ComboBox::popupClosed() {
}

} // namespace animatedwidgets
