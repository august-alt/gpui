find_package(Qt5 COMPONENTS Core Gui REQUIRED)

set(HEADERS
    common.h
    model.h
    plugin.h
    pluginstorage.h
    pluginstorage.h
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
    bundle/policybundle.h
    bundle/policyroles.h
    bundle/policytreemodel.h
    bundle/policytreemodelitem.h
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
)

set(SOURCES
    model.cpp
    plugin.cpp
    pluginstorage.cpp
    bundle/policybundle.cpp
    commands/command.cpp
    commands/commandgroup.cpp
    presentation/presentationwidget.cpp    
    registry/abstractregistrysource.cpp
    registry/policystatemanager.cpp
    registry/polregistrysource.cpp
)

add_definitions(
    -DGPUI_MODEL_LIBRARY
)

add_gpui_library(gpui-model ${SOURCES})
target_link_libraries(gpui-model Qt5::Core Qt5::Gui)
target_link_libraries(gpui-model gpui-io)
