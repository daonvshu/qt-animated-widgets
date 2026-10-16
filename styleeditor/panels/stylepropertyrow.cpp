#include "stylepropertyrow.h"

#include "../utils/stylepropertyeditorutils.h"
#include "../themes/stylethememanager.h"

#include <QFileInfo>

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
    } else if (m_property.type == animatedwidgets::styleeditor::StylePropertyType::Icon) {
        m_iconButton = new animatedwidgets::BorderedButton(ui.editorHost);
        m_iconButton->setObjectName(QStringLiteral("iconResourceButton"));
        m_iconButton->setFixedHeight(24);
        m_iconButton->setIconSize(QSize(16, 16));
        m_iconButton->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
        ui.editorLayout->addWidget(m_iconButton);
        setIconPreview(m_property.value.toString());
        connect(m_iconButton, &animatedwidgets::BorderedButton::clicked, this, [this] {
            emit iconRequested(m_property.value.toString());
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

    StyleThemeManager::prepareWidget(this);
}

void StylePropertyRow::setIconReference(const QString& reference, const QString& previewPath) {
    if (!isEnabled())
        return;
    m_property.value = reference;
    setIconPreview(previewPath);
    emit valueEdited(reference);
}

void StylePropertyRow::setIconPreview(const QString& previewPath) {
    if (!m_iconButton)
        return;
    const QString reference = m_property.value.toString();
    m_iconButton->setIcon(QIcon(previewPath));
    m_iconButton->setText(reference.isEmpty() ? QStringLiteral("Choose...") : QFileInfo(reference).fileName());
    m_iconButton->setToolTip(reference.isEmpty() ? QStringLiteral("Choose an icon from project resources") : reference);
}

void StylePropertyRow::setColor(const QColor& color) {
    if (!isEnabled())
        return;

    m_property.value = color;
    refreshColor();
    emit valueEdited(color);
}

void StylePropertyRow::setAutomaticColorBackground(const QColor& color) {
    m_automaticColorBackground = color;
    if (m_colorButton)
        refreshColor();
}

void StylePropertyRow::refreshColor() {
    const QColor color = m_property.value.value<QColor>();
    const QColor displayColor = color.isValid() ? color : m_automaticColorBackground;
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
