find_package(Qt5 COMPONENTS Core REQUIRED)

set(HEADERS
    genericfile.h
    io.h
    policyfile.h
    policyfileformat.h
)

set(SOURCES
    genericfile.cpp
    io.cpp
    policyfile.cpp
    policyfileformat.cpp
)

add_definitions(
    -DGPUI_IO_LIBRARY
)

add_gpui_library(gpui-io ${SOURCES})
target_link_libraries(gpui-io Qt5::Core)
