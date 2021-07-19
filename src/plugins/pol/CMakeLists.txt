find_package(Qt5 COMPONENTS Core REQUIRED)

set(HEADERS
    polformat.h
)

set(SOURCES
    polformat.cpp
    polplugin.cpp
)

add_gpui_plugin(pol-plugin ${SOURCES})
target_link_libraries(pol-plugin Qt5::Core)
