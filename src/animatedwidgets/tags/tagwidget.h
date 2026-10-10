#pragma once

#include "animatedwidgets/tags/tagstyle.h"

#include <QWidget>

namespace animatedwidgets {

class HoverIconButton;

class TagWidget : public QWidget {
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText)
    Q_PROPERTY(QIcon icon READ icon WRITE setIcon)
    Q_PROPERTY(QColor background READ background WRITE setBackground)
    Q_PROPERTY(QColor border READ border WRITE setBorder)
    Q_PROPERTY(QColor content READ content WRITE setContent)
    Q_PROPERTY(QColor disabledContent READ disabledContent WRITE setDisabledContent)
    Q_PROPERTY(int borderWidth READ borderWidth WRITE setBorderWidth)
    Q_PROPERTY(int cornerRadius READ cornerRadius WRITE setCornerRadius)
    Q_PROPERTY(int contentPadding READ contentPadding WRITE setContentPadding)
    Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize)
    Q_PROPERTY(int iconSpacing READ iconSpacing WRITE setIconSpacing)
    Q_PROPERTY(QIcon closeIcon READ closeIcon WRITE setCloseIcon)
    Q_PROPERTY(QSize closeIconSize READ closeIconSize WRITE setCloseIconSize)
    Q_PROPERTY(int closeButtonSize READ closeButtonSize WRITE setCloseButtonSize)
    Q_PROPERTY(bool closeButtonVisible READ closeButtonVisible WRITE setCloseButtonVisible)

public:
    explicit TagWidget(QWidget* parent = nullptr);

    QString text() const;
    void setText(const QString& text);
    QIcon icon() const;
    void setIcon(const QIcon& icon);

    TagStyle tagStyle() const;
    void setTagStyle(const TagStyle& style);
    QColor background() const;
    void setBackground(const QColor& color);
    QColor border() const;
    void setBorder(const QColor& color);
    QColor content() const;
    void setContent(const QColor& color);
    QColor disabledContent() const;
    void setDisabledContent(const QColor& color);

    int borderWidth() const;
    void setBorderWidth(int width);
    int cornerRadius() const;
    void setCornerRadius(int radius);
    int contentPadding() const;
    void setContentPadding(int padding);
    QSize iconSize() const;
    void setIconSize(const QSize& size);
    int iconSpacing() const;
    void setIconSpacing(int spacing);

    QIcon closeIcon() const;
    void setCloseIcon(const QIcon& icon);
    QSize closeIconSize() const;
    void setCloseIconSize(const QSize& size);
    int closeButtonSize() const;
    void setCloseButtonSize(int size);
    bool closeButtonVisible() const;
    void setCloseButtonVisible(bool visible);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

signals:
    void closeRequested();

protected:
    bool event(QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    void applyStyle();
    void updateCloseGeometry();

    TagStyle m_style;
    QString m_text;
    QIcon m_icon;
    HoverIconButton* m_closeButton = nullptr;
};

} // namespace animatedwidgets

#ifndef ANIMATEDWIDGETS_DESIGNER_PLUGIN
using TagWidget = animatedwidgets::TagWidget;
#endif
