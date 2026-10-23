#include "stylepreviewutils.h"

#include <QAbstractButton>
#include <QVariant>

int StylePreviewUtils::resolveDimension(int requested, int suggested, int hint, bool autoSize, int fallback) {
    const int value = autoSize ? hint : requested > 0 ? requested : suggested > 0 ? suggested : hint;
    return qBound(1, value > 0 ? value : fallback, 4096);
}

QSize StylePreviewUtils::resolveSize(const QSize& requested, const QSize& suggested, const QSize& hint,
                                     bool widthAuto, bool heightAuto) {
    return QSize(resolveDimension(requested.width(), suggested.width(), hint.width(), widthAuto, 150),
                 resolveDimension(requested.height(), suggested.height(), hint.height(), heightAuto, 42));
}

void StylePreviewUtils::applyState(QWidget* preview, int state, const QList<animatedwidgets::styleeditor::StyleProperty>& properties) {
    if (!preview)
        return;

    auto* button = qobject_cast<QAbstractButton*>(preview);
    bool enabled = true;
    bool checked = button && button->isChecked();
    bool checkable = button && button->isCheckable();
    const animatedwidgets::styleeditor::StyleProperty* checkState = nullptr;
    for (const auto& property : properties) {
        if (property.name == QStringLiteral("enabled"))
            enabled = property.value.toBool();
        else if (property.name == QStringLiteral("checked"))
            checked = property.value.toBool();
        else if (property.name == QStringLiteral("checkable"))
            checkable = property.value.toBool();
        else if (property.name == QStringLiteral("checkState"))
            checkState = &property;
    }
    preview->setEnabled(enabled && state != 3);
    preview->setProperty("styleEditorState", state);
    if (button) {
        button->setCheckable(checkable || state == 4);
        if (state != 4 && checkState && checkState->apply) {
            checkState->apply(checkState->value);
        } else {
            const bool targetChecked = (checkable && checked) || state == 4;
            if (button->isChecked() != targetChecked)
                button->setChecked(targetChecked);
        }
        button->setDown(state == 2);
    }
    preview->update();
}
