#include "screencolorpicker.h"

#include "screencoloroverlay.h"

#include <QCursor>
#include <QEvent>
#include <QGuiApplication>
#include <QPixmap>
#include <QScreen>

ScreenColorPicker::ScreenColorPicker(QWidget* parent)
    : QObject(parent)
    , m_owner(parent)
{
    if (parent)
        parent->installEventFilter(this);
    connect(qGuiApp, &QGuiApplication::screenRemoved, this, &ScreenColorPicker::cancel);
    connect(qGuiApp, &QGuiApplication::applicationStateChanged, this, [this](Qt::ApplicationState state) {
        if (state != Qt::ApplicationActive)
            cancel();
    });
}

ScreenColorPicker::~ScreenColorPicker() {
    clearOverlays();
}

bool ScreenColorPicker::eventFilter(QObject* watched, QEvent* event) {
    if (watched == m_owner && event->type() == QEvent::Hide)
        cancel();
    return QObject::eventFilter(watched, event);
}

bool ScreenColorPicker::start() {
    if (!m_owner || !m_overlays.isEmpty())
        return false;

    const auto screens = QGuiApplication::screens();
    if (screens.isEmpty())
        return false;

    QVector<QImage> captures;
    captures.reserve(screens.size());
    for (QScreen* screen : screens) {
        const QImage image = screen->grabWindow(0).toImage();
        if (image.isNull())
            return false;
        captures.append(image);
    }

    ScreenColorOverlay* active = nullptr;
    const int screenCount = static_cast<int>(screens.size());
    for (int i = 0; i < screenCount; ++i) {
        QScreen* screen = screens.at(i);
        auto* overlay = new ScreenColorOverlay(screen, captures.at(i), m_owner);
        m_overlays.append(overlay);
        if (screen->geometry().contains(QCursor::pos()))
            active = overlay;

        m_screenConnections.append(connect(screen, &QScreen::geometryChanged, this, &ScreenColorPicker::cancel));
        m_screenConnections.append(connect(screen, &QScreen::logicalDotsPerInchChanged, this, &ScreenColorPicker::cancel));
        connect(overlay, &ScreenColorOverlay::cancelled, this, &ScreenColorPicker::cancel);
        connect(overlay, &ScreenColorOverlay::colorSelected, this, [this](const QColor& color) {
            clearOverlays();
            emit colorSelected(color);
            emit finished();
        });
    }

    const auto overlays = m_overlays;
    for (const auto& overlay : overlays) {
        overlay->show();
        if (m_overlays.isEmpty())
            return true;
    }
    if (!active)
        active = m_overlays.constFirst().data();
    active->raise();
    active->activateWindow();
    active->setFocus();
    return true;
}

void ScreenColorPicker::cancel() {
    if (m_overlays.isEmpty())
        return;

    clearOverlays();
    emit finished();
}

void ScreenColorPicker::clearOverlays() {
    for (const auto& connection : m_screenConnections)
        disconnect(connection);
    m_screenConnections.clear();

    const auto overlays = m_overlays;
    m_overlays.clear();
    for (const auto& overlay : overlays) {
        if (overlay) {
            disconnect(overlay.data(), nullptr, this, nullptr);
            overlay->hide();
            overlay->deleteLater();
        }
    }
}
