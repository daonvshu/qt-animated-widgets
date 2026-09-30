#pragma once

#include "animatedwidgets/core/valuetransition.h"

#include <QColor>

class QAbstractSpinBox;
class QEvent;
class QLineEdit;
class QPainter;

class SpinBoxAppearance {
public:
    explicit SpinBoxAppearance(QAbstractSpinBox* owner);

    void setEditor(QLineEdit* editor);
    void handleEvent(QEvent* event);
    void paint(QPainter& painter) const;

    bool hasError() const;
    void setHasError(bool error);

    QColor normalBorderColor() const;
    void setNormalBorderColor(const QColor& color);
    QColor hoverBorderColor() const;
    void setHoverBorderColor(const QColor& color);
    QColor focusBorderColor() const;
    void setFocusBorderColor(const QColor& color);
    QColor errorBorderColor() const;
    void setErrorBorderColor(const QColor& color);
    QColor disabledBorderColor() const;
    void setDisabledBorderColor(const QColor& color);

    QColor normalBackgroundColor() const;
    void setNormalBackgroundColor(const QColor& color);
    QColor hoverBackgroundColor() const;
    void setHoverBackgroundColor(const QColor& color);
    QColor focusBackgroundColor() const;
    void setFocusBackgroundColor(const QColor& color);
    QColor errorBackgroundColor() const;
    void setErrorBackgroundColor(const QColor& color);
    QColor disabledBackgroundColor() const;
    void setDisabledBackgroundColor(const QColor& color);

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
    animatedwidgets::ValueTransition m_borderTransition;
    animatedwidgets::ValueTransition m_backgroundTransition;

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
