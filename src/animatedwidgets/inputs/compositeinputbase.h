#pragma once

#include "animatedwidgets/namespacealias.h"

#include "animatedwidgets/core/valuetransition.h"

#include <QColor>
#include <QMargins>
#include <QPointer>
#include <QWidget>

class QBoxLayout;
class QHBoxLayout;
class QVBoxLayout;

namespace animatedwidgets {

class CompositeInputBase : public QWidget {
    Q_OBJECT
    Q_PROPERTY(bool hasError READ hasError WRITE setHasError NOTIFY hasErrorChanged)
    Q_PROPERTY(QColor borderNormal READ borderNormal WRITE setBorderNormal)
    Q_PROPERTY(QColor borderHover READ borderHover WRITE setBorderHover)
    Q_PROPERTY(QColor borderFocus READ borderFocus WRITE setBorderFocus)
    Q_PROPERTY(QColor borderError READ borderError WRITE setBorderError)
    Q_PROPERTY(QColor borderDisabled READ borderDisabled WRITE setBorderDisabled)
    Q_PROPERTY(QColor backgroundNormal READ backgroundNormal WRITE setBackgroundNormal)
    Q_PROPERTY(QColor backgroundHover READ backgroundHover WRITE setBackgroundHover)
    Q_PROPERTY(QColor backgroundFocus READ backgroundFocus WRITE setBackgroundFocus)
    Q_PROPERTY(QColor backgroundError READ backgroundError WRITE setBackgroundError)
    Q_PROPERTY(QColor backgroundDisabled READ backgroundDisabled WRITE setBackgroundDisabled)
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
    QColor currentBorderColor() const;

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

} // namespace animatedwidgets

#ifndef ANIMATEDWIDGETS_DESIGNER_PLUGIN
using CompositeInputBase = animatedwidgets::CompositeInputBase;
#endif
