cmake_minimum_required(VERSION 3.21)

project(AnimatedWidgets VERSION 0.1.0 LANGUAGES CXX)

option(BUILD_EXAMPLES "Build example applications" ON)
option(BUILD_TESTS "Build unit tests" ON)
option(BUILD_DESIGNER_PLUGIN "Build the Qt Designer widget plugin" ON)
option(ANIMATEDWIDGETS_ENABLE_INSTALL "Enable install and package export rules" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets Svg)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Svg)
find_package(SmoothScrollbar QUIET CONFIG)

add_library(AnimatedWidgetsCore
    src/animatedwidgets/core/animationspec.h
    src/animatedwidgets/core/animationcontroller.h
    src/animatedwidgets/core/animationcontroller.cpp
    src/animatedwidgets/core/valuetransition.h
    src/animatedwidgets/core/valuetransition.cpp
)
add_library(AnimatedWidgets::Core ALIAS AnimatedWidgetsCore)
target_include_directories(AnimatedWidgetsCore PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(AnimatedWidgetsCore PUBLIC Qt${QT_VERSION_MAJOR}::Core)

add_library(AnimatedWidgetsStyle
    src/animatedwidgets/style/colorpalette.h
    src/animatedwidgets/style/widgettheme.h
    src/animatedwidgets/style/colorpalette.cpp
    src/animatedwidgets/style/widgettheme.cpp
)
add_library(AnimatedWidgets::Style ALIAS AnimatedWidgetsStyle)
target_include_directories(AnimatedWidgetsStyle PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(AnimatedWidgetsStyle PUBLIC AnimatedWidgetsCore Qt${QT_VERSION_MAJOR}::Gui)

add_library(AnimatedWidgetsWidgets
    src/animatedwidgets/calendars/calendarstyle.h
    src/animatedwidgets/calendars/calendarstyle.cpp
    src/animatedwidgets/utils/calendarstyleutils.h
    src/animatedwidgets/utils/calendarstyleutils.cpp
    src/animatedwidgets/calendars/calendardateitemdelegate.cpp
    src/animatedwidgets/calendars/calendardateitemdelegate.h
    src/animatedwidgets/calendars/calendardaterangeselectpopup.cpp
    src/animatedwidgets/calendars/calendardaterangeselectpopup.h
    src/animatedwidgets/calendars/calendardateselector.cpp
    src/animatedwidgets/calendars/calendardateselector.h
    src/animatedwidgets/calendars/calendardateselectpopup.cpp
    src/animatedwidgets/calendars/calendardateselectpopup.h
    src/animatedwidgets/calendars/calendardatetimeselectpopup.cpp
    src/animatedwidgets/calendars/calendardatetimeselectpopup.h
    src/animatedwidgets/calendars/calendarmonthselector.cpp
    src/animatedwidgets/calendars/calendarmonthselector.h
    src/animatedwidgets/calendars/calendarmonthselectpopup.cpp
    src/animatedwidgets/calendars/calendarmonthselectpopup.h
    src/animatedwidgets/calendars/calendarsectiondata.h
    src/animatedwidgets/calendars/calendartimeitemstyledelegate.cpp
    src/animatedwidgets/calendars/calendartimeitemstyledelegate.h
    src/animatedwidgets/calendars/calendartimeselector.cpp
    src/animatedwidgets/calendars/calendartimeselector.h
    src/animatedwidgets/calendars/calendaryearselector.cpp
    src/animatedwidgets/calendars/calendaryearselector.h
    src/animatedwidgets/calendars/calendaryearselectpopup.cpp
    src/animatedwidgets/calendars/calendaryearselectpopup.h
    src/animatedwidgets/calendars/resources/calendars.qrc
    src/animatedwidgets/utils/calendardateutils.h
    src/animatedwidgets/utils/calendardateutils.cpp
    src/animatedwidgets/popups/shadowwidget.h
    src/animatedwidgets/popups/shadowwidget.cpp
    src/animatedwidgets/popups/basepopupdlg.h
    src/animatedwidgets/popups/basepopupdlg.cpp
    src/animatedwidgets/popups/popupanimation.h
    src/animatedwidgets/popups/popupanimation.cpp
    src/animatedwidgets/popups/fadeslidepopupanimation.h
    src/animatedwidgets/popups/fadeslidepopupanimation.cpp
    src/animatedwidgets/popups/popupmenustyle.h
    src/animatedwidgets/popups/popupdismissfilter.h
    src/animatedwidgets/popups/popupdismissfilter.cpp
    src/animatedwidgets/popups/popupmenuitem.h
    src/animatedwidgets/popups/popupmenunode.h
    src/animatedwidgets/popups/popupmenunode.cpp
    src/animatedwidgets/popups/resources/popups.qrc
    src/animatedwidgets/popups/popupmenurowtype.h
    src/animatedwidgets/popups/popupmenugroup.h
    src/animatedwidgets/popups/popupmenufiltermodel.h
    src/animatedwidgets/popups/popupmenufiltermodel.cpp
    src/animatedwidgets/popups/searchpopupmenu.h
    src/animatedwidgets/popups/searchpopupmenu.cpp
    src/animatedwidgets/popups/defaultpopupmenumodel.h
    src/animatedwidgets/popups/defaultpopupmenumodel.cpp
    src/animatedwidgets/popups/groupedpopupmenumodel.h
    src/animatedwidgets/popups/groupedpopupmenumodel.cpp
    src/animatedwidgets/popups/popupmenuliststyle.h
    src/animatedwidgets/popups/popupmenuviewportclip.h
    src/animatedwidgets/popups/popupmenuviewportclip.cpp
    src/animatedwidgets/popups/defaultpopupmenuliststyle.h
    src/animatedwidgets/popups/defaultpopupmenuliststyle.cpp
    src/animatedwidgets/popups/defaultpopupmenustyle.h
    src/animatedwidgets/popups/defaultpopupmenustyle.cpp
    src/animatedwidgets/popups/popupmenuitemdelegate.h
    src/animatedwidgets/popups/popupmenuitemdelegate.cpp
    src/animatedwidgets/popups/popupmenu.h
    src/animatedwidgets/popups/popupmenu.cpp
    src/animatedwidgets/popups/treepopupmenu.h
    src/animatedwidgets/popups/treepopupmenu.cpp
    src/animatedwidgets/buttons/animatedbuttonbase.h
    src/animatedwidgets/buttons/animatedbuttonbase.cpp
    src/animatedwidgets/buttons/buttonstate.h
    src/animatedwidgets/buttons/buttonstatestyle.h
    src/animatedwidgets/utils/buttonstatecolors.h
    src/animatedwidgets/utils/buttonstatecolors.cpp
    src/animatedwidgets/utils/inputcontextmenu.h
    src/animatedwidgets/utils/inputcontextmenu.cpp
    src/animatedwidgets/utils/textsearchutils.h
    src/animatedwidgets/utils/textsearchutils.cpp
    src/animatedwidgets/utils/popupmenuitemdata.h
    src/animatedwidgets/utils/popupmenuresources.h
    src/animatedwidgets/utils/widgetresources.h
    src/animatedwidgets/utils/smoothscrolling.h
    src/animatedwidgets/utils/smoothscrolling.cpp
    src/animatedwidgets/utils/treemodelutils.h
    src/animatedwidgets/utils/treemodelutils.cpp
    src/animatedwidgets/utils/combocreationutils.h
    src/animatedwidgets/utils/combocreationutils.cpp
    src/animatedwidgets/utils/popupmenuitemdata.cpp
    src/animatedwidgets/utils/buttonpainter.h
    src/animatedwidgets/utils/buttonpainter.cpp
    src/animatedwidgets/utils/checkableiconpainter.h
    src/animatedwidgets/utils/checkableiconpainter.cpp
    src/animatedwidgets/buttons/checkableiconanimator.h
    src/animatedwidgets/buttons/checkableiconanimator.cpp
    src/animatedwidgets/buttons/radiobutton.h
    src/animatedwidgets/buttons/radiobutton.cpp
    src/animatedwidgets/buttons/checkbox.h
    src/animatedwidgets/buttons/checkbox.cpp
    src/animatedwidgets/buttons/filledbutton.h
    src/animatedwidgets/buttons/filledbutton.cpp
    src/animatedwidgets/buttons/outlinebutton.h
    src/animatedwidgets/buttons/outlinebutton.cpp
    src/animatedwidgets/buttons/borderedbutton.h
    src/animatedwidgets/buttons/borderedbutton.cpp
    src/animatedwidgets/buttons/hoverbutton.h
    src/animatedwidgets/buttons/hoverbutton.cpp
    src/animatedwidgets/buttons/fillediconbutton.h
    src/animatedwidgets/buttons/fillediconbutton.cpp
    src/animatedwidgets/buttons/outlineiconbutton.h
    src/animatedwidgets/buttons/outlineiconbutton.cpp
    src/animatedwidgets/buttons/borderediconbutton.h
    src/animatedwidgets/buttons/borderediconbutton.cpp
    src/animatedwidgets/buttons/hovericonbutton.h
    src/animatedwidgets/buttons/hovericonbutton.cpp
    src/animatedwidgets/buttons/variantbuttons.h
    src/animatedwidgets/switches/switch.h
    src/animatedwidgets/switches/switch.cpp
    src/animatedwidgets/progressbars/progressbar.h
    src/animatedwidgets/progressbars/progressbar.cpp
    src/animatedwidgets/sliders/slider.h
    src/animatedwidgets/sliders/slider.cpp
    src/animatedwidgets/inputs/lineedit.h
    src/animatedwidgets/inputs/lineedit.cpp
    src/animatedwidgets/inputs/compositeinputbase.h
    src/animatedwidgets/inputs/compositeinputbase.cpp
    src/animatedwidgets/inputs/plaintextedit.h
    src/animatedwidgets/inputs/plaintextedit.cpp
    src/animatedwidgets/inputs/spinboxappearance.h
    src/animatedwidgets/inputs/spinboxappearance.cpp
    src/animatedwidgets/inputs/spinbox.h
    src/animatedwidgets/inputs/spinbox.cpp
    src/animatedwidgets/inputs/doublespinbox.h
    src/animatedwidgets/inputs/doublespinbox.cpp
    src/animatedwidgets/inputs/iconlineedit.h
    src/animatedwidgets/inputs/iconlineedit.cpp
    src/animatedwidgets/inputs/passwordlineedit.h
    src/animatedwidgets/inputs/passwordlineedit.cpp
    src/animatedwidgets/inputs/compositelineedit.h
    src/animatedwidgets/inputs/compositelineedit.cpp
    src/animatedwidgets/inputs/compositeplaintextedit.h
    src/animatedwidgets/inputs/compositeplaintextedit.cpp
    src/animatedwidgets/inputs/compositespinbox.h
    src/animatedwidgets/inputs/compositespinbox.cpp
    src/animatedwidgets/inputs/compositedoublespinbox.h
    src/animatedwidgets/inputs/compositedoublespinbox.cpp
    src/animatedwidgets/comboboxes/combobox.h
    src/animatedwidgets/comboboxes/combobox.cpp
    src/animatedwidgets/comboboxes/searchcombobox.h
    src/animatedwidgets/comboboxes/searchcombobox.cpp
    src/animatedwidgets/comboboxes/treecombobox.h
    src/animatedwidgets/comboboxes/treecombobox.cpp
    src/animatedwidgets/comboboxes/hierarchicalcombobox.h
    src/animatedwidgets/comboboxes/hierarchicalcombobox.cpp
    src/animatedwidgets/comboboxes/cascadercombobox.h
    src/animatedwidgets/comboboxes/cascadercombobox.cpp
    src/animatedwidgets/popups/cascaderpopupmenu.h
    src/animatedwidgets/popups/cascaderpopupmenu.cpp
    src/animatedwidgets/popups/cascadercolumnview.h
    src/animatedwidgets/popups/cascadercolumnview.cpp
    src/animatedwidgets/popups/cascaderitemdelegate.h
    src/animatedwidgets/popups/cascaderitemdelegate.cpp
    src/animatedwidgets/comboboxes/creatablecombobox.h
    src/animatedwidgets/comboboxes/creatablecombobox.cpp
    src/animatedwidgets/comboboxes/treepopupmodel.h
    src/animatedwidgets/comboboxes/treepopupmodel.cpp
    src/animatedwidgets/comboboxes/multiselectcombobox.h
    src/animatedwidgets/comboboxes/multiselectcombobox.cpp
    src/animatedwidgets/tags/tagstyle.h
    src/animatedwidgets/tags/tagwidget.h
    src/animatedwidgets/tags/tagwidget.cpp
    src/animatedwidgets/comboboxes/combolistpopup.h
    src/animatedwidgets/comboboxes/combolistpopup.cpp
    src/animatedwidgets/comboboxes/combopopupmodel.h
    src/animatedwidgets/comboboxes/combopopupmodel.cpp
    src/animatedwidgets/comboboxes/compositecombobox.h
    src/animatedwidgets/comboboxes/compositecombobox.cpp
    src/animatedwidgets/comboboxes/resources/comboboxes.qrc
    src/animatedwidgets/inputs/resources/inputs.qrc
)
add_library(AnimatedWidgets::Widgets ALIAS AnimatedWidgetsWidgets)
target_include_directories(AnimatedWidgetsWidgets PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(AnimatedWidgetsWidgets PUBLIC
    AnimatedWidgetsStyle
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Svg
)

set(ANIMATEDWIDGETS_HAS_SMOOTHSCROLLBAR OFF)
if(TARGET SmoothScrollbar::SmoothScrollbar)
    target_link_libraries(AnimatedWidgetsWidgets PRIVATE SmoothScrollbar::SmoothScrollbar)
    target_compile_definitions(AnimatedWidgetsWidgets PRIVATE ANIMATEDWIDGETS_HAS_SMOOTHSCROLLBAR)
    set(ANIMATEDWIDGETS_HAS_SMOOTHSCROLLBAR ON)
    message(STATUS "AnimatedWidgets: SmoothScrollbar enabled")
else()
    message(STATUS "AnimatedWidgets: SmoothScrollbar not found; using native scrolling")
endif()

add_library(AnimatedWidgets INTERFACE)
add_library(AnimatedWidgets::AnimatedWidgets ALIAS AnimatedWidgets)
target_link_libraries(AnimatedWidgets INTERFACE AnimatedWidgetsWidgets)

if(BUILD_DESIGNER_PLUGIN)
    add_subdirectory(plugins/designer)
endif()

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(ANIMATEDWIDGETS_ENABLE_INSTALL)
    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)

    install(DIRECTORY src/animatedwidgets/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/animatedwidgets
        FILES_MATCHING PATTERN "*.h")
    install(TARGETS AnimatedWidgetsCore AnimatedWidgetsStyle AnimatedWidgetsWidgets AnimatedWidgets
        EXPORT AnimatedWidgetsTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
    if(BUILD_DESIGNER_PLUGIN)
        install(TARGETS animatedwidgetsdesigner
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/qtplugins/designer
            RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR}/qtplugins/designer
        )
    endif()
    install(EXPORT AnimatedWidgetsTargets
        FILE AnimatedWidgetsTargets.cmake
        NAMESPACE AnimatedWidgets::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/AnimatedWidgets
    )

    configure_package_config_file(
        cmake/AnimatedWidgetsConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/AnimatedWidgetsConfig.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/AnimatedWidgets
    )
    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/AnimatedWidgetsConfigVersion.cmake
        COMPATIBILITY SameMajorVersion
    )
    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/AnimatedWidgetsConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/AnimatedWidgetsConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/AnimatedWidgets
    )
endif()
