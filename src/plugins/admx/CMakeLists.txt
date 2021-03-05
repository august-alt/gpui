set(HEADERS
    admxformat.h
)

set(SOURCES
    admxformat.cpp
    admxplugin.cpp
)

add_gpui_plugin(admx-plugin ${SOURCES})
