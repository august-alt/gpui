set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

set(XSD_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/basetypes.xsd"
    "${CMAKE_CURRENT_SOURCE_DIR}/securitydefinitions.xsd"
    "${CMAKE_CURRENT_SOURCE_DIR}/securitydefinitionfiles.xsd"
)

add_xsd_library(security_static_xsd_library ${XSD_FILES})
