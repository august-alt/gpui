find_package(Qt5 COMPONENTS Core LinguistTools REQUIRED)

find_package(MVVM REQUIRED)

find_package(GPUI COMPONENTS core io REQUIRED)
include_directories(${PREFERENCES_EDITOR_INCLUDE_DIRS})

set(PLUGIN_NAME administrative-templates-plugin)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

include_directories("${CMAKE_SOURCE_DIR}/src/core")

set(HEADERS
    administrativetemplates.h
    administrativetemplatessnapin.h
    admx/category.h
    admx/majorversion.h
    admx/minorversion.h
    admx/policybooleanelement.h
    admx/policydecimalelement.h
    admx/policydefinitions.h
    admx/policyelement.h
    admx/policyenumelement.h
    admx/policy.h
    admx/policyitem.h
    admx/policylistelement.h
    admx/policylongdecimalelement.h
    admx/policymultitextelement.h
    admx/policytextelement.h
    admx/policytype.h
    admx/registryvalue.h
    admx/registryvaluetype.h
    admx/resources.h
    admx/supporteddefinition.h
    admx/supportedon.h
    admx/supportedproduct.h
    bundle/itemtype.h
    bundle/policybundle.h
    bundle/policyroles.h
    commands/command.h
    commands/commandgroup.h
    presentation/checkbox.h
    presentation/combobox.h
    presentation/decimaltextbox.h
    presentation/dropdownlist.h
    presentation/listbox.h
    presentation/longdecimaltextbox.h
    presentation/multitextbox.h
    presentation/policyresources.h
    presentation/presentation.h
    presentation/presentationwidget.h
    presentation/presentationwidgetvisitor.h
    presentation/textbox.h
    presentation/text.h
    registry/abstractregistryentry.h
    registry/abstractregistrysource.h
    registry/policystatemanager.h
    registry/polregistrysource.h
    registry/registry.h
    registry/registryentry.h
    registry/registryentrytype.h
    registry/registrysourcetype.h
    ui/administrativetemplateswidget.h
    ui/administrativetemplatesproxymodel.h
)

set(SOURCES
    administrativetemplatessnapin.cpp
    administrativetemplatesplugin.cpp
    bundle/policybundle.cpp
    commands/command.cpp
    commands/commandgroup.cpp
    presentation/presentationwidget.cpp
    registry/abstractregistrysource.cpp
    registry/policystatemanager.cpp
    registry/polregistrysource.cpp
    ui/administrativetemplateswidget.cpp
    ui/administrativetemplatesproxymodel.cpp
)

set(UI_FORMS
    ui/administrativetemplateswidget.ui
)

set(SOURCES ${SOURCES} ${HEADERS} ${UI_FORMS})

add_definitions(
    -DGPUI_ADMINISTRATIVE_TEMPLATES_PLUGIN
)

set(TS_FILES
    ui/i18n/administrative_templates_common_translation_en.ts
    ui/i18n/administrative_templates_common_translation_ru.ts
)

set_source_files_properties(${TS_FILES} PROPERTIES OUTPUT_LOCATION ${CMAKE_CURRENT_SOURCE_DIR})

qt5_add_translation(QM_FILES ${TS_FILES})

add_translation_resource(RESOURCES_SRC ${QM_FILES})

qt5_add_resources(LIB_RESOURCES ${RESOURCES_SRC} ${ICON_RESOURCES})

add_gpui_plugin(${PLUGIN_NAME} ${SOURCES} ${LIB_RESOURCES})
target_link_libraries(${PLUGIN_NAME} Qt5::Core)
target_link_libraries(${PLUGIN_NAME} ${GPUI_CORE_LIBRARY} ${GPUI_IO_LIBRARY})
target_link_libraries(${PLUGIN_NAME} smb-storage-static)
