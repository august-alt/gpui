cmake_minimum_required(VERSION 2.8)

project(GPUI)

execute_process(
    COMMAND
    bash -c "grep 'Version:' .gear/gpui.spec | awk -F ':' '{ print $2 }' | tr -d [:space:]"
    OUTPUT_VARIABLE GPUI_GEAR_VERSION
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

string(REPLACE "." ";" GEAR_VERSION_LIST ${GPUI_GEAR_VERSION})
list(GET GEAR_VERSION_LIST 0 GPUI_VERSION_MAJOR)
list(GET GEAR_VERSION_LIST 1 GPUI_VERSION_MINOR)
list(GET GEAR_VERSION_LIST 2 GPUI_VERSION_PATCH)

set(CMAKE_MODULE_PATH ${GPUI_SOURCE_DIR}/cmake)

set(GPUI_DIR "${CMAKE_BINARY_DIR}")

include(TranslationUtils)
include(FindXsd)
include(FindLdap)

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
