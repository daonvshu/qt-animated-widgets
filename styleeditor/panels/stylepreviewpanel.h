#pragma once

#include "../document/styleeditoritem.h"
#include "ui_stylepreviewpanel.h"

#include <QPointer>

class ColorPickerDialog;
class StylePopupDialog;
class StylePreviewClient;
class QWindow;

class StylePreviewPanel : public QWidget {
    Q_OBJECT

public:
    explicit StylePreviewPanel(QWidget* parent = nullptr);
    ~StylePreviewPanel() override;

    void setPreview(const StyleEditorItem* item, const StyleResourceSettings& resources = {}, int state = 0);

    QColor previewBackground() const;
    void setPreviewBackground(const QColor& color);

    void setThemeBackground(const QColor& color);

signals:
    void settingsEdited(const StylePreviewSettings& settings);
    void backgroundEdited(const QColor& color);
    void dialogRequested(StylePopupDialog* popup);
    void previewUpdated(const QString& itemId, const QList<animatedwidgets::styleeditor::StyleProperty>& properties, const QString& code);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
    void changeEvent(QEvent* event) override;
    void showEvent(QShowEvent* event) override;

private slots:
    void on_previewWidthEdit_editingFinished();
    void on_previewHeightEdit_editingFinished();
    void on_previewWidthAutoButton_toggled(bool checked);
    void on_previewHeightAutoButton_toggled(bool checked);
    void on_previewBackgroundButton_clicked();
    void on_resetPreviewButton_clicked();
    void on_previewRetryButton_clicked();

private:
    void createLayout();
    void updateSize();
    void updateBackground();
    void applySizeInput();
    void applyAdaptiveSetting(bool width, bool enabled);
    void requestPreview();
    void attachWindow(quintptr windowId);
    void detachWindow();
    void receivePreview(const QJsonObject& message);
    void showStatus(const QString& text, bool retry);

    Ui::StylePreviewPanel ui;

    StylePreviewClient* m_client = nullptr;
    QPointer<QWidget> m_container;
    QPointer<QWindow> m_foreignWindow;
    QPointer<ColorPickerDialog> m_colorPopup;
    StylePreviewSettings m_settings;
    QColor m_background;
    QColor m_themeBackground;
    QSize m_suggestedSize;
    QSize m_sizeHint;
    QJsonObject m_itemData;
    StyleResourceSettings m_resources;
    int m_state = 0;
    quint64 m_generation = 0;
    bool m_layoutReady = false;
    bool m_updatingBackground = false;
};
