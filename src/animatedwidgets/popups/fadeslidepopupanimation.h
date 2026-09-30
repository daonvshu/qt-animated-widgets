#pragma once

#include "animatedwidgets/popups/popupanimation.h"

#include <QParallelAnimationGroup>
#include <QPoint>
#include <QPropertyAnimation>

namespace animatedwidgets {

class FadeSlidePopupAnimation final : public PopupAnimation {
    Q_OBJECT

public:
    explicit FadeSlidePopupAnimation(QObject* parent = nullptr);

    void prepareShow(QWidget* popup, Qt::Edge edge, const QRect& target) override;
    void show(QWidget* popup, Qt::Edge edge, const QRect& target, std::function<void()> finished) override;
    void hide(QWidget* popup, Qt::Edge edge, std::function<void()> finished) override;
    void stop() override;

    int duration() const;
    void setDuration(int duration);
    int slideDistance() const;
    void setSlideDistance(int distance);

private:
    static QPoint slideOffset(Qt::Edge edge, int distance);

    QParallelAnimationGroup m_showGroup;
    QParallelAnimationGroup m_hideGroup;
    QPropertyAnimation* m_showOpacity = nullptr;
    QPropertyAnimation* m_showGeometry = nullptr;
    QPropertyAnimation* m_hideOpacity = nullptr;
    QPropertyAnimation* m_hideGeometry = nullptr;
    std::function<void()> m_showFinished;
    std::function<void()> m_hideFinished;
    int m_duration = 180;
    int m_slideDistance = 30;
};

} // namespace animatedwidgets
