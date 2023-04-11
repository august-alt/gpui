find_package(GPUI COMPONENTS gui io REQUIRED)
include_directories(${GPUI_INCLUDE_DIRS})

find_package(Qt5 COMPONENTS Core Gui Test REQUIRED)
set(QT_USE_QTTEST TRUE)

add_subdirectory(presentationbuilder)
