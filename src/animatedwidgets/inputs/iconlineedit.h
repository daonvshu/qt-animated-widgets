#pragma once

#include "animatedwidgets/namespacealias.h"

#include "animatedwidgets/inputs/lineedit.h"

#include <QIcon>

class QResizeEvent;

namespace animatedwidgets {

class HoverIconButton;

class IconLineEdit : public LineEdit {
    Q_OBJECT
    Q_PROPERTY(QIcon leadingIcon READ leadingIcon WRITE setLeadingIcon)
    Q_PROPERTY(QIcon trailingIcon READ trailingIcon WRITE setTrailingIcon)
    Q_PROPERTY(QSize leadingIconSize READ leadingIconSize WRITE setLeadingIconSize)
    Q_PROPERTY(QSize trailingIconSize READ trailingIconSize WRITE setTrailingIconSize)
    Q_PROPERTY(bool leadingIconVisible READ leadingIconVisible WRITE setLeadingIconVisible)
    Q_PROPERTY(bool trailingIconVisible READ trailingIconVisible WRITE setTrailingIconVisible)
    Q_PROPERTY(int iconSpacing READ iconSpacing WRITE setIconSpacing)
    Q_PROPERTY(QColor leadingIconNormalColor READ leadingIconNormalColor WRITE setLeadingIconNormalColor)
    Q_PROPERTY(QColor leadingIconHoverColor READ leadingIconHoverColor WRITE setLeadingIconHoverColor)
    Q_PROPERTY(QColor leadingIconPressedColor READ leadingIconPressedColor WRITE setLeadingIconPressedColor)
    Q_PROPERTY(QColor leadingIconDisabledColor READ leadingIconDisabledColor WRITE setLeadingIconDisabledColor)
    Q_PROPERTY(QColor leadingIconBackgroundHover READ leadingIconBackgroundHover WRITE setLeadingIconBackgroundHover)
    Q_PROPERTY(QColor leadingIconBackgroundPressed READ leadingIconBackgroundPressed WRITE setLeadingIconBackgroundPressed)
    Q_PROPERTY(QColor leadingIconBackgroundDisabled READ leadingIconBackgroundDisabled WRITE setLeadingIconBackgroundDisabled)
    Q_PROPERTY(QColor trailingIconNormalColor READ trailingIconNormalColor WRITE setTrailingIconNormalColor)
    Q_PROPERTY(QColor trailingIconHoverColor READ trailingIconHoverColor WRITE setTrailingIconHoverColor)
    Q_PROPERTY(QColor trailingIconPressedColor READ trailingIconPressedColor WRITE setTrailingIconPressedColor)
    Q_PROPERTY(QColor trailingIconDisabledColor READ trailingIconDisabledColor WRITE setTrailingIconDisabledColor)
    Q_PROPERTY(QColor trailingIconBackgroundHover READ trailingIconBackgroundHover WRITE setTrailingIconBackgroundHover)
    Q_PROPERTY(QColor trailingIconBackgroundPressed READ trailingIconBackgroundPressed WRITE setTrailingIconBackgroundPressed)
    Q_PROPERTY(QColor trailingIconBackgroundDisabled READ trailingIconBackgroundDisabled WRITE setTrailingIconBackgroundDisabled)

public:
    explicit IconLineEdit(QWidget* parent = nullptr);

    QIcon leadingIcon() const;
    void setLeadingIcon(const QIcon& icon);
    QIcon trailingIcon() const;
    void setTrailingIcon(const QIcon& icon);
    QSize leadingIconSize() const;
    void setLeadingIconSize(const QSize& size);
    QSize trailingIconSize() const;
    void setTrailingIconSize(const QSize& size);
    bool leadingIconVisible() const;
    void setLeadingIconVisible(bool visible);
    bool trailingIconVisible() const;
    void setTrailingIconVisible(bool visible);
    int iconSpacing() const;
    void setIconSpacing(int spacing);
    QColor leadingIconNormalColor() const;
    void setLeadingIconNormalColor(const QColor& color);
    QColor leadingIconHoverColor() const;
    void setLeadingIconHoverColor(const QColor& color);
    QColor leadingIconPressedColor() const;
    void setLeadingIconPressedColor(const QColor& color);
    QColor leadingIconDisabledColor() const;
    void setLeadingIconDisabledColor(const QColor& color);
    QColor leadingIconBackgroundHover() const;
    void setLeadingIconBackgroundHover(const QColor& color);
    QColor leadingIconBackgroundPressed() const;
    void setLeadingIconBackgroundPressed(const QColor& color);
    QColor leadingIconBackgroundDisabled() const;
    void setLeadingIconBackgroundDisabled(const QColor& color);

    QColor trailingIconNormalColor() const;
    void setTrailingIconNormalColor(const QColor& color);
    QColor trailingIconHoverColor() const;
    void setTrailingIconHoverColor(const QColor& color);
    QColor trailingIconPressedColor() const;
    void setTrailingIconPressedColor(const QColor& color);
    QColor trailingIconDisabledColor() const;
    void setTrailingIconDisabledColor(const QColor& color);
    QColor trailingIconBackgroundHover() const;
    void setTrailingIconBackgroundHover(const QColor& color);
    QColor trailingIconBackgroundPressed() const;
    void setTrailingIconBackgroundPressed(const QColor& color);
    QColor trailingIconBackgroundDisabled() const;
    void setTrailingIconBackgroundDisabled(const QColor& color);

    HoverIconButton* leadingButton() const;
    HoverIconButton* trailingButton() const;
    void setAnimationDuration(int duration) override;
    QSize sizeHint() const override;

signals:
    void leadingIconClicked();
    void trailingIconClicked();

protected:
    bool event(QEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void updateContentMargins() override;

private:
    QSize effectiveIconSize(const QSize& requested) const;
    void updateIconGeometry();

    HoverIconButton* m_leadingButton = nullptr;
    HoverIconButton* m_trailingButton = nullptr;
    QIcon m_leadingIcon;
    QIcon m_trailingIcon;
    QSize m_leadingIconSize = QSize(16, 16);
    QSize m_trailingIconSize = QSize(16, 16);
    bool m_leadingVisible = true;
    bool m_trailingVisible = true;
    int m_iconSpacing = 6;
};

} // namespace animatedwidgets

#ifndef ANIMATEDWIDGETS_DESIGNER_PLUGIN
using IconLineEdit = animatedwidgets::IconLineEdit;
#endif
