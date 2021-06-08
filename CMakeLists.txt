cmake_minimum_required(VERSION 2.8)

project(GPUI)

set(GPUI_VERSION_MAJOR 0)
set(GPUI_VERSION_MINOR 1)

set(CMAKE_MODULE_PATH ${GPUI_SOURCE_DIR}/cmake)

set(GPUI_DIR "${CMAKE_BINARY_DIR}")

if(UNIX)
  include(GNUInstallDirs)
else()
  if (WIN32)
    set(${CMAKE_INSTALL_LIBDIR} "lib")
    set(${CMAKE_INSTALL_DATADIR} "share")
    set(${CMAKE_INSTALL_INCLUDEDIR} "include")
    set(${CMAKE_INSTALL_BINDIR} "bin")
    message(STATUS "Setting installation destination on Windows to: ${CMAKE_INSTALL_PREFIX}")
  else()
    message(FATAL_ERROR "System not UNIX nor WIN32 - not implemented yet")
  endif()
endif()

# configure and install GPUIConfig.cmake file
configure_file(
    "${CMAKE_MODULE_PATH}/GPUIConfig.cmake.in"
    "${CMAKE_BINARY_DIR}/GPUIConfig.cmake"
    IMMEDIATE @ONLY)

# export the GPUI package
export(PACKAGE GPUI)

option(GPUI_BUILD_TESTS "Build the gpui tests." OFF)

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
