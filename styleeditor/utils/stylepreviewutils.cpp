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

    bool enabled = true;
    bool checked = false;
    bool checkable = false;
    for (const auto& property : properties) {
        if (property.name == QStringLiteral("enabled"))
            enabled = property.value.toBool();
        else if (property.name == QStringLiteral("checked"))
            checked = property.value.toBool();
        else if (property.name == QStringLiteral("checkable"))
            checkable = property.value.toBool();
    }
    preview->setEnabled(enabled && state != 3);
    preview->setProperty("styleEditorState", state);
    if (auto* button = qobject_cast<QAbstractButton*>(preview)) {
        button->setCheckable(checkable || state == 4);
        button->setChecked((checkable && checked) || state == 4);
        button->setDown(state == 2);
    }
    preview->update();
}
