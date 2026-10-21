#include "buttonstyleplugin.h"

#include "buttonstyleproperties.h"

#include "animatedwidgets/buttons/borderedbutton.h"
#include "animatedwidgets/buttons/borderediconbutton.h"
#include "animatedwidgets/buttons/filledbutton.h"
#include "animatedwidgets/buttons/fillediconbutton.h"
#include "animatedwidgets/buttons/hoverbutton.h"
#include "animatedwidgets/buttons/hovericonbutton.h"
#include "animatedwidgets/buttons/outlinebutton.h"
#include "animatedwidgets/buttons/outlineiconbutton.h"
#include "animatedwidgets/styleeditor/api/stylewidgettypebuilder.h"

using animatedwidgets::styleeditor::StyleEditorWidgetType;
using animatedwidgets::styleeditor::StyleWidgetTypeBuilder;
using animatedwidgets::styleeditor::StyleWidgetTypeOptions;

QString ButtonStylePlugin::pluginId() const {
    return QStringLiteral("animatedwidgets.buttons");
}

QString ButtonStylePlugin::pluginName() const {
    return QStringLiteral("Animated Widgets Buttons");
}

QList<StyleEditorWidgetType> ButtonStylePlugin::widgetTypes() const {
    const QString category = QStringLiteral("Buttons");

    StyleWidgetTypeOptions filled;
    filled.id = QStringLiteral("filledbutton");
    filled.name = QStringLiteral("Filled Button");
    filled.category = category;
    filled.suggestedSize = QSize(150, 42);
    filled.defineProperties = &ButtonStyleProperties::filled;

    StyleWidgetTypeOptions outline;
    outline.id = QStringLiteral("outlinebutton");
    outline.name = QStringLiteral("Outline Button");
    outline.category = category;
    outline.suggestedSize = QSize(150, 42);
    outline.defineProperties = &ButtonStyleProperties::outline;

    StyleWidgetTypeOptions bordered;
    bordered.id = QStringLiteral("borderedbutton");
    bordered.name = QStringLiteral("Bordered Button");
    bordered.category = category;
    bordered.suggestedSize = QSize(150, 42);
    bordered.defineProperties = &ButtonStyleProperties::bordered;

    StyleWidgetTypeOptions hover;
    hover.id = QStringLiteral("hoverbutton");
    hover.name = QStringLiteral("Hover Button");
    hover.category = category;
    hover.suggestedSize = QSize(150, 42);
    hover.defineProperties = &ButtonStyleProperties::hover;

    StyleWidgetTypeOptions filledIcon;
    filledIcon.id = QStringLiteral("fillediconbutton");
    filledIcon.name = QStringLiteral("Filled Icon Button");
    filledIcon.category = category;
    filledIcon.suggestedSize = QSize(48, 48);
    filledIcon.defineProperties = &ButtonStyleProperties::filledIcon;

    StyleWidgetTypeOptions outlineIcon;
    outlineIcon.id = QStringLiteral("outlineiconbutton");
    outlineIcon.name = QStringLiteral("Outline Icon Button");
    outlineIcon.category = category;
    outlineIcon.suggestedSize = QSize(48, 48);
    outlineIcon.defineProperties = &ButtonStyleProperties::outlineIcon;

    StyleWidgetTypeOptions borderedIcon;
    borderedIcon.id = QStringLiteral("borderediconbutton");
    borderedIcon.name = QStringLiteral("Bordered Icon Button");
    borderedIcon.category = category;
    borderedIcon.suggestedSize = QSize(48, 48);
    borderedIcon.defineProperties = &ButtonStyleProperties::borderedIcon;

    StyleWidgetTypeOptions hoverIcon;
    hoverIcon.id = QStringLiteral("hovericonbutton");
    hoverIcon.name = QStringLiteral("Hover Icon Button");
    hoverIcon.category = category;
    hoverIcon.suggestedSize = QSize(48, 48);
    hoverIcon.defineProperties = &ButtonStyleProperties::hoverIcon;

    QList<StyleEditorWidgetType> types = {
        StyleWidgetTypeBuilder::create<FilledButton>(filled),
        StyleWidgetTypeBuilder::create<OutlineButton>(outline),
        StyleWidgetTypeBuilder::create<BorderedButton>(bordered),
        StyleWidgetTypeBuilder::create<HoverButton>(hover)
    };

    for (StyleEditorWidgetType& type : types) {
        const auto create = type.create;
        type.create = [create](QWidget* parent) {
            QWidget* button = create(parent);
            button->setProperty("text", QStringLiteral("Preview"));
            return button;
        };
    }

    types.append({
        StyleWidgetTypeBuilder::create<FilledIconButton>(filledIcon),
        StyleWidgetTypeBuilder::create<OutlineIconButton>(outlineIcon),
        StyleWidgetTypeBuilder::create<BorderedIconButton>(borderedIcon),
        StyleWidgetTypeBuilder::create<HoverIconButton>(hoverIcon)
    });
    return types;
}
