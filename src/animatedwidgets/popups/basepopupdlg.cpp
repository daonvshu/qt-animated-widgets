#include "animatedwidgets/popups/basepopupdlg.h"

#include "animatedwidgets/popups/fadeslidepopupanimation.h"
#include "animatedwidgets/popups/popupanimation.h"

#include <QApplication>
#include <QCloseEvent>
#include <QCursor>
#include <QGuiApplication>
#include <QHoverEvent>
#include <QScreen>
#include <QShowEvent>
#include <QTimer>
#include <QWindow>

namespace animatedwidgets {

BasePopupDlg::BasePopupDlg(QWidget* parent)
    : ShadowWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(windowFlags() | Qt::Popup | Qt::WindowStaysOnTopHint | Qt::NoDropShadowWindowHint | Qt::FramelessWindowHint);
    setPopupAnimation(new FadeSlidePopupAnimation);
}

void BasePopupDlg::moveTo(const QPoint& pos, Qt::Edge edgeAlign, Qt::Alignment posAlign) {
    m_initPos = pos;
    m_hasPlacementRequest = true;
    m_hasAnchorRect = false;
    arrowEdgeAlign = edgeAlign;
    arrowPosAlign = posAlign;
}

void BasePopupDlg::moveToTargetBottom(const QWidget* target, Qt::Alignment posAlign, int yOffset) {
    moveToTarget(target, Qt::TopEdge, posAlign, QPoint(0, yOffset));
}

void BasePopupDlg::moveToTargetTop(const QWidget* target, Qt::Alignment posAlign, int yOffset) {
    moveToTarget(target, Qt::BottomEdge, posAlign, QPoint(0, yOffset));
}

void BasePopupDlg::moveToTargetLeft(const QWidget* target, Qt::Alignment posAlign, int xOffset) {
    moveToTarget(target, Qt::RightEdge, posAlign, QPoint(xOffset, 0));
}

void BasePopupDlg::moveToTargetRight(const QWidget* target, Qt::Alignment posAlign, int xOffset) {
    moveToTarget(target, Qt::LeftEdge, posAlign, QPoint(xOffset, 0));
}

void BasePopupDlg::moveToTarget(const QWidget* target, Qt::Edge edgeAlign, Qt::Alignment posAlign, const QPoint& offset) {
    if (!target)
        return;
    moveToTargetRect(QRect(target->mapToGlobal(QPoint()), target->size()), edgeAlign, posAlign, offset);
}

void BasePopupDlg::moveToTargetBottom(const QRect& rect, Qt::Alignment posAlign, int yOffset) {
    moveToTargetRect(rect, Qt::TopEdge, posAlign, QPoint(0, yOffset));
}

void BasePopupDlg::moveToTargetTop(const QRect& rect, Qt::Alignment posAlign, int yOffset) {
    moveToTargetRect(rect, Qt::BottomEdge, posAlign, QPoint(0, yOffset));
}

void BasePopupDlg::moveToTargetLeft(const QRect& rect, Qt::Alignment posAlign, int xOffset) {
    moveToTargetRect(rect, Qt::RightEdge, posAlign, QPoint(xOffset, 0));
}

void BasePopupDlg::moveToTargetRight(const QRect& rect, Qt::Alignment posAlign, int xOffset) {
    moveToTargetRect(rect, Qt::LeftEdge, posAlign, QPoint(xOffset, 0));
}

void BasePopupDlg::moveToTargetRect(const QRect& rect, Qt::Edge edge, Qt::Alignment alignment, const QPoint& offset) {
    if (!rect.isValid())
        return;
    m_anchorRect = rect;
    m_anchorOffset = offset;
    m_hasPlacementRequest = true;
    m_hasAnchorRect = true;
    arrowEdgeAlign = edge;
    arrowPosAlign = alignment;
    m_initPos = calcAnchorPoint(arrowEdgeAlign, arrowPosAlign);
}

PopupAnimation* BasePopupDlg::popupAnimation() const {
    return m_animation;
}

void BasePopupDlg::setPopupAnimation(PopupAnimation* animation) {
    if (m_animation == animation)
        return;
    if (m_animation) {
        m_animation->stop();
        delete m_animation;
    }
    m_animation = animation;
    if (m_animation)
        m_animation->setParent(this);
    if (m_closing && m_hideAnimationStarted)
        startHideAnimation();
}

void BasePopupDlg::startHideAnimation() {
    m_hideAnimationStarted = true;
    const int generation = ++m_hideGeneration;
    if (m_animation) {
        m_animation->hide(this, arrowEdgeAlign, [this, generation] { finishClose(generation); });
    } else {
        finishClose(generation);
    }
}

