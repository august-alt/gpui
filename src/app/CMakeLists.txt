macro(add_gpui_executable name)
  # add executable
  add_executable(${name} ${ARGN})

  # add install target
  set_target_properties(${name} PROPERTIES OUTPUT_NAME gpui-${name})
  install(TARGETS ${name} DESTINATION bin
      PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

  # copy binary
  add_custom_command(TARGET ${name} POST_BUILD COMMAND
      ${CMAKE_COMMAND} ARGS -E copy
        $<TARGET_FILE:${name}>
        ${GPUI_BINARY_DIR}/bin/$<TARGET_FILE_NAME:${name}>
      COMMENT "Copy ${name} to ${GPUI_BINARY_DIR}/bin")

endmacro(add_gpui_executable)

find_package(GPUI COMPONENTS gui model REQUIRED)
include_directories(${GPUI_INCLUDE_DIRS})

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(HEADERS
)

set(SOURCES
    main.cpp
)

add_gpui_executable(main ${SOURCES})

target_link_libraries(main ${GPUI_LIBRARIES})
target_link_libraries(main Qt5::Widgets)
