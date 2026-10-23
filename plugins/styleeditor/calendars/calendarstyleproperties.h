#pragma once

#include "../utils/styleobjectproperties.h"
#include "animatedwidgets/calendars/calendarstyle.h"

class CalendarStyleProperties {
public:
    static void calendarDateSelector(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void calendarMonthSelector(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void calendarYearSelector(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void calendarTimeSelector(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void calendarDateSelectPopup(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void calendarDateTimeSelectPopup(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void calendarDateRangeSelectPopup(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void calendarMonthSelectPopup(animatedwidgets::styleeditor::StylePropertyBuilder& builder);
    static void calendarYearSelectPopup(animatedwidgets::styleeditor::StylePropertyBuilder& builder);

private:
    template <typename Widget, typename Value>
    static void member(animatedwidgets::styleeditor::StylePropertyBuilder& builder, const QString& group, const QString& name, const QString& label,
                       Value animatedwidgets::CalendarStyle::* field) {
        StyleObjectProperties::add<Widget>(
                builder, group, QStringLiteral("calendar.") + name, label, field, [](Widget* widget) { return widget->calendarStyle(); },
                [](Widget* widget, const animatedwidgets::CalendarStyle& style) { widget->setCalendarStyle(style); }, QStringLiteral("widget->calendarStyle()"),
                QStringLiteral("setCalendarStyle(style)"));
    }

    template <typename Widget>
    static void font(animatedwidgets::styleeditor::StylePropertyBuilder& builder, const QString& group, const QString& name,
                     QFont animatedwidgets::CalendarStyle::* field) {
        StyleObjectProperties::font<Widget, animatedwidgets::CalendarStyle>(
                builder, group, QStringLiteral("calendar.") + name, field, [](Widget* widget) { return widget->calendarStyle(); },
                [](Widget* widget, const animatedwidgets::CalendarStyle& style) { widget->setCalendarStyle(style); }, QStringLiteral("widget->calendarStyle()"),
                QStringLiteral("setCalendarStyle(style)"));
    }

    template <typename Widget>
    static void padding(animatedwidgets::styleeditor::StylePropertyBuilder& builder, const QString& group, const QString& name,
                        QMargins animatedwidgets::CalendarStyle::* field) {
        StyleObjectProperties::margins<Widget, animatedwidgets::CalendarStyle>(
                builder, group, QStringLiteral("calendar.") + name, field, [](Widget* widget) { return widget->calendarStyle(); },
                [](Widget* widget, const animatedwidgets::CalendarStyle& style) { widget->setCalendarStyle(style); }, QStringLiteral("widget->calendarStyle()"),
                QStringLiteral("setCalendarStyle(style)"));
    }

    template <typename Widget>
    static void appearance(animatedwidgets::styleeditor::StylePropertyBuilder& builder) {
        using animatedwidgets::CalendarStyle;
        member<Widget>(builder, QStringLiteral("Appearance"), QStringLiteral("background"), QStringLiteral("Background"), &CalendarStyle::background);
        member<Widget>(builder, QStringLiteral("Appearance"), QStringLiteral("border"), QStringLiteral("Border"), &CalendarStyle::border);
        member<Widget>(builder, QStringLiteral("Appearance"), QStringLiteral("borderVisible"), QStringLiteral("Border Visible"), &CalendarStyle::borderVisible);
        member<Widget>(builder, QStringLiteral("Appearance"), QStringLiteral("borderWidth"), QStringLiteral("Border Width"), &CalendarStyle::borderWidth);
        member<Widget>(builder, QStringLiteral("Appearance"), QStringLiteral("cornerRadius"), QStringLiteral("Corner Radius"), &CalendarStyle::cornerRadius);
        member<Widget>(builder, QStringLiteral("Appearance"), QStringLiteral("content"), QStringLiteral("Content"), &CalendarStyle::content);
        member<Widget>(builder, QStringLiteral("Appearance"), QStringLiteral("disabledContent"), QStringLiteral("Disabled Content"),
                       &CalendarStyle::disabledContent);
        member<Widget>(builder, QStringLiteral("Appearance"), QStringLiteral("separator"), QStringLiteral("Separator"), &CalendarStyle::separator);
        member<Widget>(builder, QStringLiteral("Appearance"), QStringLiteral("separatorWidth"), QStringLiteral("Separator Width"),
                       &CalendarStyle::separatorWidth);
        member<Widget>(builder, QStringLiteral("Appearance"), QStringLiteral("separatorVisible"), QStringLiteral("Separator Visible"),
                       &CalendarStyle::separatorVisible);
        font<Widget>(builder, QStringLiteral("Item font"), QStringLiteral("itemFont"), &CalendarStyle::itemFont);
        font<Widget>(builder, QStringLiteral("Title font"), QStringLiteral("titleFont"), &CalendarStyle::titleFont);
        builder.enabledWhen(QStringLiteral("calendar.borderWidth"), QStringLiteral("calendar.borderVisible"));
        builder.enabledWhen(QStringLiteral("calendar.border"), QStringLiteral("calendar.borderVisible"));
    }

    template <typename Widget>
    static void navigation(animatedwidgets::styleeditor::StylePropertyBuilder& builder) {
        using animatedwidgets::CalendarStyle;
        member<Widget>(builder, QStringLiteral("Navigation"), QStringLiteral("buttonBackground"), QStringLiteral("Button Background"),
                       &CalendarStyle::buttonBackground);
        member<Widget>(builder, QStringLiteral("Navigation"), QStringLiteral("buttonHoverBackground"), QStringLiteral("Button Hover Background"),
                       &CalendarStyle::buttonHoverBackground);
        member<Widget>(builder, QStringLiteral("Navigation"), QStringLiteral("buttonPressedBackground"), QStringLiteral("Button Pressed Background"),
                       &CalendarStyle::buttonPressedBackground);
        member<Widget>(builder, QStringLiteral("Navigation"), QStringLiteral("buttonSelectedBackground"), QStringLiteral("Button Selected Background"),
                       &CalendarStyle::buttonSelectedBackground);
        member<Widget>(builder, QStringLiteral("Navigation"), QStringLiteral("buttonSelectedContent"), QStringLiteral("Button Selected Content"),
                       &CalendarStyle::buttonSelectedContent);
        member<Widget>(builder, QStringLiteral("Navigation"), QStringLiteral("buttonBorder"), QStringLiteral("Button Border"), &CalendarStyle::buttonBorder);
        member<Widget>(builder, QStringLiteral("Navigation"), QStringLiteral("buttonBorderWidth"), QStringLiteral("Button Border Width"),
                       &CalendarStyle::buttonBorderWidth);
        member<Widget>(builder, QStringLiteral("Navigation"), QStringLiteral("buttonCornerRadius"), QStringLiteral("Button Corner Radius"),
                       &CalendarStyle::buttonCornerRadius);
        member<Widget>(builder, QStringLiteral("Navigation"), QStringLiteral("navigationContent"), QStringLiteral("Navigation Content"),
                       &CalendarStyle::navigationContent);
        member<Widget>(builder, QStringLiteral("Navigation"), QStringLiteral("navigationHoverContent"), QStringLiteral("Navigation Hover Content"),
                       &CalendarStyle::navigationHoverContent);
        member<Widget>(builder, QStringLiteral("Navigation"), QStringLiteral("navigationIconSize"), QStringLiteral("Navigation Icon Size"),
                       &CalendarStyle::navigationIconSize);
        member<Widget>(builder, QStringLiteral("Navigation"), QStringLiteral("navigationButtonSize"), QStringLiteral("Navigation Button Size"),
                       &CalendarStyle::navigationButtonSize);
        member<Widget>(builder, QStringLiteral("Navigation"), QStringLiteral("dateTitleButtonSize"), QStringLiteral("Date Title Button Size"),
                       &CalendarStyle::dateTitleButtonSize);
        member<Widget>(builder, QStringLiteral("Navigation"), QStringLiteral("iconHint"), QStringLiteral("Icon Hint"), &CalendarStyle::iconHint);
        member<Widget>(builder, QStringLiteral("Navigation"), QStringLiteral("actionBackground"), QStringLiteral("Action Background"),
                       &CalendarStyle::actionBackground);
        member<Widget>(builder, QStringLiteral("Navigation"), QStringLiteral("actionContent"), QStringLiteral("Action Content"), &CalendarStyle::actionContent);
        font<Widget>(builder, QStringLiteral("Date title font"), QStringLiteral("dateTitleFont"), &CalendarStyle::dateTitleFont);
    }

    template <typename Widget>
    static void dateCells(animatedwidgets::styleeditor::StylePropertyBuilder& builder) {
        using animatedwidgets::CalendarStyle;
        member<Widget>(builder, QStringLiteral("Date cells"), QStringLiteral("adjacentMonthContent"), QStringLiteral("Adjacent Month Content"),
                       &CalendarStyle::adjacentMonthContent);
        member<Widget>(builder, QStringLiteral("Date cells"), QStringLiteral("dateHoverBackground"), QStringLiteral("Date Hover Background"),
                       &CalendarStyle::dateHoverBackground);
        member<Widget>(builder, QStringLiteral("Date cells"), QStringLiteral("dateSelectedBackground"), QStringLiteral("Date Selected Background"),
                       &CalendarStyle::dateSelectedBackground);
        member<Widget>(builder, QStringLiteral("Date cells"), QStringLiteral("dateSelectedContent"), QStringLiteral("Date Selected Content"),
                       &CalendarStyle::dateSelectedContent);
        member<Widget>(builder, QStringLiteral("Date cells"), QStringLiteral("dateRangeBackground"), QStringLiteral("Date Range Background"),
                       &CalendarStyle::dateRangeBackground);
        member<Widget>(builder, QStringLiteral("Date cells"), QStringLiteral("todayBorder"), QStringLiteral("Today Border"), &CalendarStyle::todayBorder);
        member<Widget>(builder, QStringLiteral("Date cells"), QStringLiteral("todayBorderWidth"), QStringLiteral("Today Border Width"),
                       &CalendarStyle::todayBorderWidth);
        member<Widget>(builder, QStringLiteral("Date cells"), QStringLiteral("dateCornerRadius"), QStringLiteral("Date Corner Radius"),
                       &CalendarStyle::dateCornerRadius);
        member<Widget>(builder, QStringLiteral("Date cells"), QStringLiteral("dateBoxSize"), QStringLiteral("Date Box Size"), &CalendarStyle::dateBoxSize);
        member<Widget>(builder, QStringLiteral("Date cells"), QStringLiteral("weekdayBackground"), QStringLiteral("Weekday Background"),
                       &CalendarStyle::weekdayBackground);
        member<Widget>(builder, QStringLiteral("Date cells"), QStringLiteral("weekdayContent"), QStringLiteral("Weekday Content"),
                       &CalendarStyle::weekdayContent);
        padding<Widget>(builder, QStringLiteral("Date padding"), QStringLiteral("dateContentPadding"), &CalendarStyle::dateContentPadding);
    }

    template <typename Widget>
    static void timeCells(animatedwidgets::styleeditor::StylePropertyBuilder& builder) {
        using animatedwidgets::CalendarStyle;
        member<Widget>(builder, QStringLiteral("Time cells"), QStringLiteral("timeBackground"), QStringLiteral("Time Background"),
                       &CalendarStyle::timeBackground);
        member<Widget>(builder, QStringLiteral("Time cells"), QStringLiteral("timeHoverBackground"), QStringLiteral("Time Hover Background"),
                       &CalendarStyle::timeHoverBackground);
        member<Widget>(builder, QStringLiteral("Time cells"), QStringLiteral("timeSelectedBackground"), QStringLiteral("Time Selected Background"),
                       &CalendarStyle::timeSelectedBackground);
        member<Widget>(builder, QStringLiteral("Time cells"), QStringLiteral("timeSelectedContent"), QStringLiteral("Time Selected Content"),
                       &CalendarStyle::timeSelectedContent);
        member<Widget>(builder, QStringLiteral("Time cells"), QStringLiteral("timeCornerRadius"), QStringLiteral("Time Corner Radius"),
                       &CalendarStyle::timeCornerRadius);
        member<Widget>(builder, QStringLiteral("Time cells"), QStringLiteral("timeItemHeight"), QStringLiteral("Time Item Height"),
                       &CalendarStyle::timeItemHeight);
        member<Widget>(builder, QStringLiteral("Time cells"), QStringLiteral("scrollbarHandle"), QStringLiteral("Scrollbar Handle"),
                       &CalendarStyle::scrollbarHandle);
        member<Widget>(builder, QStringLiteral("Time cells"), QStringLiteral("scrollbarWidth"), QStringLiteral("Scrollbar Width"),
                       &CalendarStyle::scrollbarWidth);
        member<Widget>(builder, QStringLiteral("Time cells"), QStringLiteral("scrollbarCornerRadius"), QStringLiteral("Scrollbar Corner Radius"),
                       &CalendarStyle::scrollbarCornerRadius);
        member<Widget>(builder, QStringLiteral("Time cells"), QStringLiteral("scrollbarMinimumHandleHeight"), QStringLiteral("Scrollbar Minimum Handle Height"),
                       &CalendarStyle::scrollbarMinimumHandleHeight);
        member<Widget>(builder, QStringLiteral("Time cells"), QStringLiteral("scrollbarHandlePadding"), QStringLiteral("Scrollbar Handle Padding"),
                       &CalendarStyle::scrollbarHandlePadding);
        member<Widget>(builder, QStringLiteral("Time cells"), QStringLiteral("scrollbarStyleSheet"), QStringLiteral("Scrollbar Style Sheet"),
                       &CalendarStyle::scrollbarStyleSheet);
        padding<Widget>(builder, QStringLiteral("Time padding"), QStringLiteral("timeContentPadding"), &CalendarStyle::timeContentPadding);
    }

    template <typename Widget>
    static void popup(animatedwidgets::styleeditor::StylePropertyBuilder& builder) {
        using animatedwidgets::CalendarStyle;
        member<Widget>(builder, QStringLiteral("Popup"), QStringLiteral("popupBackground"), QStringLiteral("Popup Background"),
                       &CalendarStyle::popupBackground);
        member<Widget>(builder, QStringLiteral("Popup"), QStringLiteral("popupBorder"), QStringLiteral("Popup Border"), &CalendarStyle::popupBorder);
        member<Widget>(builder, QStringLiteral("Popup"), QStringLiteral("popupBorderWidth"), QStringLiteral("Popup Border Width"),
                       &CalendarStyle::popupBorderWidth);
        member<Widget>(builder, QStringLiteral("Popup"), QStringLiteral("popupBorderVisible"), QStringLiteral("Popup Border Visible"),
                       &CalendarStyle::popupBorderVisible);
        member<Widget>(builder, QStringLiteral("Popup"), QStringLiteral("popupCornerRadius"), QStringLiteral("Popup Corner Radius"),
                       &CalendarStyle::popupCornerRadius);
        member<Widget>(builder, QStringLiteral("Popup"), QStringLiteral("shadowVisible"), QStringLiteral("Shadow Visible"), &CalendarStyle::shadowVisible);
        member<Widget>(builder, QStringLiteral("Popup"), QStringLiteral("shadowColor"), QStringLiteral("Shadow Color"), &CalendarStyle::shadowColor);
        member<Widget>(builder, QStringLiteral("Popup"), QStringLiteral("shadowRadius"), QStringLiteral("Shadow Radius"), &CalendarStyle::shadowRadius);
        member<Widget>(builder, QStringLiteral("Popup"), QStringLiteral("arrowVisible"), QStringLiteral("Arrow Visible"), &CalendarStyle::arrowVisible);
        builder.enabledWhen(QStringLiteral("calendar.popupBorder"), QStringLiteral("calendar.popupBorderVisible"));
        builder.enabledWhen(QStringLiteral("calendar.popupBorderWidth"), QStringLiteral("calendar.popupBorderVisible"));
        builder.enabledWhen(QStringLiteral("calendar.shadowColor"), QStringLiteral("calendar.shadowVisible"));
        builder.enabledWhen(QStringLiteral("calendar.shadowRadius"), QStringLiteral("calendar.shadowVisible"));
    }
};
