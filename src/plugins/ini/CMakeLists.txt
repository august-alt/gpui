find_package(GPUI COMPONENTS core io REQUIRED)
include_directories(${GPUI_INCLUDE_DIRS})

find_package(Qt5 COMPONENTS Core REQUIRED)

set(HEADERS
    iniformat.h
)

set(SOURCES
    iniformat.cpp
    iniplugin.cpp
)

add_gpui_plugin(inifile-plugin ${SOURCES})
target_link_libraries(inifile-plugin Qt5::Core)
target_link_libraries(inifile-plugin ${GPUI_LIBRARIES})
