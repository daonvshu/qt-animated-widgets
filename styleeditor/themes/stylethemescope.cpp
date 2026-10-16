#include "stylethemescope.h"

StyleThemeScope::StyleThemeScope(QWidget* parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground);
}

QColor StyleThemeScope::surface() const {
    return m_surface;
}

void StyleThemeScope::setSurface(const QColor& color) {
    m_surface = color;
}

QColor StyleThemeScope::border() const {
    return m_border;
}

void StyleThemeScope::setBorder(const QColor& color) {
    m_border = color;
}

QColor StyleThemeScope::text() const {
    return m_text;
}

void StyleThemeScope::setText(const QColor& color) {
    m_text = color;
}

QColor StyleThemeScope::mutedText() const {
    return m_mutedText;
}

void StyleThemeScope::setMutedText(const QColor& color) {
    m_mutedText = color;
}

QColor StyleThemeScope::disabledText() const {
    return m_disabledText;
}

void StyleThemeScope::setDisabledText(const QColor& color) {
    m_disabledText = color;
}

QColor StyleThemeScope::hover() const {
    return m_hover;
}

void StyleThemeScope::setHover(const QColor& color) {
    m_hover = color;
}

QColor StyleThemeScope::selection() const {
    return m_selection;
}

void StyleThemeScope::setSelection(const QColor& color) {
    m_selection = color;
}

QColor StyleThemeScope::iconSelection() const {
    return m_iconSelection;
}

void StyleThemeScope::setIconSelection(const QColor& color) {
    m_iconSelection = color;
}

QColor StyleThemeScope::canvas() const {
    return m_canvas;
}

void StyleThemeScope::setCanvas(const QColor& color) {
    m_canvas = color;
}

QColor StyleThemeScope::automatic() const {
    return m_automatic;
}

void StyleThemeScope::setAutomatic(const QColor& color) {
    m_automatic = color;
}
