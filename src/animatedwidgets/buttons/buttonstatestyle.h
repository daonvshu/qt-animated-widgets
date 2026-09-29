#pragma once

#include <QColor>
#include <QMetaType>

namespace animatedwidgets {

class ButtonStateStyle {
    Q_GADGET
    Q_PROPERTY(QColor background READ background WRITE setBackground)
    Q_PROPERTY(QColor border READ border WRITE setBorder)
    Q_PROPERTY(QColor content READ content WRITE setContent)

public:
    ButtonStateStyle() = default;
    ButtonStateStyle(const QColor& background, const QColor& border, const QColor& content)
        : m_background(background)
        , m_border(border)
        , m_content(content) {
    }

    QColor background() const {
        return m_background;
    }
    void setBackground(const QColor& color) {
        m_background = color;
    }

    QColor border() const {
        return m_border;
    }
    void setBorder(const QColor& color) {
        m_border = color;
    }

    QColor content() const {
        return m_content;
    }
    void setContent(const QColor& color) {
        m_content = color;
    }

    bool operator==(const ButtonStateStyle& other) const {
        return m_background == other.m_background && m_border == other.m_border && m_content == other.m_content;
    }

    bool operator!=(const ButtonStateStyle& other) const {
        return !(*this == other);
    }

private:
    QColor m_background;
    QColor m_border;
    QColor m_content;
};

} // namespace animatedwidgets

Q_DECLARE_METATYPE(animatedwidgets::ButtonStateStyle)
