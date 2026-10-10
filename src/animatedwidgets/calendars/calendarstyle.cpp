#include "animatedwidgets/calendars/calendarstyle.h"

namespace animatedwidgets {

CalendarStyle::CalendarStyle()
{
    itemFont.setPixelSize(14);
    titleFont.setPixelSize(14);
    titleFont.setWeight(QFont::Medium);
    dateTitleFont = titleFont;
    dateTitleFont.setWeight(QFont::Bold);
}

QString CalendarStyle::buttonStyleSheet() const {
    return QStringLiteral("QPushButton{border:%1px solid %2;border-radius:%3px;color:%4;background:%5;}"
                          "QPushButton:hover{background:%6;}QPushButton:pressed{background:%7;}"
                          "QPushButton:checked{color:%8;background:%9;}QPushButton:disabled{color:%10;}")
        .arg(qMax(0, buttonBorderWidth)).arg(buttonBorder.name(QColor::HexArgb)).arg(qMax(0, buttonCornerRadius))
        .arg(content.name(QColor::HexArgb), buttonBackground.name(QColor::HexArgb),
             buttonHoverBackground.name(QColor::HexArgb), buttonPressedBackground.name(QColor::HexArgb),
             buttonSelectedContent.name(QColor::HexArgb), buttonSelectedBackground.name(QColor::HexArgb),
             disabledContent.name(QColor::HexArgb));
}

QString CalendarStyle::selectorStyleSheet() const {
    return QStringLiteral("QWidget[calendarSelector=\"true\"]{background:%1;border:%2px solid %3;border-radius:%4px;}")
        .arg(background.name(QColor::HexArgb)).arg(borderVisible ? qMax(0, borderWidth) : 0)
        .arg(border.name(QColor::HexArgb)).arg(qMax(0, cornerRadius));
}

QString CalendarStyle::dateStyleSheet() const {
    return QStringLiteral("QTableView{border:none;background:%1;padding:%2px %3px %4px %5px;}"
                          "QHeaderView::section{border:none;color:%6;background:%7;}")
        .arg(background.name(QColor::HexArgb)).arg(qMax(0, dateContentPadding.top()))
        .arg(qMax(0, dateContentPadding.right())).arg(qMax(0, dateContentPadding.bottom()))
        .arg(qMax(0, dateContentPadding.left()))
        .arg(weekdayContent.name(QColor::HexArgb), weekdayBackground.name(QColor::HexArgb));
}

QString CalendarStyle::timeStyleSheet() const {
    const QString scrollbar = scrollbarStyleSheet.isEmpty()
        ? QStringLiteral("QScrollBar:vertical{border:none;background:transparent;width:%1px;margin:0px;}"
                         "QScrollBar::handle:vertical{background:%2;border-radius:%3px;min-height:%4px;margin:0px %5px;}"
                         "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background:transparent;}"
                         "QScrollBar::add-line:vertical,QScrollBar::sub-line:vertical{height:0px;}")
            .arg(qMax(0, scrollbarWidth)).arg(scrollbarHandle.name(QColor::HexArgb)).arg(qMax(0, scrollbarCornerRadius))
            .arg(qMax(0, scrollbarMinimumHandleHeight)).arg(qMax(0, scrollbarHandlePadding))
        : scrollbarStyleSheet;
    return QStringLiteral("QListView{border:none;background:%1;}").arg(timeBackground.name(QColor::HexArgb)) + scrollbar;
}

QString CalendarStyle::popupStyleSheet(const QString& objectName) const {
    return QStringLiteral("#%1{background:%2;border:%3px solid %4;border-radius:%5px;}")
        .arg(objectName, popupBackground.name(QColor::HexArgb)).arg(popupBorderVisible ? qMax(0, popupBorderWidth) : 0)
        .arg(popupBorder.name(QColor::HexArgb)).arg(qMax(0, popupCornerRadius));
}

} // namespace animatedwidgets
