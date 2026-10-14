#pragma once

#include "animatedwidgets/namespacealias.h"

#include "animatedwidgets/inputs/iconlineedit.h"

#include <QTimer>

namespace animatedwidgets {

class PasswordLineEdit : public IconLineEdit {
    Q_OBJECT
    Q_PROPERTY(bool passwordVisible READ passwordVisible WRITE setPasswordVisible)
    Q_PROPERTY(QIcon showPasswordIcon READ showPasswordIcon WRITE setShowPasswordIcon)
    Q_PROPERTY(QIcon hidePasswordIcon READ hidePasswordIcon WRITE setHidePasswordIcon)
    Q_PROPERTY(bool autoHideEnabled READ autoHideEnabled WRITE setAutoHideEnabled)
    Q_PROPERTY(int autoHideDuration READ autoHideDuration WRITE setAutoHideDuration)

public:
    explicit PasswordLineEdit(QWidget* parent = nullptr);

    bool passwordVisible() const;
    void setPasswordVisible(bool visible);
    QIcon showPasswordIcon() const;
    void setShowPasswordIcon(const QIcon& icon);
    QIcon hidePasswordIcon() const;
    void setHidePasswordIcon(const QIcon& icon);
    bool autoHideEnabled() const;
    void setAutoHideEnabled(bool enabled);
    int autoHideDuration() const;
    void setAutoHideDuration(int duration);

protected:
    bool event(QEvent* event) override;

private:
    QIcon m_showPasswordIcon;
    QIcon m_hidePasswordIcon;
    QTimer m_autoHideTimer;
    bool m_passwordVisible = false;
    bool m_autoHideEnabled = true;
    int m_autoHideDuration = 3000;
};

} // namespace animatedwidgets

#ifndef ANIMATEDWIDGETS_DESIGNER_PLUGIN
using PasswordLineEdit = animatedwidgets::PasswordLineEdit;
#endif
