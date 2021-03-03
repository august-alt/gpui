find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(HEADERS
    gui.h
    mainwindow.h
)

set(SOURCES
    gui.cpp
    mainwindow.cpp
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

add_gpui_library(gpui-gui ${SOURCES} ${UI_SOURCES} ${MOC_SOURCES})
target_link_libraries(gpui-gui Qt5::Widgets)
