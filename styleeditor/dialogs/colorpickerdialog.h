#pragma once

#include "stylepopupdialog.h"
#include "ui_colorpickerdialog.h"

#include <QColor>
#include <QPointer>

class QLineEdit;
class QRegularExpressionValidator;
class ScreenColorPicker;

namespace animatedwidgets {
class PopupMenu;
}

class ColorPickerDialog : public StylePopupDialog {
    Q_OBJECT

public:
    explicit ColorPickerDialog(const QColor& baseColor, QWidget* parent = nullptr);

signals:
    void colorAccepted(const QColor& color);

private slots:
    void on_btn_close_clicked();
    void on_btn_confirm_clicked();
    void on_btn_cancel_clicked();
    void on_btn_automatic_clicked();
    void on_btn_screen_clicked();
    void on_sv_picker_colorChanged(const QColor& color);
    void on_h_picker_colorChanged(const QColor& color);
    void on_preset_picker_colorChanged(const QColor& color);
    void on_input_hex_textEdited(const QString& text);
    void on_input_r_textEdited(const QString& text);
    void on_input_g_textEdited(const QString& text);
    void on_input_b_textEdited(const QString& text);
    void on_input_h_textEdited(const QString& text);
    void on_input_s_textEdited(const QString& text);
    void on_input_v_textEdited(const QString& text);
    void on_alphaSlider_valueChanged(int value);
    void on_alphaValueEdit_textEdited(const QString& text);
    void on_alphaValueEdit_editingFinished();
    void on_alphaValueEdit_trailingIconClicked();
    void on_value_type_currentIndexChanged(int index);

private:
    enum class AlphaDisplayFormat { Decimal, Percentage, Hexadecimal };

    void createLayout();
    void updateColor(const QColor& color, QLineEdit* source = nullptr);
    void refreshValueInput(QLineEdit* source = nullptr);
    void updateRgbComponent(int component, const QString& text, QLineEdit* source);
    void updateHsvComponent(int component, const QString& text, QLineEdit* source);
    void refreshAcceptEnabled();
    void refreshAlphaDisplay(QLineEdit* source = nullptr);
    void updateAlphaValidator();

    Ui::ColorPickerDialog ui;

    QColor m_color;
    QColor m_acceptedColor;

    AlphaDisplayFormat m_alphaDisplayFormat = AlphaDisplayFormat::Decimal;
    QRegularExpressionValidator* m_alphaValidator = nullptr;
    QPointer<animatedwidgets::PopupMenu> m_alphaFormatPopup;
    ScreenColorPicker* m_screenPicker = nullptr;
    QPointer<QWidget> m_screenPickFocus;
};
