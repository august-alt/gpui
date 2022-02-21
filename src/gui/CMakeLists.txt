find_package(Qt5 COMPONENTS Widgets Qml LinguistTools REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(HEADERS
    aboutdialog.h
    contentwidget.h
    commandlineoptions.h
    commandlineparser.h
    gui.h
    listboxdialog.h
    mainwindow.h
    mainwindowsettings.h
    presentationbuilder.h
)

set(SOURCES
    aboutdialog.cpp
    commandlineparser.cpp
    contentwidget.cpp
    gui.cpp
    listboxdialog.cpp
    mainwindow.cpp
    mainwindowsettings.cpp
    presentationbuilder.cpp
)

set(UI_FORMS
    aboutdialog.ui
    contentwidget.ui
    listboxdialog.ui
    mainwindow.ui
)

set(MOC_HEADERS
    aboutdialog.h
    contentwidget.h
    listboxdialog.h
    mainwindow.h
    mainwindowsettings.h
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

add_translation_resource(RESOURCES_SRC ${QM_FILES})

qt5_add_resources(LIB_RESOURCES ${RESOURCES_SRC} ${ICON_RESOURCES})

add_gpui_library(gpui-gui ${UI_SOURCES} ${MOC_SOURCES} ${SOURCES} ${LIB_RESOURCES})
target_link_libraries(gpui-gui Qt5::Widgets Qt5::Qml)
target_link_libraries(gpui-gui gpui-model gpui-io)
target_link_libraries(gpui-gui smb-storage-static)
