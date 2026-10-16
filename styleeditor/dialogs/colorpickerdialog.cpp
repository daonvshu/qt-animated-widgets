#include "colorpickerdialog.h"

#include "../themes/stylethememanager.h"

#include "animatedwidgets/popups/defaultpopupmenumodel.h"
#include "animatedwidgets/popups/popupmenu.h"

#include <QIntValidator>
#include <QRegularExpressionValidator>
#include <QSignalBlocker>

ColorPickerDialog::ColorPickerDialog(const QColor& baseColor, QWidget* parent)
    : StylePopupDialog(parent)
    , m_color(baseColor.isValid() ? baseColor : QColor(Qt::red))
{
    createLayout();
    updateColor(m_color);

    connect(this, &StylePopupDialog::finished, this, [this](int result) {
        if (result == 1)
            emit colorAccepted(m_acceptedColor);
    });
}

void ColorPickerDialog::createLayout() {
    ui.setupUi(this);
    setWindowTitle(QStringLiteral("Select color"));
    setFocusProxy(ui.input_hex);

    ui.input_hex->setValidator(new QRegularExpressionValidator(QRegularExpression(QStringLiteral("#[0-9A-Fa-f]{6}([0-9A-Fa-f]{2})?")), ui.input_hex));
    for (QLineEdit* input : {ui.input_r, ui.input_g, ui.input_b, ui.input_s, ui.input_v})
        input->setValidator(new QIntValidator(0, 255, input));
    ui.input_h->setValidator(new QIntValidator(0, 360, ui.input_h));

    m_alphaValidator = new QRegularExpressionValidator(ui.alphaValueEdit);
    ui.alphaValueEdit->setValidator(m_alphaValidator);
    updateAlphaValidator();

    for (QLineEdit* input : {ui.input_hex, ui.input_r, ui.input_g, ui.input_b, ui.input_h, ui.input_s, ui.input_v}) {
        connect(input, &QLineEdit::editingFinished, this, [this] { refreshValueInput(); });
        connect(input, &QLineEdit::textChanged, this, [this] { refreshAcceptEnabled(); });
    }
    connect(ui.alphaValueEdit, &QLineEdit::textChanged, this, [this] { refreshAcceptEnabled(); });
}

void ColorPickerDialog::updateColor(const QColor& color, QLineEdit* source) {
    m_color = color;
    ui.sv_picker->setBaseColor(color);
    ui.h_picker->setBaseColor(color);
    ui.preset_picker->setBaseColor(color);
    refreshValueInput(source);
}

void ColorPickerDialog::refreshValueInput(QLineEdit* source) {
    const QList<QPair<QLineEdit*, QString>> values{
        {ui.input_hex, m_color.name()},
        {ui.input_r, QString::number(m_color.red())},
        {ui.input_g, QString::number(m_color.green())},
        {ui.input_b, QString::number(m_color.blue())},
        {ui.input_h, QString::number(qMax(0, m_color.hsvHue()))},
        {ui.input_s, QString::number(m_color.hsvSaturation())},
        {ui.input_v, QString::number(m_color.value())}};
    for (const auto& entry : values) {
        if (entry.first == source)
            continue;
        const QSignalBlocker blocker(entry.first);
        entry.first->setText(entry.second);
    }

    const QSignalBlocker blocker(ui.alphaSlider);
    ui.alphaSlider->setValue(m_color.alpha());
    refreshAlphaDisplay(source);
    refreshAcceptEnabled();
}

void ColorPickerDialog::refreshAlphaDisplay(QLineEdit* source) {
    if (source == ui.alphaValueEdit)
        return;

    QString text;
    switch (m_alphaDisplayFormat) {
        case AlphaDisplayFormat::Decimal:
            text = QString::number(m_color.alpha());
            break;
        case AlphaDisplayFormat::Percentage:
            text = QString::number(m_color.alpha() * 100.0 / 255.0, 'f', 0) + QLatin1Char('%');
            break;
        case AlphaDisplayFormat::Hexadecimal:
            text = QStringLiteral("0x") + QString::number(m_color.alpha(), 16).rightJustified(2, QLatin1Char('0')).toUpper();
            break;
    }
    const QSignalBlocker blocker(ui.alphaValueEdit);
    ui.alphaValueEdit->setText(text);
}

void ColorPickerDialog::updateAlphaValidator() {
    QString pattern;
    switch (m_alphaDisplayFormat) {
        case AlphaDisplayFormat::Decimal:
            pattern = QStringLiteral("(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[0-9]{1,2})");
            break;
        case AlphaDisplayFormat::Percentage:
            pattern = QStringLiteral("(100|[0-9]{1,2})%?");
            break;
        case AlphaDisplayFormat::Hexadecimal:
            pattern = QStringLiteral("(0[xX])?[0-9A-Fa-f]{1,2}");
            break;
    }
    m_alphaValidator->setRegularExpression(QRegularExpression(pattern));
}

void ColorPickerDialog::refreshAcceptEnabled() {
    bool valid = false;
    if (ui.value_type->currentIndex() == 0)
        valid = ui.input_hex->hasAcceptableInput();
    else if (ui.value_type->currentIndex() == 1)
        valid = ui.input_r->hasAcceptableInput() && ui.input_g->hasAcceptableInput() && ui.input_b->hasAcceptableInput();
    else
        valid = ui.input_h->hasAcceptableInput() && ui.input_s->hasAcceptableInput() && ui.input_v->hasAcceptableInput();
    ui.btn_confirm->setEnabled(valid && ui.alphaValueEdit->hasAcceptableInput());
}

