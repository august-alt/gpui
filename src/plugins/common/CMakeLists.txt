find_package(GPUI COMPONENTS io model REQUIRED)
include_directories(${GPUI_INCLUDE_DIRS})

find_package(Qt5 COMPONENTS Core REQUIRED)

set(HEADERS
    basetypes.h
    policydefinitionfiles.h
    policydefinitions.h
)

set(SOURCES
    basetypes.cpp
    policydefinitionfiles.cpp
    policydefinitions.cpp
)

add_library(policy-common STATIC ${SOURCES})
target_link_libraries(policy-common Qt5::Core)
target_link_libraries(policy-common ${GPUI_LIBRARIES})
