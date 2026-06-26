cmake_minimum_required(VERSION 3.14)

project(GPUI)
set(CMAKE_CXX_STANDARD 17)

execute_process(
    COMMAND
    bash -c "grep 'Version:' .gear/gpui.spec | awk -F ':' '{ print $2 }' | tr -d [:space:]"
    OUTPUT_VARIABLE GPUI_GEAR_VERSION
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

string(REPLACE "." ";" GEAR_VERSION_LIST ${GPUI_GEAR_VERSION})
list(GET GEAR_VERSION_LIST 0 GPUI_VERSION_MAJOR)
list(GET GEAR_VERSION_LIST 1 GPUI_VERSION_MINOR)
list(GET GEAR_VERSION_LIST 2 GPUI_VERSION_PATCH)

set(CMAKE_MODULE_PATH ${GPUI_SOURCE_DIR}/cmake)

set(GPUI_MODULE_DIR ${GPUI_SOURCE_DIR}/cmake)

set(GPUI_DIR "${CMAKE_BINARY_DIR}")

set(QT_VERSION_MAJOR 6 CACHE STRING "Major Qt version to build against (5 or 6)")
set_property(CACHE QT_VERSION_MAJOR PROPERTY STRINGS 5 6)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Qml LinguistTools Test)

if(QT_VERSION_MAJOR EQUAL 6)
    find_package(Qt6 REQUIRED COMPONENTS Core5Compat)
    set(QT_COMPAT_LIB Qt6::Core5Compat)
else()
    set(QT_COMPAT_LIB "")
endif()

include(TranslationUtils)
include(FindXsd)
include(FindLdap)
include(XsdUtils)
include(CheckCXXCompilerFlag)

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
    "${GPUI_MODULE_DIR}/GPUIConfig.cmake.in"
    "${CMAKE_BINARY_DIR}/GPUIConfig.cmake"
    IMMEDIATE @ONLY)

# export the GPUI package
export(PACKAGE GPUI)

option(GPUI_BUILD_TESTS "Build the gpui tests." OFF)

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
