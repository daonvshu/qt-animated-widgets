find_package(QRouter REQUIRED CONFIG)
find_package(VirtualItemViews REQUIRED CONFIG)

add_executable(animatedwidgets-styleeditor
    main.cpp
    styleeditorwindow.h
    styleeditorwindow.cpp
    styleeditorwindow.ui
    pages/styleeditorpage.h
    pages/styleeditorpage.cpp
    pages/styleeditorpage.ui
    pages/workspacepage.h
    pages/workspacepage.cpp
    pages/workspacesectionpage.h
    pages/workspacesectionpage.cpp
    pages/workspacesectionpage.ui
    pages/componentlibrarypage.h
    pages/componentlibrarypage.cpp
    pages/thememanagerpage.h
    pages/thememanagerpage.cpp
    pages/exportprojectpage.h
    pages/exportprojectpage.cpp
    pages/designguidelinespage.h
    pages/designguidelinespage.cpp
    pages/helpdocumentationpage.h
    pages/helpdocumentationpage.cpp
    navigation/styleworkspacerouter.h
    navigation/styleworkspacerouter.cpp
    resources/styleeditor.qrc
    dialogs/stylepopupdialog.h
    dialogs/stylepopupdialog.cpp
    dialogs/stylemessagepopup.h
    dialogs/stylemessagepopup.cpp
    dialogs/stylemessagepopup.ui
    dialogs/styledocumentfilepopup.h
    dialogs/styledocumentfilepopup.cpp
    dialogs/styledocumentfilepopup.ui
    dialogs/stylewidgetitempopup.h
    dialogs/stylewidgetitempopup.cpp
    dialogs/stylewidgetitempopup.ui
    dialogs/colorpickerdialog.h
    dialogs/colorpickerdialog.cpp
    dialogs/colorpickerdialog.ui
    document/styledocument.h
    document/stylepreviewsettings.h
    document/styleeditoritem.h
    document/styleeditorsession.h
    document/styleeditorsession.cpp
    document/styledocumentsession.h
    document/styledocumentsession.cpp
    panels/stylepropertyinspector.h
    panels/stylepropertyinspector.cpp
    panels/stylepreviewpanel.h
    panels/stylepreviewpanel.cpp
    panels/stylepreviewpanel.ui
    panels/stylepropertyrow.h
    panels/stylepropertyrow.cpp
    panels/stylepropertyrow.ui
    widgets/colors/vscolorpicker.h
    widgets/colors/vscolorpicker.cpp
    widgets/colors/hcolorpicker.h
    widgets/colors/hcolorpicker.cpp
    widgets/colors/presetcolorpicker.h
    widgets/colors/presetcolorpicker.cpp
    panels/stylewidgettreemodel.h
    panels/stylewidgettreemodel.cpp
    panels/stylewidgettreeadapter.h
    panels/stylewidgettreeadapter.cpp
    panels/stylewidgettreerow.h
    panels/stylewidgettreerow.cpp
    panels/stylewidgettreerow.ui
    plugins/stylepluginregistry.h
    plugins/stylepluginregistry.cpp
    utils/styledocumentcodec.h
    utils/styledocumentcodec.cpp
    utils/stylecodegenerator.h
    utils/stylecodegenerator.cpp
    utils/stylepropertyeditorutils.h
    utils/stylepropertyeditorutils.cpp
    utils/stylepreviewutils.h
    utils/stylepreviewutils.cpp
)

target_link_libraries(animatedwidgets-styleeditor PRIVATE
    AnimatedWidgets::StyleEditorApi
    AnimatedWidgets::AnimatedWidgets
    Qt${QT_VERSION_MAJOR}::Widgets
    qrouter
    VirtualItemViews::VirtualItemViews
)

target_include_directories(animatedwidgets-styleeditor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(animatedwidgets-styleeditor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/styleeditor"
)
