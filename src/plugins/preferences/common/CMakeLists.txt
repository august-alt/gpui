find_package(GPUI COMPONENTS io model REQUIRED)
include_directories(${GPUI_INCLUDE_DIRS})

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(HEADERS
    commonroles.h
    commonwidget.h
    preferencesdialog.h
)

set(SOURCES
    commonwidget.cpp
    preferencesdialog.cpp
)

set(UI_FORMS
    commonwidget.ui
    preferencesdialog.ui
)

set(MOC_HEADERS
    commonwidget.h
    preferencesdialog.h
)

qt5_wrap_ui(UI_SOURCES ${UI_FORMS})
qt5_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})

add_library(preferences-common STATIC ${UI_SOURCES} ${MOC_SOURCES} ${SOURCES})
target_link_libraries(preferences-common Qt5::Core Qt5::Widgets)
target_link_libraries(preferences-common ${GPUI_LIBRARIES})
