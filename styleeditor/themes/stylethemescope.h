#pragma once

#include <QColor>
#include <QWidget>

class StyleThemeScope : public QWidget {
    Q_OBJECT
    Q_PROPERTY(QColor surface READ surface WRITE setSurface)
    Q_PROPERTY(QColor border READ border WRITE setBorder)
    Q_PROPERTY(QColor text READ text WRITE setText)
    Q_PROPERTY(QColor mutedText READ mutedText WRITE setMutedText)
    Q_PROPERTY(QColor disabledText READ disabledText WRITE setDisabledText)
    Q_PROPERTY(QColor hover READ hover WRITE setHover)
    Q_PROPERTY(QColor selection READ selection WRITE setSelection)
    Q_PROPERTY(QColor iconSelection READ iconSelection WRITE setIconSelection)
    Q_PROPERTY(QColor canvas READ canvas WRITE setCanvas)
    Q_PROPERTY(QColor automatic READ automatic WRITE setAutomatic)

public:
    explicit StyleThemeScope(QWidget* parent = nullptr);

    QColor surface() const;
    void setSurface(const QColor& color);
    QColor border() const;
    void setBorder(const QColor& color);
    QColor text() const;
    void setText(const QColor& color);
    QColor mutedText() const;
    void setMutedText(const QColor& color);
    QColor disabledText() const;
    void setDisabledText(const QColor& color);

    QColor hover() const;
    void setHover(const QColor& color);
    QColor selection() const;
    void setSelection(const QColor& color);
    QColor iconSelection() const;
    void setIconSelection(const QColor& color);

    QColor canvas() const;
    void setCanvas(const QColor& color);
    QColor automatic() const;
    void setAutomatic(const QColor& color);

private:
    QColor m_surface;
    QColor m_border;
    QColor m_text;
    QColor m_mutedText;
    QColor m_disabledText;

    QColor m_hover;
    QColor m_selection;
    QColor m_iconSelection;

    QColor m_canvas;
    QColor m_automatic;
};
