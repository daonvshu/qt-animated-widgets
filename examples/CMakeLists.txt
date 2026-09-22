find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Svg)

add_executable(animatedwidgets-gallery
    main.cpp
    gallery.ui
)
target_link_libraries(animatedwidgets-gallery PRIVATE
    AnimatedWidgets::AnimatedWidgets
    Qt${QT_VERSION_MAJOR}::Svg
)
