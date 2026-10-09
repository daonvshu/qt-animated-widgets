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
