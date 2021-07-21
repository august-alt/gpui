find_package(Qt5 COMPONENTS Core REQUIRED)

set(HEADERS
    genericfile.h
    genericfile.inl
    io.h
    policydefinitionsfile.h
    policyresourcesfile.h
    policyfileformat.h
    policyfileformat.inl
    registryfile.h
    registryfileformat.h
    registryfileformat.inl
)

set(SOURCES    
    io.cpp
    policydefinitionsfile.cpp
    policyresourcesfile.cpp
    registryfile.cpp
)

add_definitions(
    -DGPUI_IO_LIBRARY
)

add_gpui_library(gpui-io ${SOURCES})
target_link_libraries(gpui-io Qt5::Core)
