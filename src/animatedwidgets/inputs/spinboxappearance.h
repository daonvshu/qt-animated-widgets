#pragma once

#include "animatedwidgets/core/valuetransition.h"

#include <QColor>

class QAbstractSpinBox;
class QEvent;
class QLineEdit;
class QPainter;

namespace animatedwidgets {

class SpinBoxAppearance {
public:
    explicit SpinBoxAppearance(QAbstractSpinBox* owner);

    void setEditor(QLineEdit* editor);
    void handleEvent(QEvent* event);
    void paint(QPainter& painter) const;

    bool hasError() const;
    void setHasError(bool error);

    QColor borderNormal() const;
    void setBorderNormal(const QColor& color);
    QColor borderHover() const;
    void setBorderHover(const QColor& color);
    QColor borderFocus() const;
    void setBorderFocus(const QColor& color);
    QColor borderError() const;
    void setBorderError(const QColor& color);
    QColor borderDisabled() const;
    void setBorderDisabled(const QColor& color);

    QColor backgroundNormal() const;
    void setBackgroundNormal(const QColor& color);
    QColor backgroundHover() const;
    void setBackgroundHover(const QColor& color);
    QColor backgroundFocus() const;
    void setBackgroundFocus(const QColor& color);
    QColor backgroundError() const;
    void setBackgroundError(const QColor& color);
    QColor backgroundDisabled() const;
    void setBackgroundDisabled(const QColor& color);

    QColor textColor() const;
    void setTextColor(const QColor& color);
    QColor disabledTextColor() const;
    void setDisabledTextColor(const QColor& color);

    int borderWidth() const;
    void setBorderWidth(int width);
    bool isBorderVisible() const;
    void setBorderVisible(bool visible);
    int cornerRadius() const;
    void setCornerRadius(int radius);
    int contentPadding() const;
    void setContentPadding(int padding);
    void setTrailingControlWidth(int width);
    int animationDuration() const;
    void setAnimationDuration(int duration);

private:
    QColor targetBorderColor() const;
    QColor targetBackgroundColor() const;
    void refresh(bool immediate = false);
    void updateEditorPalette();
    void updateEditorMargins();

    QAbstractSpinBox* m_owner = nullptr;
    QLineEdit* m_editor = nullptr;
    ValueTransition m_borderTransition;
    ValueTransition m_backgroundTransition;

    QColor m_normalBorderColor = QColor(QStringLiteral("#c8cdd5"));
    QColor m_hoverBorderColor = QColor(QStringLiteral("#8da3c0"));
    QColor m_focusBorderColor = QColor(QStringLiteral("#3478f6"));
    QColor m_errorBorderColor = QColor(QStringLiteral("#e5484d"));
    QColor m_disabledBorderColor = QColor(QStringLiteral("#d5dae0"));

    QColor m_normalBackgroundColor = QColor(Qt::white);
    QColor m_hoverBackgroundColor;
    QColor m_focusBackgroundColor = QColor(Qt::white);
    QColor m_errorBackgroundColor = QColor(QStringLiteral("#fff8f8"));
    QColor m_disabledBackgroundColor = QColor(QStringLiteral("#f3f5f7"));

    QColor m_textColor = QColor(QStringLiteral("#20242a"));
    QColor m_disabledTextColor = QColor(QStringLiteral("#9298a2"));

    int m_borderWidth = 1;
    bool m_borderVisible = true;
    int m_cornerRadius = 6;
    int m_contentPadding = 10;
    int m_trailingControlWidth = 34;
    int m_animationDuration = 300;
    bool m_hasError = false;
    bool m_hovered = false;
};

} // namespace animatedwidgets
