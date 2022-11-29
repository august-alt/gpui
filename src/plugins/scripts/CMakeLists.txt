find_package(GPUI COMPONENTS core io REQUIRED)
find_package(Qt5 COMPONENTS Widgets Core LinguistTools REQUIRED)
find_package(MVVM REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(HEADERS
    scriptscontentwidget.h
    scriptssnapin.h
    scriptswidget.h
    scriptspowershellwidget.h
    groupscriptcontaineritem.h
    scriptitem.h
    scriptitemcontainer.h
    scriptmodelbuilder.h
    scriptsdialog.h
    scriptsmodel.h
    basescriptswidget.h
    scriptwidgetcommonslots.h
    addscriptwidget.h
    scriptssnapin.h
    scriptssnapinprivate.h
    scriptstreemodel.h
    scriptsmodelio.h
    scriptstreeproxymodel.h
)

set(SOURCES
    scriptscontentwidget.cpp
    scriptssnapin.cpp
    scriptsplugin.cpp
    scriptswidget.cpp
    scriptspowershellwidget.cpp
    groupscriptcontaineritem.cpp
    scriptitem.cpp
    scriptitemcontainer.cpp
    scriptmodelbuilder.cpp
    scriptsdialog.cpp
    scriptsmodel.cpp
    basescriptswidget.cpp
    scriptwidgetcommonslots.cpp
    addscriptwidget.cpp
    scriptssnapin.cpp
    scriptssnapinprivate.cpp
    scriptstreemodel.cpp
    scriptsmodelio.cpp
    scriptstreeproxymodel.cpp
)

set(UIFORMS
    scriptscontentwidget.ui
    scriptswidget.ui
    scriptsdialog.ui
    scriptspowershellwidget.ui
    addscriptwidget.ui
)

include_directories(${CMAKE_SOURCE_DIR}/src)

set(PLUGIN_NAME scripts-plugin)

file(GLOB_RECURSE TS_FILES ${CMAKE_CURRENT_SOURCE_DIR} *.ts)

#set_source_files_properties(${TS_FILES} PROPERTIES OUTPUT_LOCATION ${CMAKE_CURRENT_SOURCE_DIR})

add_translation(QM_FILES "${TS_FILES}")

add_translation_resource(RESOURCES_SRC "${QM_FILES}")

qt5_add_resources(LIB_RESOURCES ${RESOURCES_SRC})

set(SOURCES ${SOURCES} ${HEADERS} ${UIFORMS})

add_gpui_plugin(${PLUGIN_NAME} ${SOURCES} ${LIB_RESOURCES})
target_link_libraries(${PLUGIN_NAME} Qt5::Core Qt5::Widgets)
target_link_libraries(${PLUGIN_NAME} ${GPUI_LIBRARIES})
target_link_libraries(${PLUGIN_NAME} ${MVVM_LIBRARIES})