void BasePopupDlg::finishClose(int generation) {
    QTimer::singleShot(0, this, [this, generation] {
        if (!m_closing || m_reallyClose || generation != m_hideGeneration)
            return;
        m_reallyClose = true;
        close();
    });
}

void BasePopupDlg::resizeEvent(QResizeEvent* event) {
    ShadowWidget::resizeEvent(event);
}

QPoint BasePopupDlg::calcAnchorPoint(Qt::Edge edge, Qt::Alignment alignment) const {
    if (!m_hasAnchorRect)
        return m_initPos;

    if (edge == Qt::TopEdge || edge == Qt::BottomEdge) {
        const int x = alignment & Qt::AlignHCenter ? m_anchorRect.center().x()
                : alignment & Qt::AlignRight ? m_anchorRect.right() + 1 : m_anchorRect.left();
        const int y = edge == Qt::TopEdge ? m_anchorRect.bottom() + 1 + m_anchorOffset.y()
                                           : m_anchorRect.top() - m_anchorOffset.y();
        return QPoint(x, y);
    }

    const int y = alignment & Qt::AlignVCenter ? m_anchorRect.center().y()
            : alignment & Qt::AlignBottom ? m_anchorRect.bottom() + 1 : m_anchorRect.top();
    const int x = edge == Qt::LeftEdge ? m_anchorRect.right() + 1 + m_anchorOffset.x()
                                        : m_anchorRect.left() - m_anchorOffset.x();
    return QPoint(x, y);
}

QRect BasePopupDlg::calcPopupRect(Qt::Edge edge, Qt::Alignment alignment) const {
    const QPoint anchor = calcAnchorPoint(edge, alignment);
    const QMargins margins = shadowMargins();
    int x = anchor.x();
    int y = anchor.y();
    if (edge == Qt::TopEdge || edge == Qt::BottomEdge) {
        y += edge == Qt::TopEdge ? -margins.top() : -height() + margins.bottom();
        x += alignment & Qt::AlignHCenter ? -width() / 2
                : alignment & Qt::AlignRight ? -width() + margins.right() : -margins.left();
    } else {
        x += edge == Qt::LeftEdge ? -margins.left() : -width() + margins.right();
        y += alignment & Qt::AlignVCenter ? -height() / 2
                : alignment & Qt::AlignBottom ? -height() + margins.bottom() : -margins.top();
    }
    return QRect(QPoint(x, y), size());
}

QRect BasePopupDlg::screenAvailableRectForPlacement() const {
    QScreen* screen = QGuiApplication::screenAt(calcAnchorPoint(arrowEdgeAlign, arrowPosAlign));
    if (!screen && parentWidget())
        screen = parentWidget()->screen();
    if (!screen)
        screen = QGuiApplication::primaryScreen();
    return screen ? screen->availableGeometry() : QRect();
}

