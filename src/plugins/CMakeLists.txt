macro(add_gpui_plugin name)
  # add plugin
  add_library(${name} SHARED ${ARGN})

  # add install target
  install(TARGETS ${name} DESTINATION ${CMAKE_INSTALL_LIBDIR}/gpui/plugins
      PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

  # copy plugin into build directory
  add_custom_command(TARGET ${name} POST_BUILD COMMAND
      ${CMAKE_COMMAND} ARGS -E copy
        $<TARGET_FILE:${name}>
        ${GPUI_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/gpui/plugins/$<TARGET_FILE_NAME:${name}>
      COMMENT "Copying plugin ${name} to ${GPUI_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/gpui/plugins directory")

endmacro(add_gpui_plugin)

add_subdirectory(administrative_templates)
add_subdirectory(adml)
add_subdirectory(admx)
add_subdirectory(cmtl)
add_subdirectory(cmtx)
add_subdirectory(common)
add_subdirectory(ini)
add_subdirectory(pol)
add_subdirectory(preferences)
add_subdirectory(reg)
add_subdirectory(scripts)
add_subdirectory(security)
add_subdirectory(spol)
add_subdirectory(storage)
