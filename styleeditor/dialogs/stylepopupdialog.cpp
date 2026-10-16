#include "stylepopupdialog.h"

#include "../themes/stylethememanager.h"

#include <QKeyEvent>

StylePopupDialog::StylePopupDialog(QWidget* parent)
    : BasePopupDlg(parent)
{
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    setWindowModality(Qt::WindowModal);
    shadowCornerRadius = 12;

    connect(this, &BasePopupDlg::closeRequested, this, [this] {
        m_finishing = true;
        setEnabled(false);
    });
    connect(this, &BasePopupDlg::popClosed, this, [this] { emit finished(m_result); });
}

void StylePopupDialog::showDialog() {
    StyleThemeManager::prepareWidget(this);
    ensureShadowSizeExpanded();
    if (const QWidget* owner = parentWidget()) {
        const QRect ownerRect(owner->mapToGlobal(QPoint()), owner->size());
        const QPoint position = ownerRect.center() - QPoint(width() / 2, height() / 2);
        const QMargins margins = shadowMargins();
        moveTo(position + QPoint(margins.left(), margins.top()), Qt::TopEdge, Qt::AlignLeft);
    }
    show();
}

void StylePopupDialog::finish(int result) {
    if (m_finishing)
        return;

    m_result = result;
    close();
}

void StylePopupDialog::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        close();
        event->accept();
        return;
    }
    BasePopupDlg::keyPressEvent(event);
}
