#pragma once

#include "animatedwidgets/core/valuetransition.h"
#include "animatedwidgets/popups/popupmenu.h"

#include <QColor>
#include <QComboBox>
#include <QIcon>
#include <QLineEdit>
#include <QPointer>

namespace animatedwidgets {

class ComboListPopup;

class ComboBox : public QComboBox {
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
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor)
    Q_PROPERTY(QColor disabledTextColor READ disabledTextColor WRITE setDisabledTextColor)
    Q_PROPERTY(int borderWidth READ borderWidth WRITE setBorderWidth)
    Q_PROPERTY(bool borderVisible READ isBorderVisible WRITE setBorderVisible)
    Q_PROPERTY(int cornerRadius READ cornerRadius WRITE setCornerRadius)
    Q_PROPERTY(int contentPadding READ contentPadding WRITE setContentPadding)
    Q_PROPERTY(int animationDuration READ animationDuration WRITE setAnimationDuration)
    Q_PROPERTY(QIcon arrowIcon READ arrowIcon WRITE setArrowIcon)
    Q_PROPERTY(QSize arrowIconSize READ arrowIconSize WRITE setArrowIconSize)
    Q_PROPERTY(int arrowRightMargin READ arrowRightMargin WRITE setArrowRightMargin)
    Q_PROPERTY(bool arrowVisible READ arrowVisible WRITE setArrowVisible)
    Q_PROPERTY(int popupWidth READ popupWidth WRITE setPopupWidth)
    Q_PROPERTY(Qt::Alignment popupAlign READ popupAlign WRITE setPopupAlign)
    Q_PROPERTY(QString popupScrollBarStyleSheet READ popupScrollBarStyleSheet WRITE setPopupScrollBarStyleSheet)

public:
    explicit ComboBox(QWidget* parent = nullptr);

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
    int animationDuration() const;
    void setAnimationDuration(int duration);

    QIcon arrowIcon() const;
    void setArrowIcon(const QIcon& icon);
    QSize arrowIconSize() const;
    void setArrowIconSize(const QSize& size);
    int arrowRightMargin() const;
    void setArrowRightMargin(int margin);
    bool arrowVisible() const;
    void setArrowVisible(bool visible);
    int popupWidth() const;
    void setPopupWidth(int width);
    Qt::Alignment popupAlign() const;
    void setPopupAlign(Qt::Alignment align);

    QString popupScrollBarStyleSheet() const;
    void setPopupScrollBarStyleSheet(const QString& styleSheet);

    QSize sizeHint() const override;

signals:
    void hasErrorChanged(bool error);
    void popupAboutToShow(PopupMenu* popup);

protected:
    bool event(QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void showPopup() override;
    void hidePopup() override;

private:
    QColor targetBorderColor() const;
    QColor targetBackgroundColor() const;
    void refreshState(bool immediate = false);
    void syncEditor();
    void updateEditorPalette();
    void updateEditorMargins();
    QRect arrowButtonRect() const;

    ValueTransition m_borderTransition;
    ValueTransition m_backgroundTransition;
    QPointer<QLineEdit> m_editor;
    QPointer<ComboListPopup> m_popup;

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
    QIcon m_arrowIcon;
    QSize m_arrowIconSize = QSize(14, 14);

    QString m_popupScrollBarStyleSheet;

    int m_borderWidth = 1;
    int m_cornerRadius = 6;
    int m_contentPadding = 10;
    int m_animationDuration = 300;
    int m_arrowRightMargin = 10;
    int m_popupWidth = 0;
    Qt::Alignment m_popupAlign = Qt::AlignLeft;
    bool m_borderVisible = true;
    bool m_arrowVisible = true;
    bool m_hasError = false;
    bool m_hovered = false;
    bool m_popupOpen = false;
};

} // namespace animatedwidgets

#ifndef ANIMATEDWIDGETS_DESIGNER_PLUGIN
using ComboBox = animatedwidgets::ComboBox;
#endif
