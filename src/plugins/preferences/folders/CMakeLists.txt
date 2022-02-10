find_package(GPUI COMPONENTS io model REQUIRED)
include_directories(${GPUI_INCLUDE_DIRS})

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(HEADERS
    folderroles.h
    foldersschema.h
    folderswidget.h
    modelbuilder.h
)

set(SOURCES
    foldersschema.cpp
    folderswidget.cpp
    modelbuilder.cpp
)

set(UI_FORMS
    folderswidget.ui
)

set(MOC_HEADERS
    folderswidget.h
)

qt5_wrap_ui(UI_SOURCES ${UI_FORMS})
qt5_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})

add_gpui_plugin(folders-plugin ${UI_SOURCES} ${MOC_SOURCES} ${SOURCES})
target_link_libraries(folders-plugin Qt5::Core Qt5::Widgets)
target_link_libraries(folders-plugin ${GPUI_LIBRARIES})
target_link_libraries(folders-plugin xerces-c)
