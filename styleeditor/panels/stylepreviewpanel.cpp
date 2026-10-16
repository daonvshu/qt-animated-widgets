#include "stylepreviewpanel.h"

#include "../dialogs/colorpickerdialog.h"
#include "../preview/stylepreviewclient.h"
#include "../preview/stylepreviewprotocol.h"
#include "../utils/styledocumentcodec.h"
#include "../utils/stylepreviewutils.h"

#include "animatedwidgets/utils/smoothscrolling.h"

#include <QEvent>
#include <QIntValidator>
#include <QJsonArray>
#include <QScopedValueRollback>
#include <QShowEvent>
#include <QWindow>

StylePreviewPanel::StylePreviewPanel(QWidget* parent)
    : QWidget(parent)
{
    createLayout();
}

StylePreviewPanel::~StylePreviewPanel() {
    // Detach the foreign window before the panel's container is destroyed.
    m_client->stop();
    disconnect(m_client, nullptr, this, nullptr);
}

void StylePreviewPanel::createLayout() {
    ui.setupUi(this);
    m_layoutReady = true;
    ui.previewWidthEdit->setValidator(new QIntValidator(1, 4096, ui.previewWidthEdit));
    ui.previewHeightEdit->setValidator(new QIntValidator(1, 4096, ui.previewHeightEdit));
    animatedwidgets::SmoothScrolling::install(ui.previewScrollArea);
    ui.previewScrollArea->viewport()->setAutoFillBackground(false);
    ui.previewHost->setAutoFillBackground(false);

    m_client = new StylePreviewClient(this);
    connect(m_client, &StylePreviewClient::starting, this, [this] { showStatus(QStringLiteral("Starting isolated preview..."), false); });
    connect(m_client, &StylePreviewClient::windowReady, this, &StylePreviewPanel::attachWindow);
    connect(m_client, &StylePreviewClient::windowClosing, this, &StylePreviewPanel::detachWindow);
    connect(m_client, &StylePreviewClient::updated, this, &StylePreviewPanel::receivePreview);
    connect(m_client, &StylePreviewClient::failed, this, [this](const QString& error) { showStatus(error, true); });

    setPreview(nullptr);
}

void StylePreviewPanel::setPreview(const StyleEditorItem* item, const StyleResourceSettings& resources, int state) {
    const QString previousId = m_itemData.value(QStringLiteral("id")).toString();
    const QString previousType = m_itemData.value(QStringLiteral("typeId")).toString();
    m_resources = resources;
    m_state = state;
    m_itemData = {};
    if (item) {
        m_itemData.insert(QStringLiteral("id"), item->id);
        m_itemData.insert(QStringLiteral("typeId"), item->widgetTypeId);
        m_itemData.insert(QStringLiteral("name"), item->displayName);
        QJsonObject properties = item->serializedProperties;
        const QJsonObject values = StyleDocumentCodec::serializeProperties(item->properties);
        for (auto it = values.constBegin(); it != values.constEnd(); ++it)
            properties.insert(it.key(), it.value());
        m_itemData.insert(QStringLiteral("properties"), properties);
        m_itemData.insert(QStringLiteral("inheritedProperties"), StylePreviewProtocol::encodeProperties(item->inheritedProperties));
        m_settings = item->previewSettings;
        m_suggestedSize = item->suggestedSize;
    } else {
        m_settings = {};
        m_suggestedSize = {};
    }
    if (previousId != m_itemData.value(QStringLiteral("id")).toString()
        || previousType != m_itemData.value(QStringLiteral("typeId")).toString()) {
        ++m_generation;
        m_sizeHint = {};
    }

    QWidget* const controls[] = {ui.previewWidthEdit, ui.previewHeightEdit,
                                ui.previewWidthAutoButton, ui.previewHeightAutoButton,
                                ui.previewBackgroundButton, ui.resetPreviewButton};
    for (QWidget* control : controls)
        control->setEnabled(item != nullptr);
    if (item) {
        updateSize();
    } else {
        ui.previewWidthEdit->clear();
        ui.previewHeightEdit->clear();
        ui.previewWidthAutoButton->setChecked(false);
        ui.previewHeightAutoButton->setChecked(false);
        showStatus(QStringLiteral("Select a widget style to preview."), false);
    }
    updateBackground();
    requestPreview();
}

void StylePreviewPanel::updateSize() {
    if (m_itemData.isEmpty())
        return;
    const QSize size = StylePreviewUtils::resolveSize(m_settings.size, m_suggestedSize, m_sizeHint,
                                                      m_settings.widthAuto, m_settings.heightAuto);
    ui.previewWidthEdit->setText(QString::number(size.width()));
    ui.previewHeightEdit->setText(QString::number(size.height()));
    ui.previewWidthAutoButton->setChecked(m_settings.widthAuto);
    ui.previewHeightAutoButton->setChecked(m_settings.heightAuto);
    ui.previewWidthEdit->setEnabled(!m_settings.widthAuto);
    ui.previewHeightEdit->setEnabled(!m_settings.heightAuto);
}

