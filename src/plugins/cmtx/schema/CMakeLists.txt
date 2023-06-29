set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

set(XSD_SCHEMAS
    "${CMAKE_CURRENT_SOURCE_DIR}/cmtx.xsd"
)

add_xsd_library(cmtx_xsd_library ${XSD_SCHEMAS})
