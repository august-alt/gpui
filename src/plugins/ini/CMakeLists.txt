find_package(GPUI COMPONENTS core io REQUIRED)
include_directories(${GPUI_INCLUDE_DIRS})

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

set(HEADERS
    iniformat.h
    iniformat.inl
)

set(SOURCES
    iniplugin.cpp
)

set(SOURCES ${SOURCES} ${HEADERS})

add_gpui_plugin(inifile-plugin ${SOURCES})
target_link_libraries(inifile-plugin Qt${QT_VERSION_MAJOR}::Core ${QT_COMPAT_LIB})
target_link_libraries(inifile-plugin ${GPUI_LIBRARIES})
