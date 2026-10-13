find_package(GPUI COMPONENTS core io REQUIRED)
include_directories(${GPUI_INCLUDE_DIRS})

find_package(Qt5 COMPONENTS Core REQUIRED)

set(PLUGIN_NAME "sdmx-plugin")

set(HEADERS
    sdmxformat.h
)

set(SOURCES
    sdmxformat.cpp
    sdmxplugin.cpp
)

set(SOURCES ${SOURCES} ${HEADERS})

add_gpui_plugin(${PLUGIN_NAME} ${SOURCES})
target_link_libraries(${PLUGIN_NAME} Qt5::Core)
target_link_libraries(${PLUGIN_NAME} ${GPUI_LIBRARIES})
target_link_libraries(${PLUGIN_NAME} policy-common)
target_link_libraries(${PLUGIN_NAME} security_static_xsd_library)
target_link_libraries(${PLUGIN_NAME} xerces-c)
