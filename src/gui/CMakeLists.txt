find_package(Qt5 COMPONENTS Widgets Qml LinguistTools REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(HEADERS
    aboutdialog.h
    altspinbox.h
    contentwidget.h
    commandlineoptions.h
    commandlineparser.h
    filedialogutils.h
    gptiniutil.h
    gui.h
    listboxdialog.h
    mainwindow.h
    mainwindowsettings.h
    presentationbuilder.h
    pluginwidgetinterface.h
    treevieweventfilter.h
)

set(SOURCES
    aboutdialog.cpp
    altspinbox.cpp
    commandlineparser.cpp
    contentwidget.cpp
    filedialogutils.cpp
    gptiniutil.cpp
    gui.cpp
    listboxdialog.cpp
    mainwindow.cpp
    mainwindowsettings.cpp
    treevieweventfilter.cpp
)

set(UI_FORMS
    aboutdialog.ui
    contentwidget.ui
    listboxdialog.ui
    mainwindow.ui    
)

set(MOC_HEADERS
    aboutdialog.h
    altspinbox.h
    contentwidget.h
    gptiniutil.h
    listboxdialog.h
    mainwindow.h
    mainwindowsettings.h
    pluginwidgetinterface.h
    treevieweventfilter.h
)

set(TS_FILES
    i18n/gui_en.ts
    i18n/gui_ru.ts
)

set(ICON_RESOURCES
    icon.qrc
)

set_source_files_properties(${TS_FILES} PROPERTIES OUTPUT_LOCATION ${CMAKE_CURRENT_SOURCE_DIR})

qt5_add_translation(QM_FILES ${TS_FILES})

qt5_wrap_ui(UI_SOURCES ${UI_FORMS})
qt5_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})

add_definitions(
    -DGPUI_GUI_LIBRARY
)

add_translation_resource(RESOURCES_SRC "gui" ${QM_FILES})

qt5_add_resources(LIB_RESOURCES ${RESOURCES_SRC} ${ICON_RESOURCES})

add_gpui_library(gpui-gui ${UI_SOURCES} ${MOC_SOURCES} ${SOURCES} ${LIB_RESOURCES})
target_link_libraries(gpui-gui Qt5::Widgets Qt5::Qml)
target_link_libraries(gpui-gui gpui-core gpui-io gpui-ldap)
target_link_libraries(gpui-gui smb-storage-static)