void StylePreviewPanel::requestPreview() {
    if (!m_client)
        return;
    QJsonObject snapshot;
    snapshot.insert(QStringLiteral("item"), m_itemData);
    snapshot.insert(QStringLiteral("state"), m_state);
    const QColor background = m_settings.background.isValid() ? m_settings.background : m_themeBackground;
    snapshot.insert(QStringLiteral("background"), background.isValid() ? background.name(QColor::HexArgb) : QString());

    QJsonObject settings;
    settings.insert(QStringLiteral("width"), m_settings.size.width());
    settings.insert(QStringLiteral("height"), m_settings.size.height());
    settings.insert(QStringLiteral("widthAuto"), m_settings.widthAuto);
    settings.insert(QStringLiteral("heightAuto"), m_settings.heightAuto);
    snapshot.insert(QStringLiteral("settings"), settings);
    QJsonObject resources;
    resources.insert(QStringLiteral("directory"), m_resources.projectDirectory);
    resources.insert(QStringLiteral("source"), m_resources.source);
    resources.insert(QStringLiteral("qrcFiles"), QJsonArray::fromStringList(m_resources.qrcFiles));
    snapshot.insert(QStringLiteral("resources"), resources);
    m_client->request(snapshot);
}

void StylePreviewPanel::attachWindow(quintptr windowId) {
    detachWindow();
    QWindow* window = QWindow::fromWinId(static_cast<WId>(windowId));
    if (!window) {
        m_client->stop();
        showStatus(QStringLiteral("This platform could not embed the preview window."), true);
        return;
    }
    m_foreignWindow = window;
    m_container = QWidget::createWindowContainer(window, ui.previewHost);
    m_container->hide();
    m_container->setAttribute(Qt::WA_NativeWindow);
    m_container->setFocusPolicy(Qt::StrongFocus);
    m_container->setMinimumSize(1, 1);
    QSizePolicy policy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    policy.setRetainSizeWhenHidden(true);
    m_container->setSizePolicy(policy);
    m_container->installEventFilter(this);
    ui.previewWidgetLayout->addWidget(m_container);
    ui.previewStatusLabel->hide();
    ui.previewRetryButton->hide();
    ui.previewWidgetLayout->activate();

    const WId hostWindowId = m_container->winId();
    window->setParent(m_container->windowHandle());
    policy.setRetainSizeWhenHidden(false);
    m_container->setSizePolicy(policy);
    m_client->confirmWindow(static_cast<quintptr>(hostWindowId), m_container->size());
}

void StylePreviewPanel::detachWindow() {
    QPointer<QWindow> window = m_foreignWindow;
    m_foreignWindow = nullptr;
    if (window) {
        window->hide();
        window->setParent(nullptr);
    }
    if (m_container) {
        m_container->removeEventFilter(this);
        ui.previewWidgetLayout->removeWidget(m_container);
        delete m_container.data();
    }
    m_container = nullptr;
    delete window.data();
}

void StylePreviewPanel::receivePreview(const QJsonObject& message) {
    const QString itemId = message.value(QStringLiteral("itemId")).toString();
    if (m_itemData.isEmpty() || itemId != m_itemData.value(QStringLiteral("id")).toString())
        return;
    QList<animatedwidgets::styleeditor::StyleProperty> properties;
    if (!StylePreviewProtocol::decodeProperties(message.value(QStringLiteral("properties")).toArray(), properties)) {
        showStatus(QStringLiteral("The preview plugin returned invalid property metadata."), true);
        return;
    }
    m_sizeHint = QSize(message.value(QStringLiteral("hintWidth")).toInt(), message.value(QStringLiteral("hintHeight")).toInt());
    updateSize();
    if (m_container) {
        m_container->setMinimumSize(message.value(QStringLiteral("width")).toInt(1), message.value(QStringLiteral("height")).toInt(1));
        m_container->show();
    }
    ui.previewStatusLabel->hide();
    ui.previewRetryButton->hide();
    emit previewUpdated(itemId, properties, message.value(QStringLiteral("code")).toString());
}

void StylePreviewPanel::showStatus(const QString& text, bool retry) {
    if (m_container)
        m_container->hide();
    ui.previewStatusLabel->setText(text);
    ui.previewStatusLabel->show();
    ui.previewRetryButton->setVisible(retry);
}

