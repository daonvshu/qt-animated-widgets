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
    src/animatedwidgets/buttons/animatedbuttonbase.h
    src/animatedwidgets/buttons/animatedbuttonbase.cpp
    src/animatedwidgets/buttons/buttonstate.h
    src/animatedwidgets/buttons/buttonstatestyle.h
    src/animatedwidgets/utils/buttonstatecolors.h
    src/animatedwidgets/utils/buttonstatecolors.cpp
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
    src/animatedwidgets/inputs/iconlineedit.h
    src/animatedwidgets/inputs/iconlineedit.cpp
    src/animatedwidgets/inputs/passwordlineedit.h
    src/animatedwidgets/inputs/passwordlineedit.cpp
    src/animatedwidgets/inputs/compositelineedit.h
    src/animatedwidgets/inputs/compositelineedit.cpp
    src/animatedwidgets/inputs/compositeplaintextedit.h
    src/animatedwidgets/inputs/compositeplaintextedit.cpp
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
