#include "stylepreviewutils.h"

#include <QAbstractButton>
#include <QVariant>

QSize StylePreviewUtils::resolveSize(const QSize& requested, const QSize& suggested, const QSize& hint) {
    QSize size = hint;
    if (suggested.width() > 0 && suggested.height() > 0)
        size = suggested;
    if (requested.width() > 0 && requested.height() > 0)
        size = requested;
    return QSize(qBound(1, size.width() > 0 ? size.width() : 150, 4096), qBound(1, size.height() > 0 ? size.height() : 42, 4096));
}

void StylePreviewUtils::applyState(QWidget* preview, int state) {
    if (!preview)
        return;

    preview->setEnabled(state != 3);
    preview->setProperty("styleEditorState", state);
    if (auto* button = qobject_cast<QAbstractButton*>(preview)) {
        button->setChecked(state == 4);
        button->setDown(state == 2);
    }
    preview->update();
}
