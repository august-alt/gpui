# set compiler options
if (CMAKE_COMPILER_IS_GNUCXX AND NOT WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden -pedantic -Wall -Wextra -Wno-long-long")
endif()

macro(add_gpui_library name)
  # add library
  add_library(${name} SHARED ${ARGN})

  # add install target
  install(TARGETS ${name} DESTINATION ${CMAKE_INSTALL_LIBDIR}
      PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

  # copy library into build directory
  add_custom_command(TARGET ${name} POST_BUILD COMMAND
      ${CMAKE_COMMAND} ARGS -E copy
        $<TARGET_FILE:${name}>
        ${GPUI_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/$<TARGET_FILE_NAME:${name}>
      COMMENT "Copying library ${name} to output directory")

endmacro(add_gpui_library)

add_subdirectory(app)
add_subdirectory(gui)
add_subdirectory(io)
add_subdirectory(model)
add_subdirectory(plugins)
