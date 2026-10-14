#include "stylepreviewpanel.h"

#include "../dialogs/colorpickerdialog.h"
#include "../utils/stylepreviewutils.h"

#include "animatedwidgets/utils/smoothscrolling.h"

#include <QEvent>
#include <QIntValidator>
#include <QScopedValueRollback>
#include <QShowEvent>

StylePreviewPanel::StylePreviewPanel(QWidget* parent)
    : QWidget(parent)
{
    createLayout();
}

QWidget* StylePreviewPanel::widgetParent() const {
    return ui.previewHost;
}

void StylePreviewPanel::createLayout() {
    ui.setupUi(this);
    m_layoutReady = true;
    ui.previewWidthEdit->setValidator(new QIntValidator(1, 4096, ui.previewWidthEdit));
    ui.previewHeightEdit->setValidator(new QIntValidator(1, 4096, ui.previewHeightEdit));
    animatedwidgets::SmoothScrolling::install(ui.previewScrollArea);
    ui.previewScrollArea->viewport()->setAutoFillBackground(false);

    setPreview(nullptr);
}

void StylePreviewPanel::setPreview(QWidget* preview, const StylePreviewSettings& settings, const QSize& suggestedSize) {
    if (m_preview) {
        ui.previewWidgetLayout->removeWidget(m_preview.data());
        m_preview->hide();
    }

    ++m_generation;
    m_preview = preview;
    m_settings = settings;
    m_suggestedSize = suggestedSize;
    ui.previewControls->setEnabled(preview != nullptr);
    if (preview) {
        ui.previewWidgetLayout->addWidget(preview, 0, Qt::AlignCenter);
        preview->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        updateSize();
        preview->show();
    } else {
        ui.previewWidthEdit->clear();
        ui.previewHeightEdit->clear();
    }
    updateBackground();
}

void StylePreviewPanel::updateSize() {
    if (!m_preview)
        return;
    const QSize size = StylePreviewUtils::resolveSize(m_settings.size, m_suggestedSize, m_preview->sizeHint());
    m_preview->setFixedSize(size);
    ui.previewWidthEdit->setText(QString::number(size.width()));
    ui.previewHeightEdit->setText(QString::number(size.height()));
}

void StylePreviewPanel::updateBackground() {
    if (!m_layoutReady || m_updatingBackground)
        return;
    const QScopedValueRollback<bool> updating(m_updatingBackground, true);
    window()->ensurePolished();
    const QColor color = m_settings.background.isValid() ? m_settings.background : window()->palette().color(QPalette::Window);
    const QString rgba = QStringLiteral("rgba(%1, %2, %3, %4)").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    const QString style = ui.previewCanvas->property("backgroundStyleTemplate").toString().arg(rgba);
    if (ui.previewCanvas->styleSheet() != style)
        ui.previewCanvas->setStyleSheet(style);

    ui.previewBackgroundButton->setBackgroundNormal(color);
    ui.previewBackgroundButton->setBackgroundHover(color);
    ui.previewBackgroundButton->setBackgroundPressed(color);
    ui.previewBackgroundButton->setContentNormal(color.lightness() < 128 ? QColor(Qt::white) : QColor(Qt::black));
    ui.previewBackgroundButton->setText(m_settings.background.isValid() ? QString() : QStringLiteral("Theme"));
    ui.previewBackgroundButton->setToolTip(m_settings.background.isValid() ? color.name(QColor::HexArgb) : QStringLiteral("Editor background"));
}

void StylePreviewPanel::applySizeInput() {
    if (!m_preview)
        return;
    if (!ui.previewWidthEdit->hasAcceptableInput() || !ui.previewHeightEdit->hasAcceptableInput()) {
        updateSize();
        return;
    }

    const QSize size(ui.previewWidthEdit->text().toInt(), ui.previewHeightEdit->text().toInt());
    const QSize current = StylePreviewUtils::resolveSize(m_settings.size, m_suggestedSize, m_preview->sizeHint());
    if (size == current) {
        updateSize();
        return;
    }
    m_settings.size = size;
    updateSize();
    emit settingsEdited(m_settings);
}

void StylePreviewPanel::on_previewWidthEdit_editingFinished() {
    applySizeInput();
}

void StylePreviewPanel::on_previewHeightEdit_editingFinished() {
    applySizeInput();
}

void StylePreviewPanel::on_previewBackgroundButton_clicked() {
    if (!m_preview || m_colorPopup)
        return;

    const QColor color = m_settings.background.isValid() ? m_settings.background : window()->palette().color(QPalette::Window);
    auto* popup = new ColorPickerDialog(color, window());
    m_colorPopup = popup;
    const quint64 generation = m_generation;
    connect(popup, &ColorPickerDialog::colorAccepted, this, [this, generation](const QColor& color) {
        if (!m_preview || generation != m_generation || color == m_settings.background)
            return;
        m_settings.background = color;
        updateBackground();
        emit settingsEdited(m_settings);
    });
    emit dialogRequested(popup);
}

void StylePreviewPanel::on_resetPreviewButton_clicked() {
    if (!m_preview)
        return;
    m_settings = {};
    updateSize();
    updateBackground();
    emit settingsEdited(m_settings);
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
