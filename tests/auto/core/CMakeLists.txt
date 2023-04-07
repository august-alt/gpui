find_package(GPUI COMPONENTS gui core REQUIRED)
include_directories(${GPUI_INCLUDE_DIRS})

find_package(Qt5 COMPONENTS Core Gui REQUIRED)
find_package(GTest)

add_subdirectory(translatorstorage)
