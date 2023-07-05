find_package(GPUI COMPONENTS core io REQUIRED)
include_directories(${GPUI_INCLUDE_DIRS})

find_package(Qt5 COMPONENTS Core REQUIRED)

set(HEADERS
    cmtlformat.h
)

set(SOURCES
    cmtlformat.cpp
    cmtlplugin.cpp
)

add_subdirectory(schema)

add_gpui_plugin(cmtl-plugin ${SOURCES})
target_link_libraries(cmtl-plugin Qt5::Core)
target_link_libraries(cmtl-plugin ${GPUI_LIBRARIES} policy-common)
target_link_libraries(cmtl-plugin cmtl_xsd_library)
target_link_libraries(cmtl-plugin xerces-c)