void StylePreviewPanel::updateBackground() {
    if (!m_layoutReady || m_updatingBackground)
        return;
    const QScopedValueRollback<bool> updating(m_updatingBackground, true);
    const QColor color = m_settings.background.isValid() ? m_settings.background
        : m_themeBackground.isValid() ? m_themeBackground : palette().color(QPalette::Window);
    const QString rgba = QStringLiteral("rgba(%1, %2, %3, %4)").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    const QString style = QStringLiteral("QFrame#previewCanvas {\n    background-color: %1;\n}").arg(rgba);
    if (ui.previewCanvas->styleSheet() != style)
        ui.previewCanvas->setStyleSheet(style);

    ui.previewBackgroundButton->setBackgroundNormal(color);
    ui.previewBackgroundButton->setBackgroundHover(color);
    ui.previewBackgroundButton->setBackgroundPressed(color);
    ui.previewBackgroundButton->setContentNormal(color.lightness() < 128 ? QColor(Qt::white) : QColor(Qt::black));
    ui.previewBackgroundButton->setText(m_settings.background.isValid() ? QString() : QStringLiteral("Theme"));
    ui.previewBackgroundButton->setToolTip(m_settings.background.isValid() ? color.name(QColor::HexArgb) : QStringLiteral("Editor background"));
}

void StylePreviewPanel::setThemeBackground(const QColor& color) {
    if (m_themeBackground == color)
        return;
    m_themeBackground = color;
    updateBackground();
    requestPreview();
}

void StylePreviewPanel::applySizeInput() {
    if (m_itemData.isEmpty())
        return;
    const bool widthValid = m_settings.widthAuto || ui.previewWidthEdit->hasAcceptableInput();
    const bool heightValid = m_settings.heightAuto || ui.previewHeightEdit->hasAcceptableInput();
    if (!widthValid || !heightValid) {
        updateSize();
        return;
    }
    QSize requested = m_settings.size;
    if (!m_settings.widthAuto)
        requested.setWidth(ui.previewWidthEdit->text().toInt());
    if (!m_settings.heightAuto)
        requested.setHeight(ui.previewHeightEdit->text().toInt());
    if (requested == m_settings.size) {
        updateSize();
        return;
    }
    m_settings.size = requested;
    requestPreview();
    emit settingsEdited(m_settings);
}

void StylePreviewPanel::on_previewWidthEdit_editingFinished() {
    applySizeInput();
}

void StylePreviewPanel::on_previewHeightEdit_editingFinished() {
    applySizeInput();
}

void StylePreviewPanel::applyAdaptiveSetting(bool width, bool enabled) {
    if (m_itemData.isEmpty())
        return;
    const QSize current = StylePreviewUtils::resolveSize(m_settings.size, m_suggestedSize, m_sizeHint,
                                                         m_settings.widthAuto, m_settings.heightAuto);
    const bool changed = width ? m_settings.widthAuto != enabled : m_settings.heightAuto != enabled;
    if (!changed)
        return;
    if (width) {
        m_settings.widthAuto = enabled;
        if (!enabled && m_settings.size.width() <= 0)
            m_settings.size.setWidth(current.width());
    } else {
        m_settings.heightAuto = enabled;
        if (!enabled && m_settings.size.height() <= 0)
            m_settings.size.setHeight(current.height());
    }
    updateSize();
    requestPreview();
    emit settingsEdited(m_settings);
}

void StylePreviewPanel::on_previewWidthAutoButton_toggled(bool checked) {
    applyAdaptiveSetting(true, checked);
}

void StylePreviewPanel::on_previewHeightAutoButton_toggled(bool checked) {
    applyAdaptiveSetting(false, checked);
}

void StylePreviewPanel::on_previewBackgroundButton_clicked() {
    if (m_itemData.isEmpty() || m_colorPopup)
        return;
    const QColor color = m_settings.background.isValid() ? m_settings.background : m_themeBackground;
    auto* popup = new ColorPickerDialog(color, window());
    m_colorPopup = popup;
    const quint64 generation = m_generation;
    connect(popup, &ColorPickerDialog::colorAccepted, this, [this, generation](const QColor& color) {
        if (m_itemData.isEmpty() || generation != m_generation || color == m_settings.background)
            return;
        m_settings.background = color;
        updateBackground();
        requestPreview();
        emit settingsEdited(m_settings);
    });
    emit dialogRequested(popup);
}

void StylePreviewPanel::on_resetPreviewButton_clicked() {
    if (m_itemData.isEmpty())
        return;
    m_settings = {};
    updateSize();
    updateBackground();
    requestPreview();
    emit settingsEdited(m_settings);
}

void StylePreviewPanel::on_previewRetryButton_clicked() {
    m_client->restart();
}

bool StylePreviewPanel::eventFilter(QObject* watched, QEvent* event) {
    if (watched == m_container && (event->type() == QEvent::Resize || event->type() == QEvent::Show)) {
        const QPointer<QWidget> container = m_container;
        m_client->setViewportSize(container->size());
        if (!container)
            return true;
    }
    return QWidget::eventFilter(watched, event);
}

void StylePreviewPanel::changeEvent(QEvent* event) {
    QWidget::changeEvent(event);
    if (event->type() == QEvent::PaletteChange || event->type() == QEvent::StyleChange)
        updateBackground();
}

void StylePreviewPanel::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);
    updateBackground();
}