void ColorPickerDialog::on_btn_close_clicked() {
    finish(0);
}

void ColorPickerDialog::on_btn_cancel_clicked() {
    finish(0);
}

void ColorPickerDialog::on_btn_confirm_clicked() {
    if (!ui.btn_confirm->isEnabled())
        return;
    m_acceptedColor = m_color;
    finish(1);
}

void ColorPickerDialog::on_btn_automatic_clicked() {
    m_acceptedColor = QColor();
    finish(1);
}

void ColorPickerDialog::on_sv_picker_colorChanged(const QColor& color) {
    updateColor(color);
}

void ColorPickerDialog::on_h_picker_colorChanged(const QColor& color) {
    updateColor(color);
}

void ColorPickerDialog::on_preset_picker_colorChanged(const QColor& color) {
    updateColor(color);
}

void ColorPickerDialog::on_input_hex_textEdited(const QString& text) {
    const QColor color(text);
    if (!ui.input_hex->hasAcceptableInput() || !color.isValid())
        return;
    QColor selected = color;
    if (text.size() == 7)
        selected.setAlpha(m_color.alpha());
    updateColor(selected, ui.input_hex);
}

void ColorPickerDialog::updateRgbComponent(int component, const QString& text, QLineEdit* source) {
    if (!source->hasAcceptableInput())
        return;
    QColor color = m_color;
    const int value = text.toInt();
    if (component == 0)
        color.setRed(value);
    else if (component == 1)
        color.setGreen(value);
    else
        color.setBlue(value);
    updateColor(color, source);
}

void ColorPickerDialog::updateHsvComponent(int component, const QString& text, QLineEdit* source) {
    if (!source->hasAcceptableInput())
        return;
    int hue = qMax(0, m_color.hsvHue());
    int saturation = m_color.hsvSaturation();
    int value = m_color.value();
    if (component == 0)
        hue = text.toInt() % 360;
    else if (component == 1)
        saturation = text.toInt();
    else
        value = text.toInt();
    updateColor(QColor::fromHsv(hue, saturation, value, m_color.alpha()), source);
}

void ColorPickerDialog::on_input_r_textEdited(const QString& text) {
    updateRgbComponent(0, text, ui.input_r);
}

void ColorPickerDialog::on_input_g_textEdited(const QString& text) {
    updateRgbComponent(1, text, ui.input_g);
}

void ColorPickerDialog::on_input_b_textEdited(const QString& text) {
    updateRgbComponent(2, text, ui.input_b);
}

void ColorPickerDialog::on_input_h_textEdited(const QString& text) {
    updateHsvComponent(0, text, ui.input_h);
}

void ColorPickerDialog::on_input_s_textEdited(const QString& text) {
    updateHsvComponent(1, text, ui.input_s);
}

void ColorPickerDialog::on_input_v_textEdited(const QString& text) {
    updateHsvComponent(2, text, ui.input_v);
}

void ColorPickerDialog::on_alphaSlider_valueChanged(int value) {
    QColor color = m_color;
    color.setAlpha(value);
    updateColor(color);
}

void ColorPickerDialog::on_alphaValueEdit_textEdited(const QString& text) {
    if (!ui.alphaValueEdit->hasAcceptableInput())
        return;

    QString number = text;
    int base = 10;
    if (m_alphaDisplayFormat == AlphaDisplayFormat::Percentage && number.endsWith(QLatin1Char('%')))
        number.chop(1);
    if (m_alphaDisplayFormat == AlphaDisplayFormat::Hexadecimal) {
        base = 16;
        if (number.startsWith(QStringLiteral("0x"), Qt::CaseInsensitive))
            number.remove(0, 2);
    }

    bool valid = false;
    int alpha = number.toInt(&valid, base);
    if (!valid)
        return;
    if (m_alphaDisplayFormat == AlphaDisplayFormat::Percentage)
        alpha = qRound(alpha * 255.0 / 100.0);

    QColor color = m_color;
    color.setAlpha(alpha);
    updateColor(color, ui.alphaValueEdit);
}

void ColorPickerDialog::on_alphaValueEdit_editingFinished() {
    refreshAlphaDisplay();
    refreshAcceptEnabled();
}

void ColorPickerDialog::on_alphaValueEdit_trailingIconClicked() {
    if (m_alphaFormatPopup) {
        m_alphaFormatPopup->close();
        return;
    }

    auto* popup = new animatedwidgets::PopupMenu(this);
    m_alphaFormatPopup = popup;
    popup->setStrings({QStringLiteral("Decimal (0-255)"), QStringLiteral("Percentage"), QStringLiteral("Hexadecimal")});
    popup->defaultModel()->setCurrentIndex(static_cast<int>(m_alphaDisplayFormat));
    StyleThemeManager::preparePopup(popup);
    popup->setAdaptiveHeight(180);
    popup->moveToTargetBottom(ui.alphaValueEdit, Qt::AlignRight);

    connect(popup, &animatedwidgets::PopupMenu::itemTriggered, this, [this](const QModelIndex& index) {
        if (index.row() < 0 || index.row() > static_cast<int>(AlphaDisplayFormat::Hexadecimal))
            return;
        m_alphaDisplayFormat = static_cast<AlphaDisplayFormat>(index.row());
        updateAlphaValidator();
        refreshAlphaDisplay();
        refreshAcceptEnabled();
    });
    popup->show();
}

void ColorPickerDialog::on_value_type_currentIndexChanged(int index) {
    ui.stackedWidget->setCurrentIndex(index);
    setFocusProxy(index == 0 ? ui.input_hex : index == 1 ? ui.input_r : ui.input_h);
    refreshValueInput();
}
