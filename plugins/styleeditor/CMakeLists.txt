add_library(animatedwidgetsstyleeditorbuttons MODULE
    buttons/buttonstyleplugin.h
    buttons/buttonstyleplugin.cpp
    buttons/buttonstyleplugin.json
    buttons/buttonstyleproperties.h
    buttons/buttonstyleproperties.cpp
)

target_link_libraries(animatedwidgetsstyleeditorbuttons PRIVATE
    AnimatedWidgets::StyleEditorApi
    AnimatedWidgets::AnimatedWidgets
)

set_target_properties(animatedwidgetsstyleeditorbuttons PROPERTIES
    OUTPUT_NAME "animatedwidgets-styleeditor-buttons"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/styleeditor/plugins/styleeditor"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/styleeditor/plugins/styleeditor"
)
