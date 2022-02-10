find_package(GPUI COMPONENTS io model REQUIRED)
include_directories(${GPUI_INCLUDE_DIRS})

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(HEADERS
    modelbuilder.h
    shortcutroles.h
    shortcutsschema.h
    shortcutswidget.h    
)

set(SOURCES
    modelbuilder.cpp
    shortcutsschema.cpp
    shortcutswidget.cpp
)

set(UI_FORMS
    shortcutswidget.ui
)

set(MOC_HEADERS
    shortcutswidget.h
)

qt5_wrap_ui(UI_SOURCES ${UI_FORMS})
qt5_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})

add_gpui_plugin(shortcuts-plugin ${UI_SOURCES} ${MOC_SOURCES} ${SOURCES})
target_link_libraries(shortcuts-plugin Qt5::Core Qt5::Widgets)
target_link_libraries(shortcuts-plugin ${GPUI_LIBRARIES})
target_link_libraries(shortcuts-plugin xerces-c)
