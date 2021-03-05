set(HEADERS
    regformat.h
)

set(SOURCES
    regformat.cpp
    regplugin.cpp
)

add_gpui_plugin(reg-plugin ${SOURCES})
