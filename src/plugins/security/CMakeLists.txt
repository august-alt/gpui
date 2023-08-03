find_package(Qt5 COMPONENTS Widgets Qml LinguistTools REQUIRED)

find_package(GPUI COMPONENTS core io REQUIRED)

set(PLUGIN_NAME security-plugin)
set(PLUGIN_STATIC_LIBRARY security_static)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

include_directories(${CMAKE_SOURCE_DIR}/src)

add_subdirectory(schema)

set(HEADERS
    model/presentation/category.h
    model/presentation/checkbox.h
    model/presentation/combobox.h
    model/presentation/comment.h
    model/presentation/dataelementcontent.h
    model/presentation/dataelement.h
    model/presentation/decimaltextbox.h
    model/presentation/dropdownlist.h
    model/presentation/groupbox.h
    model/presentation/ldapsearchdialog.h
    model/presentation/listbox.h
    model/presentation/longdecimaltextbox.h
    model/presentation/multitextbox.h
    model/presentation/presentationelements.h
    model/presentation/radiobutton.h
    model/presentation/securitydescriptoreditor.h
    model/presentation/securitypresentation.h
    model/presentation/textbox.h
    model/sdmx/booleanelement.h
    model/sdmx/decimalelement.h
    model/sdmx/decimaltype.h
    model/sdmx/enumerationelement.h
    model/sdmx/listelement.h
    model/sdmx/longdecimalelement.h
    model/sdmx/longdecimaltype.h
    model/sdmx/multitextelement.h
    model/sdmx/securitydefinition.h
    model/sdmx/securitydefinitions.h
    model/sdmx/securityelement.h
    model/sdmx/securityelements.h
    model/sdmx/security.h
    model/sdmx/securityitem.h
    model/sdmx/securityvalue.h
    model/sdmx/securityvaluelist.h
    model/sdmx/textelement.h
)

set(SOURCES
)

set(UI_FORMS
)

set(PLUGIN_HEADERS
    securitysnapin.h
)

set(PLUGIN_SOURCES
    securityplugin.cpp
    securitysnapin.cpp
)

file(GLOB_RECURSE TS_FILES ${CMAKE_CURRENT_SOURCE_DIR} *.ts)

set_source_files_properties(${TS_FILES} PROPERTIES OUTPUT_LOCATION ${CMAKE_CURRENT_SOURCE_DIR})

add_translation(QM_FILES ${TS_FILES})

add_translation_resource(RESOURCES_SRC "security" ${QM_FILES})

qt5_add_resources(LIB_RESOURCES ${RESOURCES_SRC} ${ICON_RESOURCES})

set(PLUGIN_SOURCES ${PLUGIN_SOURCES} ${PLUGIN_HEADERS})

set(SOURCES ${SOURCES} ${HEADERS} ${UI_FORMS})

add_library(${PLUGIN_STATIC_LIBRARY} STATIC ${SOURCES})
target_link_libraries(${PLUGIN_STATIC_LIBRARY} Qt5::Core Qt5::Widgets)
target_link_libraries(${PLUGIN_STATIC_LIBRARY} ${GPUI_LIBRARIES})

target_link_libraries(${PLUGIN_STATIC_LIBRARY} security_static_xsd_library)
target_link_libraries(${PLUGIN_STATIC_LIBRARY} xerces-c)

add_gpui_plugin(${PLUGIN_NAME} ${PLUGIN_SOURCES} ${LIB_RESOURCES})
target_link_libraries(${PLUGIN_NAME} ${PLUGIN_STATIC_LIBRARY})
