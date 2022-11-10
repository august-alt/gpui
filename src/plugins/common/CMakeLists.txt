find_package(GPUI COMPONENTS core io REQUIRED)
include_directories(${GPUI_INCLUDE_DIRS})

find_package(Qt5 COMPONENTS Core REQUIRED)

set(HEADERS
    basetypes.h
    exceptionhandler.h
    policydefinitionfiles.h
    policydefinitions.h
)

set(SOURCES
    basetypes.cpp
    exceptionhandler.cpp
    policydefinitionfiles.cpp
    policydefinitions.cpp
)

add_library(policy-common STATIC ${SOURCES})
target_link_libraries(policy-common Qt5::Core)
target_link_libraries(policy-common ${GPUI_LIBRARIES})
