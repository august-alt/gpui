find_package(GPUI COMPONENTS io model REQUIRED)
include_directories(${GPUI_INCLUDE_DIRS})

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

set(HEADERS
    regformat.h
)

set(SOURCES
    regformat.cpp
    regplugin.cpp
)

add_gpui_plugin(reg-plugin ${SOURCES})
target_link_libraries(reg-plugin Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(reg-plugin ${GPUI_LIBRARIES})

