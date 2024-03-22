find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/version.h.in
               ${CMAKE_CURRENT_SOURCE_DIR}/version.h)

file(GLOB HEADERS "*.h")

file(GLOB SOURCES "*.cpp")

file(GLOB UIFORMS "*.ui")

set(SOURCES ${HEADERS} ${SOURCES} ${UIFORMS})

add_definitions(
    -DGPUI_CORE_LIBRARY
)

add_gpui_library(gpui-core ${SOURCES})
target_link_libraries(gpui-core)

target_link_libraries(gpui-core Qt5::Core Qt5::Gui Qt5::Widgets)

