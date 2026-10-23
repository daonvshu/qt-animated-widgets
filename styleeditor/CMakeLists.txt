find_package(QRouter REQUIRED CONFIG)
find_package(VirtualItemViews REQUIRED CONFIG)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Network)

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
    pages/thememanagerpage.ui
    pages/exportprojectpage.h
    pages/exportprojectpage.cpp
    pages/designguidelinespage.h
    pages/designguidelinespage.cpp
    pages/helpdocumentationpage.h
    pages/helpdocumentationpage.cpp

    navigation/styleworkspacerouter.h
    navigation/styleworkspacerouter.cpp

    resources/styleeditor.qrc
    resources/stylethemes.qrc

    preview/stylepreviewprotocol.h
    preview/stylepreviewprotocol.cpp
    preview/stylepreviewchannel.h
    preview/stylepreviewchannel.cpp
    preview/stylepreviewclient.h
    preview/stylepreviewclient.cpp
    preview/stylepreviewcanvas.h
    preview/stylepreviewcanvas.cpp
    preview/stylepreviewworker.h
    preview/stylepreviewworker.cpp

    themes/stylethememanager.h
    themes/stylethememanager.cpp
    themes/stylethemescope.h
    themes/stylethemescope.cpp

    dialogs/stylepopupdialog.h
    dialogs/stylepopupdialog.cpp
    dialogs/stylemessagepopup.h
    dialogs/stylemessagepopup.cpp
    dialogs/stylemessagepopup.ui
    dialogs/stylewidgetitempopup.h
    dialogs/stylewidgetitempopup.cpp
    dialogs/stylewidgetitempopup.ui
    dialogs/styleiconresourcepopup.h
    dialogs/styleiconresourcepopup.cpp
    dialogs/styleiconresourcepopup.ui
    dialogs/colorpickerdialog.h
    dialogs/colorpickerdialog.cpp
    dialogs/colorpickerdialog.ui

    document/styledocument.h
    document/stylepreviewsettings.h
    document/styleresourcesettings.h
    document/styleresourceentry.h
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
    widgets/colors/screencolorpicker.h
    widgets/colors/screencolorpicker.cpp
    widgets/colors/screencoloroverlay.h
    widgets/colors/screencoloroverlay.cpp

    panels/stylewidgettreemodel.h
    panels/stylewidgettreemodel.cpp
    panels/stylewidgettreeadapter.h
    panels/stylewidgettreeadapter.cpp
    panels/stylewidgettreerow.h
    panels/stylewidgettreerow.cpp
    panels/stylewidgettreerow.ui

    panels/styleicontreemodel.h
    panels/styleicontreemodel.cpp
    panels/styleicontreeadapter.h
    panels/styleicontreeadapter.cpp
    panels/styleicontreerow.h
    panels/styleicontreerow.cpp

    plugins/stylepluginregistry.h
    plugins/stylepluginregistry.cpp

    utils/styledocumentcodec.h
    utils/styledocumentcodec.cpp
    utils/stylecodegenerator.h
    utils/stylecodegenerator.cpp
    utils/stylewidgettypeutils.h
    utils/stylewidgettypeutils.cpp
    utils/styleresourceutils.h
    utils/styleresourceutils.cpp
    utils/stylepropertyeditorutils.h
    utils/stylepropertyeditorutils.cpp
    utils/stylepreviewutils.h
    utils/stylepreviewutils.cpp
    utils/stylethemeutils.h
    utils/stylethemeutils.cpp
    utils/screencolorutils.h
    utils/screencolorutils.cpp
    utils/objectfindtool.h
    utils/objectfindtool.cpp
)

target_link_libraries(animatedwidgets-styleeditor PRIVATE
    AnimatedWidgets::StyleEditorApi
    AnimatedWidgets::AnimatedWidgets
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    qrouter
    VirtualItemViews::VirtualItemViews
)

target_include_directories(animatedwidgets-styleeditor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(animatedwidgets-styleeditor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/styleeditor"
)
