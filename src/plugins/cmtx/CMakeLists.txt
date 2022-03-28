find_package(GPUI COMPONENTS io model REQUIRED)
include_directories(${GPUI_INCLUDE_DIRS})

find_package(Qt5 COMPONENTS Core REQUIRED)

set(HEADERS
)

set(SOURCES
    cmtxplugin.cpp
)

add_gpui_plugin(cmtx-plugin ${SOURCES})
target_link_libraries(cmtx-plugin Qt5::Core)
target_link_libraries(cmtx-plugin ${GPUI_LIBRARIES} policy-common)
target_link_libraries(cmtx-plugin xerces-c)
