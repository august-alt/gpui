find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)

find_package(GPUI COMPONENTS core io REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(PLUGIN_NAME comments-plugin)

set(HEADERS
    comment.h
    commentdefinitionresources.h
    localizationresourcereference.h
    policycomments.h
    policynamespace.h
    policynamespaceassociation.h
)

set(SOURCES
)

set(UI_FORMS
)

set(SOURCES ${SOURCES} ${HEADERS} ${UI_FORMS})

add_definitions(
    -DGPUI_COMMENTS_PLUGIN
)

set(TS_FILES
)

set_source_files_properties(${TS_FILES} PROPERTIES OUTPUT_LOCATION ${CMAKE_CURRENT_SOURCE_DIR})

qt5_add_translation(QM_FILES ${TS_FILES})

add_translation_resource(RESOURCES_SRC "comments" ${QM_FILES})

qt5_add_resources(LIB_RESOURCES ${RESOURCES_SRC} ${ICON_RESOURCES})

add_gpui_plugin(${PLUGIN_NAME} ${SOURCES} ${LIB_RESOURCES})
target_link_libraries(${PLUGIN_NAME} Qt5::Core)
target_link_libraries(${PLUGIN_NAME} ${GPUI_CORE_LIBRARY} ${GPUI_GUI_LIBRARY} ${GPUI_IO_LIBRARY})
