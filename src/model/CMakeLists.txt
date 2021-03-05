set(HEADERS
    common.h
    model.h
)

set(SOURCES
    model.cpp
)

add_definitions(
    -DGPUI_MODEL_LIBRARY
)

add_gpui_library(gpui-model ${SOURCES})
target_link_libraries(gpui-model)
