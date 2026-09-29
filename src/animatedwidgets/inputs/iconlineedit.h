#pragma once

#include "animatedwidgets/inputs/lineedit.h"

#include <QIcon>

class HoverIconButton;
class QResizeEvent;

class IconLineEdit : public LineEdit {
    Q_OBJECT
    Q_PROPERTY(QIcon leadingIcon READ leadingIcon WRITE setLeadingIcon)
    Q_PROPERTY(QIcon trailingIcon READ trailingIcon WRITE setTrailingIcon)
    Q_PROPERTY(QSize leadingIconSize READ leadingIconSize WRITE setLeadingIconSize)
    Q_PROPERTY(QSize trailingIconSize READ trailingIconSize WRITE setTrailingIconSize)
    Q_PROPERTY(bool leadingIconVisible READ leadingIconVisible WRITE setLeadingIconVisible)
    Q_PROPERTY(bool trailingIconVisible READ trailingIconVisible WRITE setTrailingIconVisible)
    Q_PROPERTY(int iconSpacing READ iconSpacing WRITE setIconSpacing)

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

namespace animatedwidgets {
using IconLineEdit = ::IconLineEdit;
} // namespace animatedwidgets
