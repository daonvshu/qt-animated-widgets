#include "screencoloroverlay.h"

#include "../../utils/screencolorutils.h"

#include <QCloseEvent>
#include <QCursor>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QScreen>
#include <QWindow>

ScreenColorOverlay::ScreenColorOverlay(QScreen* screen, const QImage& image, QWidget* parent)
    : QWidget(parent, Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::NoDropShadowWindowHint)
    , m_image(image)
    , m_screenGeometry(screen->geometry())
    , m_position(QCursor::pos())
{
    setObjectName(QStringLiteral("ScreenColorOverlay"));
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    setCursor(Qt::CrossCursor);

    m_image.setDevicePixelRatio(1.0);
    winId();
    windowHandle()->setScreen(screen);
    setGeometry(m_screenGeometry);
}

void ScreenColorOverlay::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.drawImage(QRectF(rect()), m_image, QRectF(m_image.rect()));
    const QColor color = ScreenColorUtils::colorAt(m_image, m_screenGeometry, m_position);
    if (!color.isValid() || !rect().contains(mapFromGlobal(QCursor::pos())))
        return;

    const QPointF cursor = m_position - m_screenGeometry.topLeft();
    const QSizeF badgeSize(204, 150);
    QPointF origin = cursor + QPointF(24, 24);
    if (origin.x() + badgeSize.width() > width() - 8)
        origin.setX(cursor.x() - badgeSize.width() - 24);
    if (origin.y() + badgeSize.height() > height() - 8)
        origin.setY(cursor.y() - badgeSize.height() - 24);
    origin.setX(qBound(qreal(0), origin.x(), qMax(qreal(0), width() - badgeSize.width())));
    origin.setY(qBound(qreal(0), origin.y(), qMax(qreal(0), height() - badgeSize.height())));

    const QRectF badge(origin, badgeSize);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(QColor(QStringLiteral("#647084")), 1));
    painter.setBrush(QColor(QStringLiteral("#202636")));
    painter.drawRoundedRect(badge.adjusted(0.5, 0.5, -0.5, -0.5), 6, 6);
    painter.setBrush(Qt::NoBrush);

    const QPoint pixel = ScreenColorUtils::imagePosition(m_image.size(), m_screenGeometry, m_position);
    const QRectF magnifier(origin + QPointF(8, 8), QSizeF(90, 90));
    painter.fillRect(magnifier, Qt::black);
    painter.drawImage(magnifier, m_image, QRectF(pixel - QPoint(4, 4), QSizeF(9, 9)));
    painter.setPen(QPen(Qt::black, 3));
    const QRectF selected(magnifier.topLeft() + QPointF(40, 40), QSizeF(10, 10));
    painter.drawRect(selected);
    painter.setPen(QPen(Qt::white, 1));
    painter.drawRect(selected);

    const QRectF swatch(origin + QPointF(110, 12), QSizeF(82, 44));
    painter.fillRect(swatch, color);
    painter.setPen(QColor(QStringLiteral("#e7ecf5")));
    painter.drawRect(swatch);
    QFont labelFont = font();
    labelFont.setPixelSize(12);
    painter.setFont(labelFont);
    painter.drawText(QRectF(origin + QPointF(104, 64), QSizeF(96, 24)), Qt::AlignCenter, color.name().toUpper());

    labelFont.setPixelSize(11);
    painter.setFont(labelFont);
    painter.drawText(badge.adjusted(10, 104, -10, -6), Qt::AlignLeft | Qt::AlignTop,
                     QStringLiteral("Click to pick\nEsc / right-click to cancel"));
}

void ScreenColorOverlay::mouseMoveEvent(QMouseEvent* event) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    m_position = event->globalPosition();
#else
    m_position = event->screenPos();
#endif
    update();
    event->accept();
}

void ScreenColorOverlay::leaveEvent(QEvent* event) {
    update();
    QWidget::leaveEvent(event);
}

void ScreenColorOverlay::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        selectColor(event->globalPosition());
#else
        selectColor(event->screenPos());
#endif
    } else if (event->button() == Qt::RightButton) {
        emit cancelled();
    }
    event->accept();
}

void ScreenColorOverlay::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape)
        emit cancelled();
    event->accept();
}

void ScreenColorOverlay::closeEvent(QCloseEvent* event) {
    event->ignore();
    emit cancelled();
}

void ScreenColorOverlay::selectColor(const QPointF& globalPosition) {
    const QColor color = ScreenColorUtils::colorAt(m_image, m_screenGeometry, globalPosition);
    if (color.isValid())
        emit colorSelected(color);
}
