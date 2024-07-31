find_package(GPUI COMPONENTS core io REQUIRED)
include_directories(${GPUI_INCLUDE_DIRS})

find_package(Qt5 COMPONENTS Core REQUIRED)

set(CMAKE_CXX_STANDARD 17)

set(HEADERS
    polformat.h
    common.h
    encoding.h
    parser.h
)

set(SOURCES
    binary.cpp
    parser.cpp

    polplugin.cpp
    polformat.cpp
)

add_gpui_plugin(pol-plugin ${SOURCES})
target_link_libraries(pol-plugin Qt5::Core)
target_link_libraries(pol-plugin ${GPUI_LIBRARIES})
