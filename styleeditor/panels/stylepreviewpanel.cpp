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

    QWidget* const controls[] = {ui.previewWidthEdit, ui.previewHeightEdit,
                                ui.previewWidthAutoButton, ui.previewHeightAutoButton,
                                ui.previewBackgroundButton, ui.resetPreviewButton};
    for (QWidget* control : controls)
        control->setEnabled(preview != nullptr);

    if (preview) {
        ui.previewWidgetLayout->addWidget(preview, 0, Qt::AlignCenter);
        updateSize();
        preview->show();
    } else {
        ui.previewWidthEdit->clear();
        ui.previewHeightEdit->clear();
        ui.previewWidthAutoButton->setChecked(false);
        ui.previewHeightAutoButton->setChecked(false);
    }
    updateBackground();
}

void StylePreviewPanel::updateSize() {
    if (!m_preview)
        return;
    const QSize size = StylePreviewUtils::resolveSize(m_settings.size, m_suggestedSize, m_preview->sizeHint(),
                                                      m_settings.widthAuto, m_settings.heightAuto);
    m_preview->setMinimumSize(QSize(0, 0));
    m_preview->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    m_preview->setSizePolicy(m_settings.widthAuto ? QSizePolicy::Preferred : QSizePolicy::Fixed,
                             m_settings.heightAuto ? QSizePolicy::Preferred : QSizePolicy::Fixed);
    m_preview->resize(size);
    if (!m_settings.widthAuto)
        m_preview->setFixedWidth(size.width());
    if (!m_settings.heightAuto)
        m_preview->setFixedHeight(size.height());
    m_preview->updateGeometry();
    ui.previewWidthEdit->setText(QString::number(size.width()));
    ui.previewHeightEdit->setText(QString::number(size.height()));
    ui.previewWidthAutoButton->setChecked(m_settings.widthAuto);
    ui.previewHeightAutoButton->setChecked(m_settings.heightAuto);
    ui.previewWidthEdit->setEnabled(!m_settings.widthAuto);
    ui.previewHeightEdit->setEnabled(!m_settings.heightAuto);
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
    updateSize();
    emit settingsEdited(m_settings);
}

void StylePreviewPanel::on_previewWidthEdit_editingFinished() {
    applySizeInput();
}

void StylePreviewPanel::on_previewHeightEdit_editingFinished() {
    applySizeInput();
}

void StylePreviewPanel::applyAdaptiveSetting(bool width, bool enabled) {
    if (!m_preview)
        return;

    const QSize current = StylePreviewUtils::resolveSize(m_settings.size, m_suggestedSize, m_preview->sizeHint(),
                                                         m_settings.widthAuto, m_settings.heightAuto);
    bool changed = false;
    if (width) {
        changed = m_settings.widthAuto != enabled;
        m_settings.widthAuto = enabled;
        if (!enabled && m_settings.size.width() <= 0)
            m_settings.size.setWidth(current.width());
    } else {
        changed = m_settings.heightAuto != enabled;
        m_settings.heightAuto = enabled;
        if (!enabled && m_settings.size.height() <= 0)
            m_settings.size.setHeight(current.height());
    }

    updateSize();
    if (changed)
        emit settingsEdited(m_settings);
}

void StylePreviewPanel::on_previewWidthAutoButton_toggled(bool checked) {
    applyAdaptiveSetting(true, checked);
}

void StylePreviewPanel::on_previewHeightAutoButton_toggled(bool checked) {
    applyAdaptiveSetting(false, checked);
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
