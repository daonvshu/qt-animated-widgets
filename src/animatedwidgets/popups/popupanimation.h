#pragma once

#include <QObject>
#include <QRect>

#include <functional>

class QWidget;

namespace animatedwidgets {

class PopupAnimation : public QObject {
    Q_OBJECT

public:
    explicit PopupAnimation(QObject* parent = nullptr);
    ~PopupAnimation() override = default;

    virtual void prepareShow(QWidget* popup, Qt::Edge edge, const QRect& target);
    virtual void show(QWidget* popup, Qt::Edge edge, const QRect& target, std::function<void()> finished) = 0;
    virtual void hide(QWidget* popup, Qt::Edge edge, std::function<void()> finished) = 0;
    virtual void stop() = 0;
};

} // namespace animatedwidgets
