#pragma once

#include <QObject>

class QWidget;

namespace animatedwidgets {

class PopupMenuViewportClip : public QObject {
    Q_OBJECT

public:
    explicit PopupMenuViewportClip(QWidget* viewport);

    static void apply(QWidget* viewport, int radius);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    void updateMask();

    QWidget* m_viewport = nullptr;
    int m_radius = 0;
};

} // namespace animatedwidgets
