#include "animatedwidgets/inputs/passwordlineedit.h"

#include <QEvent>

PasswordLineEdit::PasswordLineEdit(QWidget* parent)
    : IconLineEdit(parent), m_autoHideTimer(this)
{
    m_showPasswordIcon = QIcon(QStringLiteral(":/animatedwidgets/inputs/eyeopen.svg"));
    m_hidePasswordIcon = QIcon(QStringLiteral(":/animatedwidgets/inputs/eyeclosed.svg"));
    setEchoMode(QLineEdit::Password);
    setTrailingIcon(m_showPasswordIcon);
    m_autoHideTimer.setSingleShot(true);
    connect(&m_autoHideTimer, &QTimer::timeout, this, [this] { setPasswordVisible(false); });
    connect(this, &IconLineEdit::trailingIconClicked, this,
            [this] { setPasswordVisible(!m_passwordVisible); });
}

bool PasswordLineEdit::passwordVisible() const { return m_passwordVisible; }
void PasswordLineEdit::setPasswordVisible(bool visible)
{
    if (m_passwordVisible == visible) return;
    m_passwordVisible = visible;
    setEchoMode(visible ? QLineEdit::Normal : QLineEdit::Password);
    setTrailingIcon(visible ? m_hidePasswordIcon : m_showPasswordIcon);
    if (visible && m_autoHideEnabled) m_autoHideTimer.start(m_autoHideDuration);
    else m_autoHideTimer.stop();
}

QIcon PasswordLineEdit::showPasswordIcon() const { return m_showPasswordIcon; }
void PasswordLineEdit::setShowPasswordIcon(const QIcon& icon)
{
    m_showPasswordIcon = icon;
    if (!m_passwordVisible) setTrailingIcon(icon);
}

QIcon PasswordLineEdit::hidePasswordIcon() const { return m_hidePasswordIcon; }
void PasswordLineEdit::setHidePasswordIcon(const QIcon& icon)
{
    m_hidePasswordIcon = icon;
    if (m_passwordVisible) setTrailingIcon(icon);
}

bool PasswordLineEdit::autoHideEnabled() const { return m_autoHideEnabled; }
void PasswordLineEdit::setAutoHideEnabled(bool enabled)
{
    m_autoHideEnabled = enabled;
    if (enabled && m_passwordVisible) m_autoHideTimer.start(m_autoHideDuration);
    else m_autoHideTimer.stop();
}

int PasswordLineEdit::autoHideDuration() const { return m_autoHideDuration; }
void PasswordLineEdit::setAutoHideDuration(int duration)
{
    m_autoHideDuration = qMax(0, duration);
    if (m_autoHideEnabled && m_passwordVisible) m_autoHideTimer.start(m_autoHideDuration);
}

bool PasswordLineEdit::event(QEvent* event)
{
    const bool handled = IconLineEdit::event(event);
    if (event->type() == QEvent::FocusOut || event->type() == QEvent::EnabledChange) {
        if (!isEnabled() || !hasFocus()) setPasswordVisible(false);
    }
    return handled;
}
