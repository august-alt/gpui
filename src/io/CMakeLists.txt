find_package(Qt5 COMPONENTS Core REQUIRED)

set(HEADERS
    basefile.h
    genericfile.h
    genericfile.inl
    genericreader.h
    genericreader.inl
    genericwriter.h
    genericwriter.inl
    commentresourcesfile.h
    inifile.h
    io.h
    policycommentsfile.h
    policydefinitionsfile.h
    policyresourcesfile.h
    policyfileformat.h
    policyfileformat.inl
    registryfile.h
    registryfileformat.h
    registryfileformat.inl
)

set(SOURCES
    basefile.cpp
    commentresourcesfile.cpp
    inifile.cpp
    io.cpp
    policycommentsfile.cpp
    policydefinitionsfile.cpp
    policyresourcesfile.cpp
    registryfile.cpp
)

add_definitions(
    -DGPUI_IO_LIBRARY
)

set(SOURCES ${SOURCES} ${HEADERS})

add_gpui_library(gpui-io ${SOURCES})
target_link_libraries(gpui-io Qt5::Core)
