#pragma once

#include "animatedwidgets/popups/shadowwidget.h"

#include <QPointer>
#include <QRect>

namespace animatedwidgets {

class PopupAnimation;

class BasePopupDlg : public ShadowWidget {
    Q_OBJECT

public:
    explicit BasePopupDlg(QWidget* parent = nullptr);

    void moveTo(const QPoint& pos, Qt::Edge edgeAlign = Qt::TopEdge, Qt::Alignment posAlign = Qt::AlignHCenter);
    void moveToTargetBottom(const QWidget* target, Qt::Alignment posAlign = Qt::AlignLeft, int yOffset = 4);
    void moveToTargetTop(const QWidget* target, Qt::Alignment posAlign = Qt::AlignLeft, int yOffset = 4);
    void moveToTargetLeft(const QWidget* target, Qt::Alignment posAlign = Qt::AlignTop, int xOffset = 4);
    void moveToTargetRight(const QWidget* target, Qt::Alignment posAlign = Qt::AlignTop, int xOffset = 4);
    void moveToTargetBottom(const QRect& targetRectGlobal, Qt::Alignment posAlign = Qt::AlignLeft, int yOffset = 4);
    void moveToTargetTop(const QRect& targetRectGlobal, Qt::Alignment posAlign = Qt::AlignLeft, int yOffset = 4);
    void moveToTargetLeft(const QRect& targetRectGlobal, Qt::Alignment posAlign = Qt::AlignTop, int xOffset = 4);
    void moveToTargetRight(const QRect& targetRectGlobal, Qt::Alignment posAlign = Qt::AlignTop, int xOffset = 4);
    PopupAnimation* popupAnimation() const;
    void setPopupAnimation(PopupAnimation* animation);

signals:
    void closeRequested();
    void popClosed();

protected:
    void updatePopupPlacement();

    void resizeEvent(QResizeEvent* event) override;
    void showEvent(QShowEvent* event) override;
    void closeEvent(QCloseEvent* event) override;

private:
    void startHideAnimation();
    void finishClose(int generation);
    void placePopup();
    void moveToTarget(const QWidget* target, Qt::Edge edgeAlign, Qt::Alignment posAlign, const QPoint& offset);
    void moveToTargetRect(const QRect& rect, Qt::Edge edge, Qt::Alignment alignment, const QPoint& offset);
    QPoint calcAnchorPoint(Qt::Edge edge, Qt::Alignment alignment) const;
    QRect calcPopupRect(Qt::Edge edge, Qt::Alignment alignment) const;
    QRect screenAvailableRectForPlacement() const;

    QRect m_anchorRect;
    QPoint m_initPos;
    QPoint m_anchorOffset;
    bool m_hasPlacementRequest = false;
    bool m_hasAnchorRect = false;
    bool m_closing = false;
    bool m_reallyClose = false;
    bool m_hideAnimationStarted = false;
    int m_hideGeneration = 0;
    int m_showGeneration = 0;
    QPointer<PopupAnimation> m_animation;
};

} // namespace animatedwidgets
