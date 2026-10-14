#include "stylepropertyrow.h"

#include "../utils/stylepropertyeditorutils.h"

StylePropertyRow::StylePropertyRow(const animatedwidgets::styleeditor::StyleProperty& property, QWidget* parent)
    : QWidget(parent)
    , m_property(property)
{
    createLayout();
}

void StylePropertyRow::createLayout() {
    ui.setupUi(this);
    ui.nameLabel->setText(m_property.label.isEmpty() ? m_property.name : m_property.label);
    ui.nameLabel->setToolTip(m_property.description);
    setToolTip(m_property.description);

    if (m_property.type == animatedwidgets::styleeditor::StylePropertyType::Color) {
        m_colorButton = new animatedwidgets::BorderedButton(ui.editorHost);
        m_colorButton->setObjectName(QStringLiteral("colorSwatch"));
        m_colorButton->setFixedSize(44, 24);
        ui.editorLayout->addWidget(m_colorButton);
        ui.editorLayout->addStretch();
        refreshColor();

        connect(m_colorButton, &animatedwidgets::BorderedButton::clicked, this, [this] {
            emit colorRequested(m_property.value.value<QColor>());
        });
    } else {
        QWidget* editor = StylePropertyEditorUtils::createEditor(m_property, ui.editorHost, [this](const QVariant& value) {
            m_property.value = value;
            emit valueEdited(value);
        });
        const int editorHeight = m_property.type == animatedwidgets::styleeditor::StylePropertyType::Alignment ? editor->sizeHint().height() : 24;
        editor->setFixedHeight(editorHeight);
        ui.editorLayout->addWidget(editor);
        if (m_property.type == animatedwidgets::styleeditor::StylePropertyType::Boolean)
            ui.editorLayout->addStretch();
    }
}

void StylePropertyRow::setColor(const QColor& color) {
    if (!isEnabled())
        return;

    m_property.value = color;
    refreshColor();
    emit valueEdited(color);
}

void StylePropertyRow::refreshColor() {
    const QColor color = m_property.value.value<QColor>();
    const QColor displayColor = color.isValid() ? color : QColor(QStringLiteral("#f2f3f5"));
    m_colorButton->setBackgroundNormal(displayColor);
    m_colorButton->setBackgroundHover(displayColor);
    m_colorButton->setBackgroundPressed(displayColor);
    m_colorButton->setText(color.isValid() ? QString() : QStringLiteral("Auto"));
    const QString description = color.isValid() ? color.name(QColor::HexArgb) : QStringLiteral("Automatic derived color");
    m_colorButton->setToolTip(description);
    m_colorButton->setAccessibleName(ui.nameLabel->text() + QStringLiteral(": ") + description);
}

void StylePropertyRow::on_resetButton_clicked() {
    emit resetRequested();
}
