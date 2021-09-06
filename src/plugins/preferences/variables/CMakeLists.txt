find_package(GPUI COMPONENTS io model REQUIRED)
include_directories(${GPUI_INCLUDE_DIRS})

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(HEADERS
    variableswidget.h
)

set(SOURCES
    variableswidget.cpp
)

set(UI_FORMS
    variableswidget.ui
)

set(MOC_HEADERS
    variableswidget.h
)

qt5_wrap_ui(UI_SOURCES ${UI_FORMS})
qt5_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})

add_gpui_plugin(variables-plugin ${UI_SOURCES} ${MOC_SOURCES} ${SOURCES})
target_link_libraries(variables-plugin Qt5::Core Qt5::Widgets)
target_link_libraries(variables-plugin ${GPUI_LIBRARIES})
