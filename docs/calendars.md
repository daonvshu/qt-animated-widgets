# Calendars

Calendar components live in `animatedwidgets/calendars`, independently of the
input controls. Their classes belong to the `animatedwidgets` namespace.

- `CalendarDateSelector`: a Monday-first, six-week date grid with month/year
  navigation, today's outline, selected dates and range highlighting.
- `CalendarYearSelector`: a decade page with year navigation.
- `CalendarMonthSelector`: a twelve-month grid.
- `CalendarTimeSelector`: hour, minute and second lists with pixel scrolling.
- `CalendarDateSelectPopup`, `CalendarDateRangeSelectPopup` and
  `CalendarDateTimeSelectPopup`: standalone date, two-month range and date/time
  popups. Year and month popups are also available separately.

The components contain no form-field bindings or application filtering logic.
Navigation buttons reuse `HoverIconButton`; text actions reuse `HoverButton`
and `FilledButton`. SVG resources load through `calendars.qrc`. Time lists use
the optional smooth-scroll integration when available.

```cpp
auto* popup = new animatedwidgets::CalendarDateSelectPopup(owner);
popup->setSelectedDate(QDate::currentDate());
QObject::connect(popup, &animatedwidgets::CalendarDateSelectPopup::dateSelected,
                 owner, [popup](const QDate& date) {
    // Apply the selected date in the application.
    Q_UNUSED(date);
    popup->close();
});
popup->moveToTargetBottom(button);
popup->show();
```

`BasePopupDlg` owns popup placement, shadow and animation behavior. Popups
delete themselves after closing; retain them with `QPointer` when necessary.
Date and range popups emit activation signals and leave closing to their
caller. The date/time popup emits its value and closes after Apply.

Use `setRangeMode(true)` and `setSelectedRange(from, to)` for a standalone
date-range grid. `dateRangeSelected` can report an incomplete range after the
first click; `CalendarDateRangeSelectPopup::dateRangeSelected` reports only
complete ranges. Earlier-month/later-month cells are visible but not selectable.
Date and year/month setters initialize controls without activating a selection.
`CalendarTimeSelector::setCurrentTime()` emits `timeChanged` when the value
changes. `setTimeFormat("HH:mm")` hides seconds; setting `"HH:mm:ss"` restores them.

The standalone date selector exposes `yearClicked` and `monthClicked` so its
host can open the supplied year/month popups or provide another selection UI.
Date, range and date/time popups already connect these actions internally.
The gallery's Calendars page demonstrates both standalone selectors and popups.
Designer exports the four standalone selectors in the `animated-widgets` group.

## Appearance

All selectors and calendar popups expose `calendarStyle()` and
`setCalendarStyle(const CalendarStyle&)`. `CalendarStyle` is a value configuration:
its default constructor supplies the default appearance, and setting it copies
the configuration into the control. To change it later, edit a copy and call
`setCalendarStyle()` again. Date, range and date/time popups pass their style to
the year/month popups they open.

The configuration groups selector background/border/radius, text and fonts,
date hover/selection/range/today colors, weekday colors, month/year button states,
navigation icon/button colors and sizes, action colors, separators, time item colors/height/
padding/radius, scrollbar appearance, and popup border/background/radius/shadow/
arrow settings. `scrollbarStyleSheet` can replace the generated scrollbar rules.
Popup radius applies to both the stylesheet body and the shadow shape.

```cpp
auto style = popup->calendarStyle();
style.popupBackground = QColor("#f8fafc");
style.popupCornerRadius = 12;
style.popupBorderVisible = true;
style.popupBorder = QColor("#d5dae0");
style.dateSelectedBackground = QColor("#16856b");
style.dateRangeBackground = QColor("#dff5eb");
style.todayBorder = style.dateSelectedBackground;
popup->setCalendarStyle(style);
```

The gallery range selector and range popup demonstrate a customized green theme.
Standalone selector hosts should pass `selector->calendarStyle()` to any calendar
popup they create themselves.

Text is intentionally customizable by the host application. `CalendarDateSelector`
accepts seven Monday-first labels through `setWeekdayLabels()` and separate year
and month title formatters. `CalendarTimeSelector::setTimeLabelFormatter()`
controls the selected time title, while
`CalendarDateTimeSelectPopup::setDateTimeLabelFormatter()` controls its summary
text. Passing an empty formatter restores the built-in numeric/ISO formatting.

```cpp
dateSelector->setWeekdayLabels({"一", "二", "三", "四", "五", "六", "日"});
dateSelector->setYearLabelFormatter([](const QDate& date) {
    return QStringLiteral("%1年").arg(date.year());
});
dateSelector->setMonthLabelFormatter([](const QDate& date) {
    return QStringLiteral("%1月").arg(date.month());
});
```
