find_package(GPUI COMPONENTS io model REQUIRED)
include_directories(${GPUI_INCLUDE_DIRS})

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(HEADERS
    iniroles.h
    iniwidget.h
    inischema.h
    modelbuilder.h
)

set(SOURCES
    iniwidget.cpp
    inischema.cpp
    modelbuilder.cpp
)

set(UI_FORMS
    iniwidget.ui
)

set(MOC_HEADERS
    iniwidget.h
)

qt5_wrap_ui(UI_SOURCES ${UI_FORMS})
qt5_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})

add_gpui_plugin(ini-plugin ${UI_SOURCES} ${MOC_SOURCES} ${SOURCES})
target_link_libraries(ini-plugin Qt5::Core Qt5::Widgets)
target_link_libraries(ini-plugin ${GPUI_LIBRARIES})
