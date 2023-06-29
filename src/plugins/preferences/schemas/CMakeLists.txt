set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

set(XSD_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/datasourcesschema.xsd"
    "${CMAKE_CURRENT_SOURCE_DIR}/devicesschema.xsd"
    "${CMAKE_CURRENT_SOURCE_DIR}/drivesschema.xsd"
    "${CMAKE_CURRENT_SOURCE_DIR}/filesschema.xsd"
    "${CMAKE_CURRENT_SOURCE_DIR}/foldersschema.xsd"
    "${CMAKE_CURRENT_SOURCE_DIR}/folderoptionsschema.xsd"
    "${CMAKE_CURRENT_SOURCE_DIR}/groupsschema.xsd"
    "${CMAKE_CURRENT_SOURCE_DIR}/inischema.xsd"
    "${CMAKE_CURRENT_SOURCE_DIR}/networkoptionsschema.xsd"
    "${CMAKE_CURRENT_SOURCE_DIR}/printersschema.xsd"
    "${CMAKE_CURRENT_SOURCE_DIR}/poweroptionsschema.xsd"
    "${CMAKE_CURRENT_SOURCE_DIR}/registryschema.xsd"
    "${CMAKE_CURRENT_SOURCE_DIR}/sharesschema.xsd"
    "${CMAKE_CURRENT_SOURCE_DIR}/shortcutsschema.xsd"
    "${CMAKE_CURRENT_SOURCE_DIR}/startmenuschema.xsd"
    "${CMAKE_CURRENT_SOURCE_DIR}/targetingschema.xsd"
    "${CMAKE_CURRENT_SOURCE_DIR}/variablesschema.xsd"
)

add_xsd_library(static_xsd_library ${XSD_FILES})
