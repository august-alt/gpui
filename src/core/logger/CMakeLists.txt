find_package(Qt5 COMPONENTS Core Gui REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB HEADERS "*.h")

file(GLOB SOURCES "*.cpp")

add_definitions(
    -DGPUI_CORE_LIBRARY
)

add_library(gpui-logger STATIC ${SOURCES} ${HEADERS})
target_include_directories(gpui-logger PUBLIC ${Qt5Gui_INCLUDE_DIRS})
target_link_libraries(gpui-logger Qt5::Core Qt5::Gui)
