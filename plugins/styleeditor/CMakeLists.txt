add_custom_target(animatedwidgetsstyleeditorplugins)

function(animatedwidgets_add_style_plugin category)
    set(target "animatedwidgetsstyleeditor${category}")
    add_library(${target} MODULE ${ARGN}
        ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/utils/stylepluginproperties.h
        ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/utils/stylepluginproperties.cpp
        ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/utils/styleplugintypeutils.h
        ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/utils/styleobjectproperties.h
        ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/utils/stylepreviewdata.h
        ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/utils/stylepreviewdata.cpp
    )
    target_link_libraries(${target} PRIVATE
        AnimatedWidgets::StyleEditorApi
        AnimatedWidgets::AnimatedWidgets
    )
    set_target_properties(${target} PROPERTIES
        OUTPUT_NAME "animatedwidgets-styleeditor-${category}"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/styleeditor/plugins/styleeditor"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/styleeditor/plugins/styleeditor"
    )
    add_dependencies(animatedwidgetsstyleeditorplugins ${target})
endfunction()

add_subdirectory(buttons)
add_subdirectory(selections)
add_subdirectory(switches)
add_subdirectory(progressbars)
add_subdirectory(sliders)
add_subdirectory(inputs)
add_subdirectory(comboboxes)
add_subdirectory(tags)
add_subdirectory(calendars)
add_subdirectory(popups)
