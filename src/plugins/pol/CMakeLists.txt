find_package(GPUI COMPONENTS core io REQUIRED)
include_directories(${GPUI_INCLUDE_DIRS})

find_package(Qt5 COMPONENTS Core REQUIRED)

set(HEADERS
    polformat.h

    pregdata.h
    pregparser.h
    iconvwrapper.h
)

set(SOURCES
    polplugin.cpp
    polformat.cpp

    pregdata.cpp
    pregparser.cpp
    pregwriter.cpp
    iconvwrapper.cpp
)

add_gpui_plugin(pol-plugin ${SOURCES})
target_link_libraries(pol-plugin Qt5::Core)
target_link_libraries(pol-plugin ${GPUI_LIBRARIES})
