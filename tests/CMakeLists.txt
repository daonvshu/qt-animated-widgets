find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)

add_executable(tstanimationcontroller tstanimationcontroller.cpp)
target_link_libraries(tstanimationcontroller PRIVATE AnimatedWidgets::Core Qt${QT_VERSION_MAJOR}::Test)
add_test(NAME tstanimationcontroller COMMAND tstanimationcontroller)
