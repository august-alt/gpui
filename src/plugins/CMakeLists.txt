macro(add_gpui_plugin name)
  # add plugin
  add_library(${name} SHARED ${ARGN})

  # add install target
  install(TARGETS ${name} DESTINATION lib/gpui/plugins)

  # copy plugin into build directory
  add_custom_command(TARGET ${name} POST_BUILD COMMAND
      ${CMAKE_COMMAND} ARGS -E copy
        $<TARGET_FILE:${name}>
        ${GPUI_BINARY_DIR}/lib/gpui/plugins/$<TARGET_FILE_NAME:${name}>
      COMMENT "Copying plugin ${name} to ${GPUI_BINARY_DIR}/lib/gpui/plugins directory")

endmacro(add_gpui_plugin)

add_subdirectory(adml)
add_subdirectory(admx)
add_subdirectory(common)
add_subdirectory(spol)
add_subdirectory(reg)
