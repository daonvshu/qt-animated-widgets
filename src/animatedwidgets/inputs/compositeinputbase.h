#pragma once

#include "animatedwidgets/core/valuetransition.h"

#include <QColor>
#include <QMargins>
#include <QPointer>
#include <QWidget>

class QBoxLayout;
class QHBoxLayout;
class QVBoxLayout;

class CompositeInputBase : public QWidget {
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
    Q_PROPERTY(int borderWidth READ borderWidth WRITE setBorderWidth)
    Q_PROPERTY(bool borderVisible READ isBorderVisible WRITE setBorderVisible)
    Q_PROPERTY(int cornerRadius READ cornerRadius WRITE setCornerRadius)
    Q_PROPERTY(bool drawInputLeftBorder READ drawInputLeftBorder WRITE setDrawInputLeftBorder)
    Q_PROPERTY(bool drawInputRightBorder READ drawInputRightBorder WRITE setDrawInputRightBorder)
    Q_PROPERTY(bool drawInputTopBorder READ drawInputTopBorder WRITE setDrawInputTopBorder)
    Q_PROPERTY(bool drawInputBottomBorder READ drawInputBottomBorder WRITE setDrawInputBottomBorder)
    Q_PROPERTY(int spacing READ spacing WRITE setSpacing)
    Q_PROPERTY(int animationDuration READ animationDuration WRITE setAnimationDuration)

public:
    explicit CompositeInputBase(QWidget* parent = nullptr);

    QWidget* topWidget() const;
    QWidget* bottomWidget() const;
    QWidget* leadingWidget() const;
    QWidget* trailingWidget() const;

    void setTopWidget(QWidget* widget);
    void setBottomWidget(QWidget* widget);
    void setLeadingWidget(QWidget* widget);
    void setTrailingWidget(QWidget* widget);

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
    QColor currentBorderColor() const;

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

    int borderWidth() const;
    void setBorderWidth(int width);
    bool isBorderVisible() const;
    void setBorderVisible(bool visible);
    int cornerRadius() const;
    void setCornerRadius(int radius);

    bool drawInputLeftBorder() const;
    void setDrawInputLeftBorder(bool draw);
    bool drawInputRightBorder() const;
    void setDrawInputRightBorder(bool draw);
    bool drawInputTopBorder() const;
    void setDrawInputTopBorder(bool draw);
    bool drawInputBottomBorder() const;
    void setDrawInputBottomBorder(bool draw);

    int spacing() const;
    void setSpacing(int spacing);
    int animationDuration() const;
    void setAnimationDuration(int duration);

    QSize sizeHint() const override;

signals:
    void hasErrorChanged(bool error);
    void currentBorderColorChanged(const QColor& color);

protected:
    void setEditorWidget(QWidget* editor);
    void setSlotMargins(const QMargins& top, const QMargins& bottom);
    void setHorizontalFrameInset(int extra);
    bool event(QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    enum class Slot { Top, Bottom, Leading, Trailing };

    QWidget* slotWidget(Slot slot) const;
    QWidget* slotHost(Slot slot) const;
    QBoxLayout* slotLayout(Slot slot) const;
    void setSlotWidget(Slot slot, QWidget* widget);

    QColor targetBorderColor() const;
    QColor targetBackgroundColor() const;
    void refreshState(bool immediate = false);
    void updateFrameMargins();
    void updateDividers();

    QVBoxLayout* m_layout = nullptr;
    QHBoxLayout* m_middleLayout = nullptr;
    QWidget* m_middleHost = nullptr;
    QWidget* m_topHost = nullptr;
    QWidget* m_bottomHost = nullptr;
    QWidget* m_leadingHost = nullptr;
    QWidget* m_trailingHost = nullptr;
    QWidget* m_editorHost = nullptr;
    QWidget* m_topDivider = nullptr;
    QWidget* m_bottomDivider = nullptr;
    QWidget* m_leadingDivider = nullptr;
    QWidget* m_trailingDivider = nullptr;
    QWidget* m_editorWidget = nullptr;

    QPointer<QWidget> m_topWidget;
    QPointer<QWidget> m_bottomWidget;
    QPointer<QWidget> m_leadingWidget;
    QPointer<QWidget> m_trailingWidget;

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

    int m_borderWidth = 1;
    int m_horizontalFrameInset = 0;
    int m_cornerRadius = 6;
    int m_animationDuration = 300;
    bool m_borderVisible = true;
    bool m_drawInputLeftBorder = false;
    bool m_drawInputRightBorder = false;
    bool m_drawInputTopBorder = false;
    bool m_drawInputBottomBorder = false;
    bool m_hasError = false;
    bool m_hovered = false;
};

namespace animatedwidgets {
using CompositeInputBase = ::CompositeInputBase;
} // namespace animatedwidgets
