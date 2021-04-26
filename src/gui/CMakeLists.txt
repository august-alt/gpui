find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(HEADERS
    gui.h
    mainwindow.h
    presentationbuilder.h
)

set(SOURCES
    gui.cpp
    mainwindow.cpp
    presentationbuilder.cpp
)

set(UI_FORMS
    mainwindow.ui
)

set(MOC_HEADERS
    mainwindow.h
)

qt5_wrap_ui(UI_SOURCES ${UI_FORMS})
qt5_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})

add_definitions(
    -DGPUI_GUI_LIBRARY
)

add_gpui_library(gpui-gui ${UI_SOURCES} ${MOC_SOURCES} ${SOURCES})
target_link_libraries(gpui-gui Qt5::Widgets)
