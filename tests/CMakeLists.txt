find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)

add_executable(tstcalendardateutils tstcalendardateutils.cpp)
target_link_libraries(tstcalendardateutils PRIVATE AnimatedWidgets::Widgets Qt${QT_VERSION_MAJOR}::Test)
add_test(NAME tstcalendardateutils COMMAND tstcalendardateutils)

add_executable(tstanimationcontroller tstanimationcontroller.cpp)
target_link_libraries(tstanimationcontroller PRIVATE AnimatedWidgets::Core Qt${QT_VERSION_MAJOR}::Test)
add_test(NAME tstanimationcontroller COMMAND tstanimationcontroller)

add_executable(tstvaluetransition tstvaluetransition.cpp)
target_link_libraries(tstvaluetransition PRIVATE AnimatedWidgets::Core Qt${QT_VERSION_MAJOR}::Test)
add_test(NAME tstvaluetransition COMMAND tstvaluetransition)

add_executable(tstbuttonstatecolors tstbuttonstatecolors.cpp)
target_link_libraries(tstbuttonstatecolors PRIVATE AnimatedWidgets::Widgets Qt${QT_VERSION_MAJOR}::Test)
add_test(NAME tstbuttonstatecolors COMMAND tstbuttonstatecolors)

add_executable(tsticonproperties tsticonproperties.cpp)
target_link_libraries(tsticonproperties PRIVATE AnimatedWidgets::Widgets Qt${QT_VERSION_MAJOR}::Test)
add_test(NAME tsticonproperties COMMAND tsticonproperties)

add_executable(tstdefaultpopupmenumodel tstdefaultpopupmenumodel.cpp)
target_link_libraries(tstdefaultpopupmenumodel PRIVATE AnimatedWidgets::Widgets Qt${QT_VERSION_MAJOR}::Test)
add_test(NAME tstdefaultpopupmenumodel COMMAND tstdefaultpopupmenumodel)

add_executable(tstgroupedpopupmenumodel tstgroupedpopupmenumodel.cpp)
target_link_libraries(tstgroupedpopupmenumodel PRIVATE AnimatedWidgets::Widgets Qt${QT_VERSION_MAJOR}::Test)
add_test(NAME tstgroupedpopupmenumodel COMMAND tstgroupedpopupmenumodel)

add_executable(tstcombopopupmodel tstcombopopupmodel.cpp)
target_link_libraries(tstcombopopupmodel PRIVATE AnimatedWidgets::Widgets Qt${QT_VERSION_MAJOR}::Test)
add_test(NAME tstcombopopupmodel COMMAND tstcombopopupmodel)

add_executable(tstcombocreationutils tstcombocreationutils.cpp)
target_link_libraries(tstcombocreationutils PRIVATE AnimatedWidgets::Widgets Qt${QT_VERSION_MAJOR}::Test)
add_test(NAME tstcombocreationutils COMMAND tstcombocreationutils)

add_executable(tsttreepathutils tsttreepathutils.cpp)
target_link_libraries(tsttreepathutils PRIVATE AnimatedWidgets::Widgets Qt${QT_VERSION_MAJOR}::Test)
add_test(NAME tsttreepathutils COMMAND tsttreepathutils)

add_executable(tstpopupmenufiltermodel tstpopupmenufiltermodel.cpp)
target_link_libraries(tstpopupmenufiltermodel PRIVATE AnimatedWidgets::Widgets Qt${QT_VERSION_MAJOR}::Test)
add_test(NAME tstpopupmenufiltermodel COMMAND tstpopupmenufiltermodel)

if(BUILD_STYLE_EDITOR)
    add_executable(tststyleicontreemodel
        tststyleicontreemodel.cpp
        ../styleeditor/panels/styleicontreemodel.h
        ../styleeditor/panels/styleicontreemodel.cpp
    )
    target_link_libraries(tststyleicontreemodel PRIVATE Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Test)
    add_test(NAME tststyleicontreemodel COMMAND tststyleicontreemodel)

    add_executable(tststyleresourceutils
        tststyleresourceutils.cpp
        ../styleeditor/utils/styleresourceutils.cpp
        ../styleeditor/utils/stylecodegenerator.cpp
        ../styleeditor/utils/styledocumentcodec.cpp
        ../styleeditor/document/styledocumentsession.cpp
    )
    target_link_libraries(tststyleresourceutils PRIVATE AnimatedWidgets::StyleEditorApi Qt${QT_VERSION_MAJOR}::Test)
    add_test(NAME tststyleresourceutils COMMAND tststyleresourceutils)

    add_executable(tststylepropertybuilder
        tststylepropertybuilder.cpp
        stylepropertytestobject.h
        ../styleeditor/utils/stylecodegenerator.h
        ../styleeditor/utils/stylecodegenerator.cpp
        ../styleeditor/utils/styleresourceutils.cpp
        ../styleeditor/utils/styledocumentcodec.h
        ../styleeditor/utils/styledocumentcodec.cpp
    )
    target_link_libraries(tststylepropertybuilder PRIVATE AnimatedWidgets::StyleEditorApi Qt${QT_VERSION_MAJOR}::Test)
    add_test(NAME tststylepropertybuilder COMMAND tststylepropertybuilder)

    add_executable(tststyledocumentsession
        tststyledocumentsession.cpp
        ../styleeditor/document/styledocumentsession.h
        ../styleeditor/document/styledocumentsession.cpp
        ../styleeditor/utils/stylepreviewutils.h
        ../styleeditor/utils/stylepreviewutils.cpp
        ../styleeditor/utils/styledocumentcodec.h
        ../styleeditor/utils/styledocumentcodec.cpp
        ../styleeditor/utils/stylecodegenerator.h
        ../styleeditor/utils/stylecodegenerator.cpp
        ../styleeditor/utils/styleresourceutils.cpp
    )
    target_link_libraries(tststyledocumentsession PRIVATE AnimatedWidgets::StyleEditorApi Qt${QT_VERSION_MAJOR}::Test)
    add_test(NAME tststyledocumentsession COMMAND tststyledocumentsession)

    add_executable(tststylewidgettreemodel
        tststylewidgettreemodel.cpp
        ../styleeditor/panels/stylewidgettreemodel.h
        ../styleeditor/panels/stylewidgettreemodel.cpp
    )
    target_link_libraries(tststylewidgettreemodel PRIVATE AnimatedWidgets::StyleEditorApi Qt${QT_VERSION_MAJOR}::Test)
    add_test(NAME tststylewidgettreemodel COMMAND tststylewidgettreemodel)
endif()
