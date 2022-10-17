find_package(GPUI COMPONENTS core io REQUIRED)

find_package(MVVM REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

add_subdirectory(schemas)

set(HEADERS
    attributes/attributesitem.h

    common/basemodelbuilder.h
    common/basepreferenceitem.h
    common/basepreferencewidget.h
    common/basepreferencereader.h
    common/basepreferencewriter.h
    common/commonitem.h
    common/commonutils.h
    common/commonview.h
    common/preferencecategoryitem.h
    common/preferencesdialog.h
    common/preferencesmodel.h
    common/propertieswidget.h
    common/tabledetailswidget.h
    common/widgetfactory.h

    datasource/datasourcecontaineritem.h
    datasource/datasourceitem.h
    datasource/datasourcemodelbuilder.h
    datasource/datasourcewidget.h

    device/deviceitem.h
    device/devicecontaineritem.h
    device/devicewidget.h

    drives/drivesitem.h
    drives/drivescontaineritem.h
    drives/drivesmodelbuilder.h
    drives/driveswidget.h

    folders/foldercontaineritem.h
    folders/folderpreferencereader.h
    folders/folderpreferencewriter.h
    folders/folderitem.h
    folders/foldermodelbuilder.h
    folders/folderwidget.h

    folder_options/folderoptionscontaineritem.h
    folder_options/folderoptionscontaineritem.inl
    folder_options/folderoptionswidget.h
    folder_options/folderoptionsitem.h
    folder_options/folderoptionsmodelbuilder.h
    folder_options/openwithitem.h
    folder_options/openwithwidget.h
    folder_options/fileextensionactionitem.h
    folder_options/fileextensioncontaineritem.h
    folder_options/fileextensionitem.h
    folder_options/fileextensionwidget.h

    files/filesitem.h
    files/filespreferencereader.h
    files/filespreferencewriter.h
    files/filescontaineritem.h
    files/filesmodelbuilder.h
    files/fileswidget.h

    ini/iniitem.h
    ini/inipreferencereader.h
    ini/inipreferencewriter.h
    ini/inicontaineritem.h
    ini/inimodelbuilder.h
    ini/iniwidget.h

    interfaces/preferencesreaderinterface.h
    interfaces/preferenceswriterinterface.h

    local_users_and_groups/baselocalitem.h
    local_users_and_groups/groupmemberdialog.h
    local_users_and_groups/groupmemberitem.h
    local_users_and_groups/groupmodelbuilder.h
    local_users_and_groups/groupmemberswidget.h
    local_users_and_groups/groupmemberscontaineritem.h
    local_users_and_groups/localgroupcontaineritem.h
    local_users_and_groups/localgroupcontaineritem.inl
    local_users_and_groups/localgroupitem.h
    local_users_and_groups/localgroupwidget.h
    local_users_and_groups/localuseritem.h
    local_users_and_groups/localuserwidget.h

    network_options/basenetworkitem.h
    network_options/dialupitem.h
    network_options/dialupwidget.h
    network_options/networkmodelbuilder.h
    network_options/networkcontaineritem.h
    network_options/networkcontaineritem.inl
    network_options/vpnitem.h
    network_options/vpnnetworkingwidget.h
    network_options/vpnoptionswidget.h
    network_options/vpnsecuritywidget.h
    network_options/vpnwidget.h

    interfaces/containeriteminterface.h
    interfaces/preferenceswidgetinterface.h
    interfaces/widgetfactoryinterface.h

    power_options/powercontaineritem.h
    power_options/poweroptionsitem.h
    power_options/poweroptionsitem.inl
    power_options/powermodelbuilder.h
    power_options/poweroptionswidget.h
    power_options/powerplanitem.h
    power_options/powerplanitem.inl
    power_options/powerplanwidget.h
    power_options/powerschemeitem.h
    power_options/powerschemeitem.inl
    power_options/powerschemewidget.h

    printers/localprinteritem.h
    printers/localprinterwidget.h
    printers/printersmodelbuilder.h
    printers/printercontaineritem.h
    printers/printercontaineritem.inl
    printers/sharedprinteritem.h
    printers/sharedprinterwidget.h
    printers/tcpprintergeneralwidget.h
    printers/tcpprinteritem.h
    printers/tcpprinterportwidget.h

    registry/registrycontaineritem.h
    registry/registrypreferencereader.h
    registry/registrypreferencewriter.h
    registry/registryitem.h
    registry/registrymodelbuilder.h
    registry/registrywidget.h

    scheduled_tasks/immediateactionwin7widget.h
    scheduled_tasks/immediateconditionswin7widget.h
    scheduled_tasks/immediatesettingswidget.h
    scheduled_tasks/immediatesettingswin7widget.h
    scheduled_tasks/immediatetaskswidget.h
    scheduled_tasks/immediatewin7widget.h
    scheduled_tasks/scheduleactionwin7widget.h
    scheduled_tasks/scheduleconditionswin7widget.h
    scheduled_tasks/scheduledtaskswidget.h
    scheduled_tasks/scheduleschedulewidget.h
    scheduled_tasks/schedulesettingswidget.h
    scheduled_tasks/schedulesettingswin7widget.h
    scheduled_tasks/scheduletriggerswin7widget.h
    scheduled_tasks/schedulewin7widget.h

    start_menu/startadvancedwinxpwidget.h
    start_menu/startclassicvistawidget.h
    start_menu/startclassicwinxpwidget.h
    start_menu/startvistawidget.h
    start_menu/startwinxpwidget.h
    start_menu/startmenuwinvistaitem.h
    start_menu/startmenuwinxpitem.h

    shares/sharesitem.h
    shares/sharespreferencereader.h
    shares/sharespreferencewriter.h
    shares/sharesmodelbuilder.h
    shares/sharescontaineritem.h
    shares/shareswidget.h

    shortcuts/shortcutsitem.h
    shortcuts/shortcutspreferencereader.h
    shortcuts/shortcutspreferencewriter.h
    shortcuts/shortcutsmodelbuilder.h
    shortcuts/shortcutscontaineritem.h
    shortcuts/shortcutswidget.h

    variables/variablesitem.h
    variables/variablespreferencereader.h
    variables/variablespreferencewriter.h
    variables/variablesmodelbuilder.h
    variables/variablescontaineritem.h
    variables/variableswidget.h

    preferencestreemodel.h
    preferencestreeproxymodel.h

    preferenceswidget.h
)

set(SOURCES
    attributes/attributesitem.cpp

    common/basemodelbuilder.cpp
    common/basepreferencewidget.cpp
    common/basepreferencereader.cpp
    common/basepreferencewriter.cpp
    common/commonitem.cpp
    common/commonutils.cpp
    common/commonview.cpp
    common/preferencecategoryitem.cpp
    common/preferencesdialog.cpp
    common/preferencesmodel.cpp
    common/propertieswidget.cpp
    common/tabledetailswidget.cpp
    common/widgetfactory.cpp

    datasource/datasourceitem.cpp
    datasource/datasourcecontaineritem.cpp
    datasource/datasourcemodelbuilder.cpp
    datasource/datasourcewidget.cpp
    datasource/datasourcewidgetslots.cpp

    device/deviceitem.cpp
    device/devicecontaineritem.cpp
    drives/drivesmodelbuilder.cpp
    device/devicewidget.cpp

    drives/drivesitem.cpp
    drives/drivescontaineritem.cpp
    drives/driveswidget.cpp
    drives/driveswidgetslots.cpp

    folders/foldercontaineritem.cpp
    folders/folderpreferencereader.cpp
    folders/folderpreferencewriter.cpp
    folders/folderitem.cpp
    folders/foldermodelbuilder.cpp
    folders/folderwidget.cpp
    folders/folderviewslots.cpp

    folder_options/folderoptionswidget.cpp
    folder_options/folderoptionsitem.cpp
    folder_options/folderoptionsmodelbuilder.cpp
    folder_options/openwithitem.cpp
    folder_options/openwithwidget.cpp
    folder_options/openwithwidgetslots.cpp
    folder_options/fileextensionactionitem.cpp
    folder_options/fileextensioncontaineritem.cpp
    folder_options/fileextensionitem.cpp
    folder_options/fileextensionwidget.cpp

    files/filesitem.cpp
    files/filespreferencereader.cpp
    files/filespreferencewriter.cpp
    files/filescontaineritem.cpp
    files/filesmodelbuilder.cpp
    files/fileswidget.cpp
    files/fileswidgetslots.cpp

    ini/iniitem.cpp
    ini/inipreferencereader.cpp
    ini/inipreferencewriter.cpp
    ini/inicontaineritem.cpp
    ini/inimodelbuilder.cpp
    ini/iniwidget.cpp
    ini/iniwidgetslots.cpp

    local_users_and_groups/baselocalitem.cpp
    local_users_and_groups/groupmemberdialog.cpp
    local_users_and_groups/groupmemberitem.cpp
    local_users_and_groups/groupmodelbuilder.cpp
    local_users_and_groups/groupmemberswidget.cpp
    local_users_and_groups/groupmemberswidgetslots.cpp
    local_users_and_groups/localgroupitem.cpp
    local_users_and_groups/localgroupwidget.cpp
    local_users_and_groups/localgroupwidgetslots.cpp
    local_users_and_groups/localuseritem.cpp
    local_users_and_groups/localuserwidget.cpp
    local_users_and_groups/localuserwidgetslots.cpp

    network_options/basenetworkitem.cpp
    network_options/dialupitem.cpp
    network_options/dialupwidget.cpp
    network_options/dialupwidgetslots.cpp
    network_options/networkmodelbuilder.cpp
    network_options/vpnitem.cpp
    network_options/vpnnetworkingwidget.cpp
    network_options/vpnoptionswidget.cpp
    network_options/vpnsecuritywidget.cpp
    network_options/vpnwidget.cpp
    network_options/vpnwidgetslots.cpp

    power_options/powercontaineritem.inl
    power_options/poweroptionsitem.cpp
    power_options/powermodelbuilder.cpp
    power_options/poweroptionswidget.cpp
    power_options/powerplanitem.cpp
    power_options/powerplanwidget.cpp
    power_options/powerschemeitem.cpp
    power_options/powerschemewidget.cpp

    printers/localprinteritem.cpp
    printers/localprinterwidget.cpp
    printers/printersmodelbuilder.cpp
    printers/sharedprinteritem.cpp
    printers/sharedprinterwidget.cpp
    printers/tcpprintergeneralwidget.cpp
    printers/tcpprinteritem.cpp
    printers/tcpprinterportwidget.cpp

    scheduled_tasks/immediateactionwin7widget.cpp
    scheduled_tasks/immediateconditionswin7widget.cpp
    scheduled_tasks/immediatesettingswidget.cpp
    scheduled_tasks/immediatesettingswin7widget.cpp
    scheduled_tasks/immediatetaskswidget.cpp
    scheduled_tasks/immediatewin7widget.cpp
    scheduled_tasks/scheduleactionwin7widget.cpp
    scheduled_tasks/scheduleconditionswin7widget.cpp
    scheduled_tasks/scheduledtasksmodelbuilder.cpp
    scheduled_tasks/scheduledtaskswidget.cpp
    scheduled_tasks/scheduleschedulewidget.cpp
    scheduled_tasks/schedulesettingswidget.cpp
    scheduled_tasks/schedulesettingswin7widget.cpp
    scheduled_tasks/scheduletriggerswin7widget.cpp
    scheduled_tasks/schedulewin7widget.cpp

    start_menu/startadvancedwinxpwidget.cpp
    start_menu/startclassicvistawidget.cpp
    start_menu/startclassicwinxpwidget.cpp
    start_menu/startvistawidget.cpp
    start_menu/startwinxpwidget.cpp
    start_menu/startmenuwinvistaitem.cpp
    start_menu/startmenuwinxpitem.cpp

    registry/registrycontaineritem.cpp
    registry/registrypreferencereader.cpp
    registry/registrypreferencewriter.cpp
    registry/registryitem.cpp
    registry/registrymodelbuilder.cpp
    registry/registrywidget.cpp
    registry/registrywidgetslots.cpp

    shares/sharesitem.cpp
    shares/sharespreferencereader.cpp
    shares/sharespreferencewriter.cpp
    shares/sharescontaineritem.cpp
    shares/sharesmodelbuilder.cpp
    shares/shareswidget.cpp
    shares/shareswidgetslots.cpp

    shortcuts/shortcutsitem.cpp
    shortcuts/shortcutspreferencereader.cpp
    shortcuts/shortcutspreferencewriter.cpp
    shortcuts/shortcutscontaineritem.cpp
    shortcuts/shortcutsmodelbuilder.cpp
    shortcuts/shortcutswidget.cpp
    shortcuts/shortcutswidgetslots.cpp

    variables/variablesitem.cpp
    variables/variablespreferencereader.cpp
    variables/variablespreferencewriter.cpp
    variables/variablescontaineritem.cpp
    variables/variablesmodelbuilder.cpp
    variables/variableswidget.cpp
    variables/variableswidgetslots.cpp

    preferencestreemodel.cpp
    preferencestreeproxymodel.cpp

    preferenceswidget.cpp
)

set(UI_FORMS
    common/commonview.ui
    common/preferencesdialog.ui
    common/propertieswidget.ui
    common/tabledetailswidget.ui

    datasource/datasourcewidget.ui

    drives/driveswidget.ui

    device/devicewidget.ui

    folders/folderswidget.ui

    folder_options/folderoptionswidget.ui
    folder_options/openwithwidget.ui
    folder_options/fileextensionwidget.ui

    files/fileswidget.ui

    ini/iniwidget.ui

    local_users_and_groups/groupmemberswidget.ui
    local_users_and_groups/groupmemberdialog.ui
    local_users_and_groups/localuserwidget.ui
    local_users_and_groups/localgroupwidget.ui

    network_options/dialupwidget.ui
    network_options/vpnnetworkingwidget.ui
    network_options/vpnoptionswidget.ui
    network_options/vpnsecuritywidget.ui
    network_options/vpnwidget.ui

    power_options/poweroptionswidget.ui
    power_options/powerplanwidget.ui
    power_options/powerschemewidget.ui

    printers/localprinterwidget.ui
    printers/sharedprinterwidget.ui
    printers/tcpprintergeneralwidget.ui
    printers/tcpprinterportwidget.ui

    registry/registrywidget.ui

    scheduled_tasks/immediateactionwin7widget.ui
    scheduled_tasks/immediateconditionswin7widget.ui
    scheduled_tasks/immediatesettingswidget.ui
    scheduled_tasks/immediatesettingswin7widget.ui
    scheduled_tasks/immediatetaskswidget.ui
    scheduled_tasks/immediatewin7widget.ui
    scheduled_tasks/scheduleactionwin7widget.ui
    scheduled_tasks/scheduleconditionswin7widget.ui
    scheduled_tasks/scheduledtaskswidget.ui
    scheduled_tasks/scheduleschedulewidget.ui
    scheduled_tasks/schedulesettingswidget.ui
    scheduled_tasks/schedulesettingswin7widget.ui
    scheduled_tasks/scheduletriggerswin7widget.ui
    scheduled_tasks/schedulewin7widget.ui

    start_menu/startadvancedwinxpwidget.ui
    start_menu/startclassicvistawidget.ui
    start_menu/startclassicwinxpwidget.ui
    start_menu/startvistawidget.ui
    start_menu/startwinxpwidget.ui

    shares/shareswidget.ui

    shortcuts/shortcutswidget.ui

    variables/variableswidget.ui

    preferenceswidget.ui
)

set(PLUGIN_HEADERS
    modelcreator.h
    modelwriter.h
    preferencessnapin.h
    preferencessnapinprivate.h
    preferencestreemodel.h
    preferencestreeproxymodel.h
)

set(PLUGIN_SOURCES
    modelcreator.cpp
    modelwriter.cpp
    preferencesplugin.cpp
    preferencessnapin.cpp
    preferencessnapinprivate.cpp
    preferencestreemodel.cpp
    preferencestreeproxymodel.cpp
)

set(PLUGIN_SOURCES ${PLUGIN_SOURCES} ${PLUGIN_HEADERS})

set(SOURCES ${SOURCES} ${HEADERS} ${UI_FORMS})

include_directories(${CMAKE_SOURCE_DIR}/src)

set(PLUGIN_NAME preferences-plugin)

add_library(preferences_static STATIC ${SOURCES})
target_link_libraries(preferences_static Qt5::Core Qt5::Widgets)
target_link_libraries(preferences_static ${GPUI_LIBRARIES})
target_link_libraries(preferences_static ${MVVM_LIBRARIES})

target_link_libraries(preferences_static static_xsd_library)
target_link_libraries(preferences_static xerces-c)

add_gpui_plugin(${PLUGIN_NAME} ${PLUGIN_SOURCES})
target_link_libraries(${PLUGIN_NAME} preferences_static)

