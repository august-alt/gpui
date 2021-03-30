find_package(GPUI COMPONENTS io model REQUIRED)
include_directories(${GPUI_INCLUDE_DIRS})

find_package(Qt5 COMPONENTS Core REQUIRED)

set(HEADERS
    admxformat.h
)

set(SOURCES
    admxformat.cpp
    admxplugin.cpp
)

add_gpui_plugin(admx-plugin ${SOURCES})
target_link_libraries(admx-plugin Qt5::Core)
target_link_libraries(admx-plugin ${GPUI_LIBRARIES} policy-common)
