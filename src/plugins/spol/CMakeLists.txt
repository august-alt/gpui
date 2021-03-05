set(HEADERS
    spolformat.h
)

set(SOURCES
    spolformat.cpp
    spolplugin.cpp
)

add_gpui_plugin(spol-plugin ${SOURCES})
