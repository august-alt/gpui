cmake_minimum_required(VERSION 2.8)

project(GPUI)

set(GPUI_VERSION_MAJOR 0)
set(GPUI_VERSION_MINOR 1)

set(CMAKE_MODULE_PATH ${GPUI_SOURCE_DIR}/cmake)

set(GPUI_DIR "${CMAKE_BINARY_DIR}")

# configure and install GPUIConfig.cmake file
configure_file(
    "${CMAKE_MODULE_PATH}/GPUIConfig.cmake.in"
    "${CMAKE_BINARY_DIR}/GPUIConfig.cmake"
    IMMEDIATE @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/GPUIConfig.cmake DESTINATION lib/gpui)

# export the GPUI package
export(PACKAGE GPUI)

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
