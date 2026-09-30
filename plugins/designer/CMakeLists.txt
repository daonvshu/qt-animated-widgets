find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS UiPlugin)

add_library(animatedwidgetsdesigner MODULE
    plugins.h
    plugins.cpp
    designerwidgets.h
    buttons/variantbuttonplugin.h
    buttons/variantbuttonplugin.cpp
    switches/switchplugin.h
    switches/switchplugin.cpp
    progressbars/progressbarplugin.h
    progressbars/progressbarplugin.cpp
    sliders/sliderplugin.h
    sliders/sliderplugin.cpp
    inputs/inputplugin.h
    inputs/inputplugin.cpp
    comboboxes/comboboxplugin.h
    comboboxes/comboboxplugin.cpp
)
target_compile_definitions(animatedwidgetsdesigner PRIVATE ANIMATEDWIDGETS_DESIGNER_PLUGIN)
target_link_libraries(animatedwidgetsdesigner PRIVATE
    AnimatedWidgets::Widgets
    Qt${QT_VERSION_MAJOR}::UiPlugin
)

get_target_property(qt_qmake_executable Qt${QT_VERSION_MAJOR}::qmake IMPORTED_LOCATION)
if(NOT qt_qmake_executable)
    message(FATAL_ERROR "Could not locate qmake for the selected Qt installation")
endif()

execute_process(
    COMMAND "${qt_qmake_executable}" -query QT_INSTALL_PLUGINS
    OUTPUT_VARIABLE qt_plugins_dir
    OUTPUT_STRIP_TRAILING_WHITESPACE
    RESULT_VARIABLE qt_plugins_query_result
)
if(NOT qt_plugins_query_result EQUAL 0 OR NOT qt_plugins_dir)
    message(FATAL_ERROR "Could not query QT_INSTALL_PLUGINS from ${qt_qmake_executable}")
endif()

set(qt_designer_plugins_dir "${qt_plugins_dir}/designer")
add_custom_command(TARGET animatedwidgetsdesigner POST_BUILD
    COMMAND "${CMAKE_COMMAND}"
        "-DPLUGIN_FILE=$<TARGET_FILE:animatedwidgetsdesigner>"
        "-DDESTINATION_DIR=${qt_designer_plugins_dir}"
        "-DBUILD_CONFIGURATION=$<CONFIG>"
        -P "${CMAKE_CURRENT_LIST_DIR}/copydesignerplugin.cmake"
    VERBATIM
)
