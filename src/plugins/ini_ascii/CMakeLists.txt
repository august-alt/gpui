find_package(GPUI COMPONENTS core io REQUIRED)
include_directories(${GPUI_INCLUDE_DIRS})

find_package(Qt5 COMPONENTS Core REQUIRED)

set(PLUGIN_NAME inifile-ascii-plugin)

set(HEADERS
    iniformat.h
    iniformat.inl
)

set(SOURCES
    iniplugin.cpp
)

set(SOURCES ${SOURCES} ${HEADERS})

add_gpui_plugin(inifile-ascii-plugin ${SOURCES})
target_link_libraries(inifile-ascii-plugin Qt5::Core)
target_link_libraries(inifile-ascii-plugin ${GPUI_LIBRARIES})
