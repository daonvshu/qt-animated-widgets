#pragma once

#include "animatedwidgets/core/valuetransition.h"

#include <QColor>
#include <QLineEdit>

class LineEdit : public QLineEdit {
    Q_OBJECT
    Q_PROPERTY(bool hasError READ hasError WRITE setHasError NOTIFY hasErrorChanged)
    Q_PROPERTY(QColor normalBorderColor READ normalBorderColor WRITE setNormalBorderColor)
    Q_PROPERTY(QColor hoverBorderColor READ hoverBorderColor WRITE setHoverBorderColor)
    Q_PROPERTY(QColor focusBorderColor READ focusBorderColor WRITE setFocusBorderColor)
    Q_PROPERTY(QColor errorBorderColor READ errorBorderColor WRITE setErrorBorderColor)
    Q_PROPERTY(QColor disabledBorderColor READ disabledBorderColor WRITE setDisabledBorderColor)
    Q_PROPERTY(QColor normalBackgroundColor READ normalBackgroundColor WRITE setNormalBackgroundColor)
    Q_PROPERTY(QColor hoverBackgroundColor READ hoverBackgroundColor WRITE setHoverBackgroundColor)
    Q_PROPERTY(QColor focusBackgroundColor READ focusBackgroundColor WRITE setFocusBackgroundColor)
    Q_PROPERTY(QColor errorBackgroundColor READ errorBackgroundColor WRITE setErrorBackgroundColor)
    Q_PROPERTY(QColor disabledBackgroundColor READ disabledBackgroundColor WRITE setDisabledBackgroundColor)
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor)
    Q_PROPERTY(QColor disabledTextColor READ disabledTextColor WRITE setDisabledTextColor)
    Q_PROPERTY(int borderWidth READ borderWidth WRITE setBorderWidth)
    Q_PROPERTY(bool borderVisible READ isBorderVisible WRITE setBorderVisible)
    Q_PROPERTY(int cornerRadius READ cornerRadius WRITE setCornerRadius)
    Q_PROPERTY(int contentPadding READ contentPadding WRITE setContentPadding)
    Q_PROPERTY(int animationDuration READ animationDuration WRITE setAnimationDuration)

public:
    explicit LineEdit(QWidget* parent = nullptr);

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
    int animationDuration() const;
    virtual void setAnimationDuration(int duration);

    QSize sizeHint() const override;

signals:
    void hasErrorChanged(bool error);

protected:
    bool event(QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void refreshInputState(bool immediate = false);
    virtual void updateContentMargins();

private:
    QColor targetBorderColor() const;
    QColor targetBackgroundColor() const;
    void updateTextPalette();

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
    int m_animationDuration = 300;
    bool m_hasError = false;
    bool m_hovered = false;
};

namespace animatedwidgets {
using LineEdit = ::LineEdit;
} // namespace animatedwidgets
