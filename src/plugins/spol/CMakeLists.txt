find_package(GPUI COMPONENTS io model REQUIRED)
include_directories(${GPUI_INCLUDE_DIRS})

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

set(HEADERS
    spolformat.h
)

set(SOURCES
    spolformat.cpp
    spolplugin.cpp
)

add_gpui_plugin(spol-plugin ${SOURCES})
target_link_libraries(spol-plugin Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(spol-plugin ${GPUI_LIBRARIES})
