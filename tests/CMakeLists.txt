find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)

add_executable(tstanimationcontroller tstanimationcontroller.cpp)
target_link_libraries(tstanimationcontroller PRIVATE AnimatedWidgets::Core Qt${QT_VERSION_MAJOR}::Test)
add_test(NAME tstanimationcontroller COMMAND tstanimationcontroller)

add_executable(tstvaluetransition tstvaluetransition.cpp)
target_link_libraries(tstvaluetransition PRIVATE AnimatedWidgets::Core Qt${QT_VERSION_MAJOR}::Test)
add_test(NAME tstvaluetransition COMMAND tstvaluetransition)

add_executable(tstbuttonstatecolors tstbuttonstatecolors.cpp)
target_link_libraries(tstbuttonstatecolors PRIVATE AnimatedWidgets::Widgets Qt${QT_VERSION_MAJOR}::Test)
add_test(NAME tstbuttonstatecolors COMMAND tstbuttonstatecolors)

add_executable(tstdefaultpopupmenumodel tstdefaultpopupmenumodel.cpp)
target_link_libraries(tstdefaultpopupmenumodel PRIVATE AnimatedWidgets::Widgets Qt${QT_VERSION_MAJOR}::Test)
add_test(NAME tstdefaultpopupmenumodel COMMAND tstdefaultpopupmenumodel)
