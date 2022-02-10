find_package(GPUI COMPONENTS io model REQUIRED)
include_directories(${GPUI_INCLUDE_DIRS})

find_package(Qt5 COMPONENTS Core REQUIRED)

set(HEADERS
    spolformat.h
)

set(SOURCES
    spolformat.cpp
    spolplugin.cpp
)

add_gpui_plugin(spol-plugin ${SOURCES})
target_link_libraries(spol-plugin Qt5::Core)
target_link_libraries(spol-plugin ${GPUI_LIBRARIES})
