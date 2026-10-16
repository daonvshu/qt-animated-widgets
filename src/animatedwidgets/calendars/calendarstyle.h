#pragma once

#include <QColor>
#include <QFont>
#include <QMargins>
#include <QSize>
#include <QString>

namespace animatedwidgets {

/** @brief Calendar appearance configuration; default construction provides the default theme. */
class CalendarStyle {
public:
    CalendarStyle();

    /** Background, border and radius of a standalone selector. */
    QColor background = Qt::transparent; /**< Selector background. */
    QColor border = QColor(0xEDEEF0); /**< Selector border color. */
    bool borderVisible = false; /**< Whether the selector border is drawn. */
    int borderWidth = 1; /**< Selector border width in pixels. */
    int cornerRadius = 0; /**< Selector corner radius in pixels. */

    /** Common text colors and fonts. */
    QColor content = QColor(0x212329); /**< Normal content color. */
    QColor disabledContent = QColor(0xBFC1C7); /**< Disabled content color. */
    QColor adjacentMonthContent = QColor(0xBFC1C7); /**< Color for dates outside the displayed month. */
    QFont itemFont; /**< Font used by date and time items. */
    QFont titleFont; /**< Font used by calendar titles and actions. */
    QFont dateTitleFont; /**< Font used by the year and month title buttons. */

    /** Divider appearance. */
    QColor separator = QColor(0xEDEEF0); /**< Separator color. */
    int separatorWidth = 1; /**< Separator width in pixels. */
    bool separatorVisible = true; /**< Whether separators are visible. */

    /** Date-grid states and geometry. */
    QColor dateHoverBackground = QColor(0xF5F5F5); /**< Date hover background. */
    QColor dateSelectedBackground = QColor(0x1778FF); /**< Selected date background. */
    QColor dateSelectedContent = Qt::white; /**< Selected date text color. */
    QColor dateRangeBackground = QColor(0xE3F3FF); /**< Background spanning a selected range. */
    QColor todayBorder = QColor(0x1778FF); /**< Today's outline color. */
    int todayBorderWidth = 1; /**< Today's outline width in pixels. */
    int dateCornerRadius = 2; /**< Date cell corner radius in pixels. */
    QSize dateBoxSize = QSize(24, 24); /**< Date selection box size in pixels. */
    QColor weekdayBackground = Qt::white; /**< Weekday header background. */
    QColor weekdayContent = QColor(0x212329); /**< Weekday header text color. */
    QMargins dateContentPadding = QMargins(12, 8, 12, 8); /**< Date table content padding in pixels. */

    /** Month/year button and navigation appearance. */
    QColor buttonBackground = Qt::transparent; /**< Month/year button background. */
    QColor buttonHoverBackground = QColor(0xF5F5F5); /**< Month/year button hover background. */
    QColor buttonPressedBackground = QColor(0xE3F3FF); /**< Month/year button pressed background. */
    QColor buttonSelectedBackground = QColor(0x1778FF); /**< Month/year button selected background. */
    QColor buttonSelectedContent = Qt::white; /**< Month/year button selected text color. */
    QColor buttonBorder = Qt::transparent; /**< Month/year button border color. */
    int buttonBorderWidth = 0; /**< Month/year button border width in pixels. */
    int buttonCornerRadius = 6; /**< Month/year button corner radius in pixels. */
    QColor navigationContent = QColor(0xBFC1C7); /**< Navigation icon color. */
    QColor navigationHoverContent = QColor(0x1778FF); /**< Navigation icon hover color. */
    QSize navigationIconSize = QSize(16, 16); /**< Navigation icon size in pixels. */
    bool iconHint = false; /**< Tint button icons with their content colors; false preserves original icon colors. */
    QSize navigationButtonSize = QSize(28, 28); /**< Navigation button size in pixels. */
    QSize dateTitleButtonSize = QSize(0, 28); /**< Year/month title button size; zero width keeps automatic sizing. */
    QColor actionBackground = QColor(0x1778FF); /**< Action button background. */
    QColor actionContent = Qt::white; /**< Action button text color. */

    /** Time-list item and scrollbar appearance. */
    QColor timeBackground = Qt::transparent; /**< Time-list background. */
    QColor timeHoverBackground = QColor(0xF2F3F5); /**< Time-item hover background. */
    QColor timeSelectedBackground = QColor(0xE3F3FF); /**< Time-item selected background. */
    QColor timeSelectedContent = QColor(0x212329); /**< Time-item selected text color. */
    int timeCornerRadius = 4; /**< Time-item corner radius in pixels. */
    int timeItemHeight = 32; /**< Time-item height in pixels. */
    QMargins timeContentPadding = QMargins(4, 2, 4, 2); /**< Time-item content padding in pixels. */
    QColor scrollbarHandle = QColor(0xBFC1C7); /**< Calendar scrollbar handle color. */
    int scrollbarWidth = 10; /**< Calendar scrollbar width in pixels. */
    int scrollbarCornerRadius = 3; /**< Calendar scrollbar handle corner radius. */
    int scrollbarMinimumHandleHeight = 40; /**< Calendar scrollbar minimum handle height. */
    int scrollbarHandlePadding = 2; /**< Calendar scrollbar handle side padding. */
    /** Custom scrollbar stylesheet; an empty value uses the generated rules. */
    QString scrollbarStyleSheet;

    /** Popup body, shadow and arrow appearance. */
    QColor popupBackground = Qt::white; /**< Popup body background. */
    QColor popupBorder = QColor(0xEDEEF0); /**< Popup border color. */
    int popupBorderWidth = 1; /**< Popup border width in pixels. */
    bool popupBorderVisible = false; /**< Whether the popup border is drawn. */
    int popupCornerRadius = 8; /**< Popup body and shadow corner radius. */
    bool shadowVisible = true; /**< Whether the popup shadow is visible. */
    QColor shadowColor = QColor(0xA9ACB6); /**< Popup shadow color. */
    int shadowRadius = 12; /**< Popup shadow blur radius. */
    bool arrowVisible = true; /**< Whether the popup arrow is visible. */

    /** @brief Generates stylesheets used by the calendar components. */
    QString buttonStyleSheet() const;
    QString selectorStyleSheet() const;
    QString dateStyleSheet() const;
    QString timeStyleSheet() const;
    QString popupStyleSheet(const QString& objectName) const;
};

} // namespace animatedwidgets
