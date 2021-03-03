set(HEADERS
    io.h
)

set(SOURCES
    io.cpp
)

add_definitions(
    -DGPUI_IO_LIBRARY
)

add_gpui_library(gpui-io ${SOURCES})
target_link_libraries(gpui-io)
