#include "animatedwidgets/utils/calendarstyleutils.h"
#include "animatedwidgets/calendars/calendarstyle.h"
#include "animatedwidgets/buttons/filledbutton.h"
#include "animatedwidgets/buttons/hoverbutton.h"
#include "animatedwidgets/buttons/hovericonbutton.h"

#include <QFrame>
#include <QLabel>
#include <QPushButton>

namespace animatedwidgets {

void CalendarStyleUtils::applyChildren(QWidget* widget, const CalendarStyle& style) {
    for (auto* line : widget->findChildren<QFrame*>()) {
        if (line->objectName() != QStringLiteral("calendarSeparator"))
            continue;
        const bool vertical = line->property("calendarVerticalSeparator").toBool();
        line->setVisible(style.separatorVisible);
        if (vertical)
            line->setFixedWidth(qMax(0, style.separatorWidth));
        else
            line->setFixedHeight(qMax(0, style.separatorWidth));
        line->setStyleSheet(QStringLiteral("border:none;background:%1;").arg(style.separator.name(QColor::HexArgb)));
    }

    for (auto* label : widget->findChildren<QLabel*>()) {
        label->setFont(label->objectName() == QStringLiteral("calendarValueLabel") ? style.itemFont : style.titleFont);
        label->setStyleSheet(QStringLiteral("QLabel{color:%1;background:transparent;}QLabel:disabled{color:%2;}")
            .arg(style.content.name(QColor::HexArgb), style.disabledContent.name(QColor::HexArgb)));
    }

    for (auto* button : widget->findChildren<QPushButton*>()) {
        if (auto* navigation = qobject_cast<HoverIconButton*>(button)) {
            navigation->setFixedSize(style.navigationButtonSize);
            navigation->setIconSize(style.navigationIconSize);
            navigation->setAnimateContentColor(true);
            navigation->setContentNormal(style.navigationContent);
            navigation->setContentHover(style.navigationHoverContent);
            navigation->setContentPressed(style.navigationHoverContent);
            navigation->setContentDisabled(style.disabledContent);
            navigation->setBackgroundHover(style.buttonHoverBackground);
            navigation->setBackgroundPressed(style.buttonPressedBackground);
            navigation->setCornerRadius(style.buttonCornerRadius);
        } else if (auto* action = qobject_cast<HoverButton*>(button)) {
            const bool dateTitle = action->objectName() == QStringLiteral("calendarDateTitleButton");
            action->setFont(dateTitle ? style.dateTitleFont : style.titleFont);
            action->setContentNormal(style.content);
            action->setContentDisabled(style.disabledContent);
            action->setBackgroundHover(style.buttonHoverBackground);
            action->setBackgroundPressed(style.buttonPressedBackground);
            action->setCornerRadius(style.buttonCornerRadius);
            if (dateTitle) {
                if (style.dateTitleButtonSize.width() > 0)
                    action->setFixedWidth(style.dateTitleButtonSize.width());
                if (style.dateTitleButtonSize.height() > 0)
                    action->setFixedHeight(style.dateTitleButtonSize.height());
            }
        } else if (auto* confirm = qobject_cast<FilledButton*>(button)) {
            confirm->setFont(style.itemFont);
            confirm->setBackgroundNormal(style.actionBackground);
            confirm->setContentNormal(style.actionContent);
            confirm->setCornerRadius(style.buttonCornerRadius);
        } else {
            button->setFont(style.itemFont);
            button->setStyleSheet(style.buttonStyleSheet());
        }
    }
}

} // namespace animatedwidgets
