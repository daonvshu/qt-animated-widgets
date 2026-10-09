#pragma once

#include <QObject>
#include <QPointer>

class QWidget;

namespace animatedwidgets {

class PopupDismissFilter : public QObject {
    Q_OBJECT

public:
    explicit PopupDismissFilter(QWidget* popup, QWidget* anchor);
    void stop();

signals:
    void dismissRequested();

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    QPointer<QWidget> m_popup;
    QPointer<QWidget> m_anchor;
};

} // namespace animatedwidgets
