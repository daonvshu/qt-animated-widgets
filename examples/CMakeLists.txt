find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Svg)

add_executable(animatedwidgets-gallery
    main.cpp
    calendarspage.cpp
    calendarspage.h
    calendarspage.ui
    buttonspage.cpp
    buttonspage.h
    buttonspage.ui
    inputspage.cpp
    inputspage.h
    inputspage.ui
    comboboxespage.cpp
    comboboxespage.h
    comboboxespage.ui
    popupmenuspage.cpp
    popupmenuspage.h
    popupmenuspage.ui
    progresspage.cpp
    progresspage.h
    progresspage.ui
    selectionpage.cpp
    selectionpage.h
    selectionpage.ui
    sliderspage.cpp
    sliderspage.h
    sliderspage.ui
    switchespage.cpp
    switchespage.h
    switchespage.ui
    gallery.ui
    resources/gallery.qrc
)
target_link_libraries(animatedwidgets-gallery PRIVATE
    AnimatedWidgets::AnimatedWidgets
    Qt${QT_VERSION_MAJOR}::Svg
)