void BasePopupDlg::placePopup() {
    if (m_hasPlacementRequest) {
        auto edgeAlign = arrowEdgeAlign;
        auto posAlign = arrowPosAlign;
        const QRect screenRect = screenAvailableRectForPlacement();
        auto popupRect = calcPopupRect(edgeAlign, posAlign);

        if (edgeAlign == Qt::TopEdge || edgeAlign == Qt::BottomEdge) {
            const bool overflowDown = popupRect.bottom() > screenRect.bottom();
            const bool overflowUp = popupRect.top() < screenRect.top();
            if ((edgeAlign == Qt::TopEdge && overflowDown) || (edgeAlign == Qt::BottomEdge && overflowUp)) {
                const auto flippedEdge = edgeAlign == Qt::TopEdge ? Qt::BottomEdge : Qt::TopEdge;
                const auto flippedRect = calcPopupRect(flippedEdge, posAlign);
                if (flippedRect.bottom() <= screenRect.bottom() && flippedRect.top() >= screenRect.top()) {
                    edgeAlign = flippedEdge;
                    popupRect = flippedRect;
                } else {
                    const int belowSpace = qMax(0, screenRect.bottom() - calcAnchorPoint(Qt::TopEdge, posAlign).y());
                    const int aboveSpace = qMax(0, calcAnchorPoint(Qt::BottomEdge, posAlign).y() - screenRect.top());
                    edgeAlign = aboveSpace > belowSpace ? Qt::BottomEdge : Qt::TopEdge;
                    popupRect = calcPopupRect(edgeAlign, posAlign);
                }
            }

            if (popupRect.right() > screenRect.right()) {
                const auto altRect = calcPopupRect(edgeAlign, Qt::AlignRight);
                if (altRect.right() <= screenRect.right() && altRect.left() >= screenRect.left()) {
                    posAlign = Qt::AlignRight;
                    popupRect = altRect;
                }
            } else if (popupRect.left() < screenRect.left()) {
                const auto altRect = calcPopupRect(edgeAlign, Qt::AlignLeft);
                if (altRect.right() <= screenRect.right() && altRect.left() >= screenRect.left()) {
                    posAlign = Qt::AlignLeft;
                    popupRect = altRect;
                }
            }
        } else {
            const bool overflowRight = popupRect.right() > screenRect.right();
            const bool overflowLeft = popupRect.left() < screenRect.left();
            if ((edgeAlign == Qt::LeftEdge && overflowRight) || (edgeAlign == Qt::RightEdge && overflowLeft)) {
                const auto flippedEdge = edgeAlign == Qt::LeftEdge ? Qt::RightEdge : Qt::LeftEdge;
                const auto flippedRect = calcPopupRect(flippedEdge, posAlign);
                if (flippedRect.right() <= screenRect.right() && flippedRect.left() >= screenRect.left()) {
                    edgeAlign = flippedEdge;
                    popupRect = flippedRect;
                } else {
                    const int rightSpace = qMax(0, screenRect.right() - calcAnchorPoint(Qt::LeftEdge, posAlign).x());
                    const int leftSpace = qMax(0, calcAnchorPoint(Qt::RightEdge, posAlign).x() - screenRect.left());
                    edgeAlign = leftSpace > rightSpace ? Qt::RightEdge : Qt::LeftEdge;
                    popupRect = calcPopupRect(edgeAlign, posAlign);
                }
            }

            if (popupRect.bottom() > screenRect.bottom()) {
                const auto altRect = calcPopupRect(edgeAlign, Qt::AlignBottom);
                if (altRect.bottom() <= screenRect.bottom() && altRect.top() >= screenRect.top()) {
                    posAlign = Qt::AlignBottom;
                    popupRect = altRect;
                }
            } else if (popupRect.top() < screenRect.top()) {
                const auto altRect = calcPopupRect(edgeAlign, Qt::AlignTop);
                if (altRect.bottom() <= screenRect.bottom() && altRect.top() >= screenRect.top()) {
                    posAlign = Qt::AlignTop;
                    popupRect = altRect;
                }
            }
        }

        const int targetX = popupRect.width() <= screenRect.width()
                ? qBound(screenRect.left(), popupRect.x(), screenRect.right() - popupRect.width() + 1) : screenRect.left();
        const int targetY = popupRect.height() <= screenRect.height()
                ? qBound(screenRect.top(), popupRect.y(), screenRect.bottom() - popupRect.height() + 1) : screenRect.top();
        arrowEdgeAlign = edgeAlign;
        arrowPosAlign = posAlign;
        move(targetX, targetY);
    } else {
        QScreen* screen = qApp->activeWindow() && qApp->activeWindow()->windowHandle()
                ? qApp->activeWindow()->windowHandle()->screen() : QGuiApplication::primaryScreen();
        if (screen)
            move(screen->geometry().center() - QPoint(width() / 2, height() / 2));
    }
}

void BasePopupDlg::updatePopupPlacement() {
    if (!isVisible() || m_closing || !m_hasPlacementRequest)
        return;
    ++m_showGeneration;
    if (m_animation)
        m_animation->stop();
    setWindowOpacity(1.0);
    placePopup();
}

void BasePopupDlg::showEvent(QShowEvent* event) {
    placePopup();
    const int generation = ++m_showGeneration;
    const QRect targetGeometry = geometry();
    const Qt::Edge targetEdge = arrowEdgeAlign;
    if (m_animation)
        m_animation->prepareShow(this, targetEdge, targetGeometry);
    ShadowWidget::showEvent(event);
    QTimer::singleShot(0, this, [this, targetEdge, targetGeometry, generation] {
        if (isVisible() && !m_closing && m_animation && generation == m_showGeneration)
            m_animation->show(this, targetEdge, targetGeometry, [] {});
    });
    if (!(windowFlags() & Qt::WindowDoesNotAcceptFocus)) {
        activateWindow();
        setFocus();
    }
}

void BasePopupDlg::closeEvent(QCloseEvent* event) {
    if (m_reallyClose) {
        emit popClosed();
        ShadowWidget::closeEvent(event);
        m_reallyClose = false;
        if (auto* parent = parentWidget()) {
            const QPoint localPos = parent->mapFromGlobal(QCursor::pos());
            if (!parent->rect().contains(localPos)) {
                QHoverEvent leave(QEvent::Leave, QPointF(), QPointF());
                QCoreApplication::sendEvent(parent, &leave);
                parent->update();
            }
        }
        return;
    }
    event->ignore();
    if (m_closing)
        return;
    m_closing = true;
    emit closeRequested();
    if (!m_hideAnimationStarted)
        startHideAnimation();
}

} // namespace animatedwidgets
