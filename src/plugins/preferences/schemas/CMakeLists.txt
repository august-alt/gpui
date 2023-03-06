cmake_policy(SET CMP0071 NEW)

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

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

foreach(FILE ${XSD_FILES})
    string(REGEX REPLACE "[.]xsd" ".h" HEADER ${FILE})
    set(HEADERS ${HEADERS} ${HEADER})
endforeach()

foreach(FILE ${XSD_FILES})
    string(REGEX REPLACE "[.]xsd" ".cpp" SOURCE ${FILE})
    set(SOURCES ${SOURCES} ${SOURCE})
endforeach()

set(OUT_FILES
    ${HEADERS}
    ${SOURCES}
)

set(XSD_FLAGS cxx-tree
    --generate-doxygen
    --generate-ostream
    --generate-serialization
    --generate-polymorphic
    --polymorphic-type-all
    --generate-wildcard
    --cxx-suffix .cpp
    --hxx-suffix .h
    --std c++11
    --type-naming ucc
    --root-element-all
)

add_custom_command(
    OUTPUT ${OUT_FILES}
    COMMAND ${XSDCXX_EXECUTABLE} ${XSD_FLAGS} ${XSD_FILES}
    DEPENDS ${XSD_FILES}
    COMMENT "Running ${XSDCXX_EXECUTABLE} ${XSD_FLAGS} ${XSD_FILES}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND_EXPAND_LISTS
)

add_library(static_xsd_library STATIC ${OUT_FILES})
