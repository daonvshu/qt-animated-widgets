#pragma once

#include <QColor>
#include <QObject>
#include <QPointer>
#include <QVector>

class QWidget;
class ScreenColorOverlay;

class ScreenColorPicker : public QObject {
    Q_OBJECT

public:
    explicit ScreenColorPicker(QWidget* parent);
    ~ScreenColorPicker() override;

    bool start();
    void cancel();

signals:
    void colorSelected(const QColor& color);
    void finished();

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    void clearOverlays();

    QPointer<QWidget> m_owner;
    QVector<QPointer<ScreenColorOverlay>> m_overlays;
    QVector<QMetaObject::Connection> m_screenConnections;
};
