#pragma once

#include "animatedwidgets/core/valuetransition.h"

#include <QColor>
#include <QPointer>
#include <QWidget>

#include <functional>

class IconLineEdit;
class QHBoxLayout;

class CompositeLineEdit : public QWidget {
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QString placeholderText READ placeholderText WRITE setPlaceholderText)
    Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly)
    Q_PROPERTY(bool hasError READ hasError WRITE setHasError NOTIFY hasErrorChanged)
    Q_PROPERTY(QColor normalBorderColor READ normalBorderColor WRITE setNormalBorderColor)
    Q_PROPERTY(QColor hoverBorderColor READ hoverBorderColor WRITE setHoverBorderColor)
    Q_PROPERTY(QColor focusBorderColor READ focusBorderColor WRITE setFocusBorderColor)
    Q_PROPERTY(QColor errorBorderColor READ errorBorderColor WRITE setErrorBorderColor)
    Q_PROPERTY(QColor disabledBorderColor READ disabledBorderColor WRITE setDisabledBorderColor)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(QColor hoverBackgroundColor READ hoverBackgroundColor WRITE setHoverBackgroundColor)
    Q_PROPERTY(QColor errorBackgroundColor READ errorBackgroundColor WRITE setErrorBackgroundColor)
    Q_PROPERTY(QColor disabledBackgroundColor READ disabledBackgroundColor WRITE setDisabledBackgroundColor)
    Q_PROPERTY(int borderWidth READ borderWidth WRITE setBorderWidth)
    Q_PROPERTY(bool borderVisible READ isBorderVisible WRITE setBorderVisible)
    Q_PROPERTY(int cornerRadius READ cornerRadius WRITE setCornerRadius)
    Q_PROPERTY(int contentPadding READ contentPadding WRITE setContentPadding)
    Q_PROPERTY(bool drawInputLeftBorder READ drawInputLeftBorder WRITE setDrawInputLeftBorder)
    Q_PROPERTY(bool drawInputRightBorder READ drawInputRightBorder WRITE setDrawInputRightBorder)
    Q_PROPERTY(int spacing READ spacing WRITE setSpacing)
    Q_PROPERTY(int animationDuration READ animationDuration WRITE setAnimationDuration)

public:
    using WidgetFactory = std::function<QWidget*(QWidget* parent)>;

    explicit CompositeLineEdit(QWidget* parent = nullptr);

    IconLineEdit* editor() const;
    QWidget* leadingWidget() const;
    QWidget* trailingWidget() const;
    void setLeadingWidget(QWidget* widget);
    void setTrailingWidget(QWidget* widget);
    void setLeadingWidgetFactory(const WidgetFactory& factory);
    void setTrailingWidgetFactory(const WidgetFactory& factory);

    QString text() const;
    void setText(const QString& text);
    QString placeholderText() const;
    void setPlaceholderText(const QString& text);
    bool isReadOnly() const;
    void setReadOnly(bool readOnly);
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
    QColor backgroundColor() const;
    void setBackgroundColor(const QColor& color);
    QColor hoverBackgroundColor() const;
    void setHoverBackgroundColor(const QColor& color);
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
    int contentPadding() const;
    void setContentPadding(int padding);
    bool drawInputLeftBorder() const;
    void setDrawInputLeftBorder(bool draw);
    bool drawInputRightBorder() const;
    void setDrawInputRightBorder(bool draw);
    int spacing() const;
    void setSpacing(int spacing);
    int animationDuration() const;
    void setAnimationDuration(int duration);

    QSize sizeHint() const override;

signals:
    void textChanged(const QString& text);
    void hasErrorChanged(bool error);
    void currentBorderColorChanged(const QColor& color);

protected:
    bool event(QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    QColor targetBorderColor() const;
    QColor targetBackgroundColor() const;
    void refreshState(bool immediate = false);
    void updateInputBorders();

    QHBoxLayout* m_layout = nullptr;
    IconLineEdit* m_editor = nullptr;
    QWidget* m_leadingDivider = nullptr;
    QWidget* m_trailingDivider = nullptr;
    QPointer<QWidget> m_leadingWidget;
    QPointer<QWidget> m_trailingWidget;
    animatedwidgets::ValueTransition m_borderTransition;
    animatedwidgets::ValueTransition m_backgroundTransition;
    QColor m_normalBorderColor = QColor(QStringLiteral("#c8cdd5"));
    QColor m_hoverBorderColor = QColor(QStringLiteral("#8da3c0"));
    QColor m_focusBorderColor = QColor(QStringLiteral("#3478f6"));
    QColor m_errorBorderColor = QColor(QStringLiteral("#e5484d"));
    QColor m_disabledBorderColor = QColor(QStringLiteral("#d5dae0"));
    QColor m_backgroundColor = Qt::white;
    QColor m_hoverBackgroundColor;
    QColor m_errorBackgroundColor = QColor(QStringLiteral("#fff8f8"));
    QColor m_disabledBackgroundColor = QColor(QStringLiteral("#f3f5f7"));
    int m_borderWidth = 1;
    bool m_borderVisible = true;
    int m_cornerRadius = 6;
    int m_animationDuration = 300;
    bool m_drawInputLeftBorder = false;
    bool m_drawInputRightBorder = false;
    bool m_hovered = false;
};

namespace animatedwidgets {
using CompositeLineEdit = ::CompositeLineEdit;
} // namespace animatedwidgets
