#pragma once

#include "../document/stylepreviewsettings.h"
#include "ui_stylepreviewpanel.h"

#include <QPointer>

class ColorPickerDialog;
class StylePopupDialog;

class StylePreviewPanel : public QWidget {
    Q_OBJECT

public:
    explicit StylePreviewPanel(QWidget* parent = nullptr);

    QWidget* widgetParent() const;
    void setPreview(QWidget* preview, const StylePreviewSettings& settings = {}, const QSize& suggestedSize = {});

signals:
    void settingsEdited(const StylePreviewSettings& settings);
    void dialogRequested(StylePopupDialog* popup);

protected:
    void changeEvent(QEvent* event) override;
    void showEvent(QShowEvent* event) override;

private slots:
    void on_previewWidthEdit_editingFinished();
    void on_previewHeightEdit_editingFinished();
    void on_previewBackgroundButton_clicked();
    void on_resetPreviewButton_clicked();

private:
    void createLayout();
    void updateSize();
    void updateBackground();
    void applySizeInput();

    Ui::StylePreviewPanel ui;

    QPointer<QWidget> m_preview;
    QPointer<ColorPickerDialog> m_colorPopup;
    StylePreviewSettings m_settings;
    QSize m_suggestedSize;
    quint64 m_generation = 0;
    bool m_layoutReady = false;
    bool m_updatingBackground = false;
};
