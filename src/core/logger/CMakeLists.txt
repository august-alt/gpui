set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB HEADERS "*.h")

file(GLOB SOURCES "*.cpp")

set(SOURCES ${HEADERS} ${SOURCES})

add_definitions(
    -DGPUI_CORE_LIBRARY
)

add_gpui_library(gpui-logger ${SOURCES})
