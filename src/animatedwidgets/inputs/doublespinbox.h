#pragma once

#include "animatedwidgets/inputs/spinboxappearance.h"

#include <QDoubleSpinBox>
#include <QIcon>

class HoverIconButton;

class DoubleSpinBox : public QDoubleSpinBox, public SpinBoxAppearance {
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
    Q_PROPERTY(QSize stepButtonSize READ stepButtonSize WRITE setStepButtonSize)
    Q_PROPERTY(int stepButtonSpacing READ stepButtonSpacing WRITE setStepButtonSpacing)
    Q_PROPERTY(int stepButtonRightMargin READ stepButtonRightMargin WRITE setStepButtonRightMargin)
    Q_PROPERTY(QSize stepIconSize READ stepIconSize WRITE setStepIconSize)
    Q_PROPERTY(QIcon stepUpIcon READ stepUpIcon WRITE setStepUpIcon)
    Q_PROPERTY(QIcon stepDownIcon READ stepDownIcon WRITE setStepDownIcon)
    Q_PROPERTY(bool stepButtonsVisible READ stepButtonsVisible WRITE setStepButtonsVisible)

public:
    explicit DoubleSpinBox(QWidget* parent = nullptr);

    void setHasError(bool error);
    QSize stepButtonSize() const;
    void setStepButtonSize(const QSize& size);
    int stepButtonSpacing() const;
    void setStepButtonSpacing(int spacing);
    int stepButtonRightMargin() const;
    void setStepButtonRightMargin(int margin);
    QSize stepIconSize() const;
    void setStepIconSize(const QSize& size);
    QIcon stepUpIcon() const;
    void setStepUpIcon(const QIcon& icon);
    QIcon stepDownIcon() const;
    void setStepDownIcon(const QIcon& icon);
    bool stepButtonsVisible() const;
    void setStepButtonsVisible(bool visible);
    QSize sizeHint() const override;

signals:
    void hasErrorChanged(bool error);

protected:
    bool event(QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    void updateStepButtons();
    void updateStepButtonGeometry();

    HoverIconButton* m_upButton = nullptr;
    HoverIconButton* m_downButton = nullptr;
    QSize m_stepButtonSize = QSize(22, 15);
    int m_stepButtonSpacing = 0;
    int m_stepButtonRightMargin = 4;
    bool m_stepButtonsVisible = true;
};

namespace animatedwidgets {
using DoubleSpinBox = ::DoubleSpinBox;
} // namespace animatedwidgets
