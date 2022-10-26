find_package(GPUI COMPONENTS core io REQUIRED)
include_directories(${GPUI_INCLUDE_DIRS})

find_package(Qt5 COMPONENTS Core REQUIRED)

set(HEADERS
    admlformat.h
    ../administrative_templates/presentation/presentationwidget.h
)

set(SOURCES
    admlformat.cpp
    admlplugin.cpp
    ../administrative_templates/presentation/presentationwidget.cpp
)

add_gpui_plugin(adml-plugin ${SOURCES})
target_link_libraries(adml-plugin Qt5::Core)
target_link_libraries(adml-plugin ${GPUI_LIBRARIES} policy-common)
target_link_libraries(adml-plugin xerces-c)
target_link_libraries(adml-plugin)
